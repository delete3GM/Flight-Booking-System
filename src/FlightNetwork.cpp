#include "FlightNetwork.h"
#include "FlightRoute.h"
#include "User.h"
#include "Utils.h"
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QElapsedTimer>
#include <QThreadPool>
#include <QMutexLocker>

FlightNetwork::FlightNetwork(QObject* parent) : QObject(parent) {}

void FlightNetwork::addCity(QString cityName) {
    for (auto* city : cities) {
        if (city->name == cityName) return;
    }
    City* newCity = new City(cityName);
    cities.append(newCity);
}

void FlightNetwork::addFlight(QString airline, QString flightNumber, QString aircratType, QString departureCity,
                              QString arrivalCity, QString departureTime, QString arrivalTime,
                              double price, int remainSeat) {
    City* depCity = nullptr;
    for (auto* city : cities) {
        if (city->name == departureCity) {
            depCity = city;
            break;
        }
    }
    if (!depCity) {
        qWarning() << "Departure city not found:" << departureCity;
        return;
    }
    std::shared_ptr<Flight> newFlight = std::make_shared<Flight>(airline, flightNumber, aircratType, departureCity, arrivalCity,
                                                                 departureTime, arrivalTime, price, remainSeat);
    FlightNode* newNode = new FlightNode(newFlight);
    if (depCity) {
        if (!depCity->flightsHead) {
            depCity->flightsHead = newNode;
        } else {
            FlightNode* temp = depCity->flightsHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        depCity->flightNum++;
    }
}

QVector<City*> FlightNetwork::getCities() const {
    return cities;
}

QVector<QString> FlightNetwork::getAllCityNames() const {
    QVector<QString> cityNames;
    for (auto* city : cities) {
        cityNames.append(city->name);
    }
    return cityNames;
}

void FlightNetwork::readFlightFromFile(const QString& file) {
    QTextStream* stream = LoadFlightFile(file);
    if (!stream) {
        qInfo() << "Stream is null!";
        return;
    }
    while(!stream->atEnd()) {
        auto lineData = stream->readLine().split(",", Qt::SkipEmptyParts);
        for (QString& part : lineData) {
            part = part.trimmed();
        }
        int nonEmptyCount = std::count_if(lineData.begin(), lineData.end(), [](const QString &str) {
            return !str.isEmpty();
        });
        if (nonEmptyCount == 9) {
            if (!lineData[3].isEmpty()) {
                addCity(lineData[3]);
            }
            if (!lineData[5].isEmpty()) {
                addCity(lineData[5]);
            }
            addFlight(lineData[0], lineData[1], lineData[2], lineData[3], lineData[4], lineData[5],
                      lineData[6], lineData[7].toDouble(), lineData[8].toInt());
        }
    }
}

void FlightNetwork::writeFlightToFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing: " << filename;
        return;
    }
    QTextStream out(&file);
    for (City* city : cities) {
        FlightNode* node = city->flightsHead;
        while (node) {
            out << QString("%1, %2, %3, %4, %5, %6, %7, %8, %9\n")
            .arg(node->flight->getAirline())
                .arg(node->flight->getFlightNumber())
                .arg(node->flight->getAircraftType())
                .arg(node->flight->getDepartureCity())
                .arg(node->flight->getDepartureTime())
                .arg(node->flight->getArrivalCity())
                .arg(node->flight->getArrivalTime())
                .arg(node->flight->getPrice())
                .arg(node->flight->getRemainSeatNum());
            node = node->next;
        }
    }
    file.close();
}

void FlightNetwork::clearData() {
    for (auto* city : cities) {
        city->flightNum = 0;
    }
    qDeleteAll(cities);
    cities.clear();
}

QVector<FlightRoute> FlightNetwork::searchFlightsDFS(const QString& departureCity,
                                                     const QString& arrivalCity,
                                                     const QDate& selectedDate) {
    QElapsedTimer timer;
    timer.start();

    QVector<FlightRoute> allPaths;

    cityMap.clear();
    for (auto* city : cities) {
        cityMap[city->name] = city;
    }
    precomputeDirectFlightPrices();

    City* startCity = cityMap.value(departureCity, nullptr);
    if (!startCity) {
        return allPaths;
    }

    QVector<FlightRoute> threadSafePaths;
    std::atomic<int> pathCount{0};
    const int MAX_PATHS = 1000;
    const int MAX_THREADS = 100;

    QMutex pathMutex;

    std::vector<std::thread> threads;

    for (FlightNode* initialNode = startCity->flightsHead;
         initialNode && pathCount < MAX_PATHS && threads.size() < MAX_THREADS;
         initialNode = initialNode->next) {

        QDateTime initialDepDateTime = QDateTime::fromString(initialNode->flight->getDepartureTime(), "yyyy-MM-dd HH:mm");
        if (initialDepDateTime.date() != selectedDate) {
            continue;
        }

        threads.emplace_back([&, initialNode]() {
            // 每个线程独立的搜索上下文
            FlightRoute localPath;
            QSet<City*> localVisited;
            QVector<FlightRoute> localPaths;

            localPath.append(initialNode->flight);
            City* nextCity = cityMap.value(initialNode->flight->getArrivalCity(), nullptr);

            dfs(arrivalCity, selectedDate, nextCity, localPath, localPaths, localVisited, 1);

            // 线程安全地合并路径
            if (!localPaths.isEmpty()) {
                QMutexLocker locker(&pathMutex);

                int remainingSlots = MAX_PATHS - pathCount;
                int pathsToAdd = qMin(remainingSlots, localPaths.size());

                if (pathsToAdd > 0) {
                    threadSafePaths.append(localPaths.mid(0, pathsToAdd));
                    pathCount += pathsToAdd;
                }
            }
        });
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    qint64 elapsedTime = timer.elapsed();
    qDebug() << "DFS total time:" << elapsedTime << "ms";
    return threadSafePaths;
}

void FlightNetwork::dfs(const QString& arrivalCity,
                        const QDate& selectedDate,
                        City* currentCity,
                        FlightRoute& path,
                        QVector<FlightRoute>& allPaths,
                        QSet<City*>& visited,
                        int depth) {
    if (depth > MAX_DEPTH || !currentCity || visited.contains(currentCity)) {
        return;
    }
    visited.insert(currentCity);

    double directPrice = getDirectFlightPrice(path.isEmpty() ? currentCity->name : path.first()->getDepartureCity(), arrivalCity);

    if (currentCity->name == arrivalCity && !path.isEmpty()) {
            allPaths.append(path);
            visited.remove(currentCity);
            return;
    }

    int lastArrivalTime = !path.isEmpty() ?
                              QDateTime::fromString(path.last()->getArrivalTime(), "yyyy-MM-dd HH:mm").toSecsSinceEpoch() :
                              QDateTime(selectedDate, QTime(0, 0)).toSecsSinceEpoch();

    for (FlightNode* node = currentCity->flightsHead; node; node = node->next) {
        QDateTime depDateTime = QDateTime::fromString(node->flight->getDepartureTime(), "yyyy-MM-dd HH:mm");
        bool isValidConnection = (depDateTime.toSecsSinceEpoch() > lastArrivalTime) &&
                                 (depDateTime.toSecsSinceEpoch() - lastArrivalTime <= 24 * 3600) &&
                                 (depDateTime.toSecsSinceEpoch() - lastArrivalTime >= 1 * 3600);

        if (!isValidConnection) {
            continue;
        }
        City* nextCity = cityMap.value(node->flight->getArrivalCity(), nullptr);
        if (!nextCity || visited.contains(nextCity)) {
            continue;
        }

        if (path.isEmpty()) {
            path.append(node->flight);
            dfs(arrivalCity, selectedDate, nextCity, path, allPaths, visited, depth + 1);
            path.removeLast();
            continue;
        }

        double currentPrice = node->flight->getPrice();
        double previousPrice = path.last()->getPrice();
        double priceSumSquare = currentPrice * currentPrice + previousPrice * previousPrice;

        if (directPrice <= 0) {
            path.append(node->flight);
            dfs(arrivalCity, selectedDate, nextCity, path, allPaths, visited, depth + 1);
            path.removeLast();
            continue;
        }
        // 剪枝
        if (priceSumSquare < directPrice * directPrice) {
            path.append(node->flight);
            dfs(arrivalCity, selectedDate, nextCity, path, allPaths, visited, depth + 1);
            path.removeLast();
        }
    }
    visited.remove(currentCity);
}

void FlightNetwork::precomputeDirectFlightPrices() {
    for (auto* city : cities) {
        for (FlightNode* node = city->flightsHead; node; node = node->next) {
            QString departureCity = city->name;
            QString arrivalCity = node->flight->getArrivalCity();
            double price = node->flight->getPrice();
            directFlightPrices[{departureCity, arrivalCity}] = price;
        }
    }
}

double FlightNetwork::getDirectFlightPrice(const QString& departureCity, const QString& arrivalCity) {
    return directFlightPrices.value(qMakePair(departureCity, arrivalCity), -1);
}

QVector<FlightRoute> FlightNetwork::sortFlights(User user, QVector<FlightRoute> flights, SORT_TYPE sortType) {
    if (flights.isEmpty()) {
        return flights;
    }

    switch (sortType) {
    case SORT_BY_PRICE:
        std::sort(flights.begin(), flights.end(),
                  [](const FlightRoute& a, const FlightRoute& b) {
                      return a.getTotalPrice() < b.getTotalPrice();
                  });
        break;
    case SORT_BY_DURA:
        std::sort(flights.begin(), flights.end(),
                  [](const FlightRoute& a, const FlightRoute& b) {
                      return Duration(a.first()->getDepartureTime(), a.last()->getArrivalTime()) <
                             Duration(b.first()->getDepartureTime(), b.last()->getArrivalTime());
                  });
        break;
    case SORT_BY_TIME:
        std::sort(flights.begin(), flights.end(),
                  [](const FlightRoute& a, const FlightRoute& b) {
                      return QDateTime::fromString(a.first()->getDepartureTime(), Qt::ISODate) <
                             QDateTime::fromString(b.first()->getDepartureTime(), Qt::ISODate);
                  });
        break;
    case SORT_BY_PERSON:
        try {
            // 计算基准价格
            double totalPrice = 0.0;
            for (const auto& route : flights) {
                totalPrice += route.getTotalPrice();
            }
            double basePrice = flights.isEmpty() ? 0 : (totalPrice / flights.size());

            std::sort(flights.begin(), flights.end(),
                      [&user, basePrice](const FlightRoute&a,const FlightRoute&b) {
                          // 价格得分
                          double priceRatio = user.getPriceRatio();
                          double aPriceScore = basePrice > 0 ?
                                                   (1.0 / (1.0 + std::pow(a.getTotalPrice() / basePrice - priceRatio, 2))) : 0;
                          double bPriceScore = basePrice > 0 ?
                                                   (1.0 / (1.0 + std::pow(b.getTotalPrice() / basePrice - priceRatio, 2))) : 0;

                          // 时间得分
                          int avgDepTime = user.getAvgDepTime();
                          QTime aDepTime = QDateTime::fromString(a.first()->getDepartureTime(), Qt::ISODate).time();
                          QTime bDepTime = QDateTime::fromString(b.first()->getDepartureTime(), Qt::ISODate).time();

                          double aTimeScore = 1.0 / (1.0 + std::pow(aDepTime.hour() - avgDepTime, 2));
                          double bTimeScore = 1.0 / (1.0 + std::pow(bDepTime.hour() - avgDepTime, 2));

                          // 转机次数得分
                          int avgTransNum = user.getAvgTransNum();
                          double aTransCountScore = 1.0 / (1.0 + std::pow(a.getTransferCount() - avgTransNum, 2));
                          double bTransCountScore = 1.0 / (1.0 + std::pow(b.getTransferCount() - avgTransNum, 2));

                          // 转机等待时间得分
                          int avgTransTime = user.getAvgTransTime();
                          double aTransTimeScore = 1.0 / (1.0 + std::pow(a.getTotalTransferTime() - avgTransTime, 2));
                          double bTransTimeScore = 1.0 / (1.0 + std::pow(b.getTotalTransferTime() - avgTransTime, 2));

                          // 机型得分
                          auto frequentAirTypes = user.getFrequentAirTypes();
                          double aTypeScore = 0, bTypeScore = 0;
                          for (const auto& flight : a) {
                              if (frequentAirTypes.contains(flight->getAircraftType())) {
                                  aTypeScore++;
                              }
                          }
                          for (const auto& flight : b) {
                              if (frequentAirTypes.contains(flight->getAircraftType())) {
                                  bTypeScore++;
                              }
                          }
                          aTypeScore /= a.getFlightCount();
                          bTypeScore /= b.getFlightCount();

                          // 综合得分
                          double aScore = 0.3 * aPriceScore + 0.2 * aTimeScore + 0.15 * aTransCountScore +
                                          0.15 * aTransTimeScore + 0.2 * aTypeScore;
                          double bScore = 0.3 * bPriceScore + 0.2 * bTimeScore + 0.15 * bTransCountScore +
                                          0.15 * bTransTimeScore + 0.2 * bTypeScore;

                          return aScore > bScore;
                      });
        } catch (const std::exception& e) {
            qDebug() << "Sorting error: " << e.what();
        }
        break;
    default:
        break;
    }
    return flights;
}

QVector<FlightRoute> FlightNetwork::searchRecommendation(const QVector<QString>& cityList, const User user) {
    QVector<FlightRoute> recommendations;
    cityMap.clear();

    QVector<QString> workingCityList = cityList;
    if (workingCityList.size() < 3) {
        // 按航班数降序排序的城市列表
        QVector<QPair<QString, int>> cityFlightCounts;
        for (const auto* city : cities) {
            if (!workingCityList.contains(city->name)) {
                cityFlightCounts.append({city->name, city->flightNum});
            }
        }

        // 按航班数降序排序
        std::sort(cityFlightCounts.begin(), cityFlightCounts.end(),
                  [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                      return a.second > b.second;
                  });

        // 选择航班数最多的城市
        while (workingCityList.size() <= 3 && !cityFlightCounts.isEmpty()) {
            workingCityList.append(cityFlightCounts.first().first);
            cityFlightCounts.removeFirst();
        }
    }

    for (City* city : cities) {
        cityMap[city->name] = city;
    }

    QVector<FlightRoute> threadSafeRecommendations;
    std::atomic<int> recommendationCount{0};
    const int MAX_THREADS = 10;

    QMutex recommendationMutex;

    std::vector<std::thread> threads;

    for (int i = 0; i < workingCityList.size(); i++) {
        for (int j = 0; j < workingCityList.size(); j++) {
            if (i == j || threads.size() >= MAX_THREADS || recommendationCount >= MAX_RECOMMENDATIONS) continue;

            threads.emplace_back([&, i, j]() {
                QString departureCityName = workingCityList[i];
                QString arrivalCityName = workingCityList[j];

                City* departureCity = cityMap.value(departureCityName, nullptr);
                City* arrivalCity = cityMap.value(arrivalCityName, nullptr);

                if (departureCity && arrivalCity) {
                    FlightRoute recommendedRoute = findSingleRoute(departureCity->name, arrivalCity->name, 0);

                    if (!recommendedRoute.isEmpty()) {
                        QMutexLocker locker(&recommendationMutex);

                        if (recommendationCount < MAX_RECOMMENDATIONS) {
                            threadSafeRecommendations.append(recommendedRoute);
                            recommendationCount ++;
                            //qDebug() << "rec:" << recommendedRoute.showFlightsInfo();
                        }
                    }
                }
            });
        }
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return threadSafeRecommendations;
}

FlightRoute FlightNetwork::findSingleRoute(const QString& departureCity, const QString& arrivalCity, const int depth) {

    City* startCity = cityMap.value(departureCity, nullptr);

    FlightRoute path;
    QSet<City*> visited;

    bool found = dfs2(arrivalCity, startCity, path, visited, depth);
    return found ? path : FlightRoute();
}

bool FlightNetwork::dfs2(const QString& arrivalCity,
                        City* currentCity,
                        FlightRoute& path,
                        QSet<City*>& visited,
                        int depth) {
    if (depth > MAX_DEPTH || !currentCity || visited.contains(currentCity)) {
        return false;
    }
    visited.insert(currentCity);

    if (currentCity->name == arrivalCity && !path.isEmpty()) {
        return true;
    }

    int lastArrivalTime = !path.isEmpty()
                              ? QDateTime::fromString(path.last()->getArrivalTime(), "yyyy-MM-dd HH:mm").toSecsSinceEpoch()
                              : 0;
    for (FlightNode* node = currentCity->flightsHead; node; node = node->next) {
        if (!node->flight) {
            continue;
        }

        int depDateTime = QDateTime::fromString(node->flight->getDepartureTime(), "yyyy-MM-dd HH:mm").toSecsSinceEpoch();

        bool isValidConnection = (lastArrivalTime == 0 ||
                                  (depDateTime > lastArrivalTime &&
                                   depDateTime - lastArrivalTime >= 1 * 3600 &&
                                   depDateTime - lastArrivalTime <= 24 * 3600));
        if (!isValidConnection) {
            continue;
        }

        City* nextCity = cityMap.value(node->flight->getArrivalCity(), nullptr);
        if (!nextCity || visited.contains(nextCity)) {
            continue;
        }

        path.append(node->flight);

        if (dfs2(arrivalCity, nextCity, path, visited, depth + 1)) {
            return true;
        }
        path.removeLast();
    }
    visited.remove(currentCity);
    return false;
}



