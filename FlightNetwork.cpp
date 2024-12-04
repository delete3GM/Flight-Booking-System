#include "FlightNetwork.h"
#include "FlightRoute.h"
#include "User.h"
#include "Utils.h"
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QElapsedTimer>
#include <random>
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

    // 使用标准线程池
    QVector<FlightRoute> threadSafePaths;
    std::atomic<int> pathCount{0};
    const int MAX_PATHS = 10000;  // 限制总路径数
    const int MAX_THREADS = std::thread::hardware_concurrency();

    // 互斥锁
    QMutex pathMutex;

    // 线程容器
    std::vector<std::thread> threads;

    // 遍历初始航班
    for (FlightNode* initialNode = startCity->flightsHead;
         initialNode && pathCount < MAX_PATHS && threads.size() < MAX_THREADS;
         initialNode = initialNode->next) {

        threads.emplace_back([&, initialNode]() {
            // 每个线程独立的搜索上下文
            FlightRoute localPath;
            QSet<City*> localVisited;
            QVector<FlightRoute> localPaths;

            // 添加初始航班
            localPath.append(initialNode->flight);
            City* nextCity = cityMap.value(initialNode->flight->getArrivalCity(), nullptr);

            // 深度优先搜索
            dfs(arrivalCity, selectedDate, nextCity, localPath, localPaths, localVisited, 1);

            // 线程安全地合并路径
            if (!localPaths.isEmpty()) {
                QMutexLocker locker(&pathMutex);

                // 检查是否超过最大路径限制
                int remainingSlots = MAX_PATHS - pathCount;
                int pathsToAdd = qMin(remainingSlots, localPaths.size());

                if (pathsToAdd > 0) {
                    threadSafePaths.append(localPaths.mid(0, pathsToAdd));
                    pathCount += pathsToAdd;
                }
            }
        });
    }

    // 等待所有线程完成
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
        int depDateTime = QDateTime::fromString(node->flight->getDepartureTime(), "yyyy-MM-dd HH:mm").toSecsSinceEpoch();
        bool isValidConnection = (depDateTime > lastArrivalTime) &&
                                 (depDateTime - lastArrivalTime <= 24 * 3600) &&
                                 (depDateTime - lastArrivalTime >= 1 * 3600);
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
    auto calculateTotalDuration = [](const FlightRoute& flightPath) {
        return Duration(flightPath.first()->getDepartureTime(), flightPath.last()->getArrivalTime());
    };
    auto calculateEarliestDepartureTime = [](const FlightRoute& flightPath) {
        return QDateTime::fromString(flightPath.first()->getDepartureTime(), Qt::ISODate);
    };
    auto calculateScore = [ &calculateEarliestDepartureTime, &user, &flights]
        (const FlightRoute& flightPath) {

        // 计算价格得分，与价格偏好偏差越大 得分越少
        double totalPrice = 0.0;
        for (const auto& route : flights) {
            totalPrice += route.getTotalPrice();
        }
        double basePrice = totalPrice / flights.size();
        double preRatio = user.getPriceRatio();

        double curRatio = SMOOTHING_FACTOR * (flightPath.getTotalPrice() / basePrice) +
                              (1 - SMOOTHING_FACTOR) * preRatio;
        double priceScore = 1.0 / (1.0 + (curRatio - preRatio) * (curRatio - preRatio));

        // 计算时间得分，与平均起飞时间偏差越大 得分越少
        QDateTime earliestDepartureTime = calculateEarliestDepartureTime(flightPath);
        double timeScore = 1.0 / (1.0 + (earliestDepartureTime.time().hour() - user.getAvgDepTime()) *
                                            (earliestDepartureTime.time().hour() - user.getAvgDepTime()));

        // 1/(1+(x-a)^2) 与平均转机数偏差越大 得分越少
        int transferCount = flightPath.getTransferCount();
        double transferScore = 1.0 / (1.0 + (transferCount - user.getAvgTransNum()) * (transferCount - user.getAvgTransNum()));

        double score = 0.5 * priceScore + 0.2 * timeScore + 0.3 * transferScore;
        return score;
    };

    switch (sortType) {
    case SORT_BY_PRICE:
        std::sort(flights.begin(), flights.end(),
                  [](const FlightRoute& a, const FlightRoute& b) {
                      return a.getTotalPrice() < b.getTotalPrice();
                  });
        break;
    case SORT_BY_DURA:
        std::sort(flights.begin(), flights.end(),
                  [calculateTotalDuration](const FlightRoute& a, const FlightRoute& b) {
                      return calculateTotalDuration(a) < calculateTotalDuration(b);
                  });
        break;
    case SORT_BY_TIME:
        std::sort(flights.begin(), flights.end(),
                  [calculateEarliestDepartureTime](const FlightRoute& a, const FlightRoute& b) {
                      return calculateEarliestDepartureTime(a) < calculateEarliestDepartureTime(b);
                  });
        break;
    case SORT_BY_PERSON:
        std::sort(flights.begin(), flights.end(),
                  [calculateScore](const FlightRoute& a, const FlightRoute& b) {
                      return calculateScore(a) > calculateScore(b);
                  });
        break;
    default:
        break;
    }
    return flights;
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


QVector<FlightRoute> FlightNetwork::searchRecommendation(const QVector<QString>& cityList, const User user) {
    QVector<FlightRoute> recommendations;
    cityMap.clear();

    QVector<QString> workingCityList = cityList;
    if (workingCityList.size() < 3) {
        QVector<QString> availableCities;
        for (const auto* city : cities) {
            if (!workingCityList.contains(city->name)) {
                availableCities.append(city->name);
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        while (workingCityList.size() <= 3 && !availableCities.isEmpty()) {
            std::uniform_int_distribution<> dis(0, availableCities.size() - 1);
            int randomIndex = dis(gen);
            workingCityList.append(availableCities[randomIndex]);
            availableCities.removeAt(randomIndex);
        }
    }

    for (City* city : cities) {
        cityMap[city->name] = city;
    }

    for (int i = 0; i < workingCityList.size(); i++) {
        for (int j = 0; j < workingCityList.size(); j++) {
            if (i == j) continue;

            QString departureCityName = workingCityList[i];
            QString arrivalCityName = workingCityList[j];

            City* departureCity = cityMap.value(departureCityName, nullptr);
            City* arrivalCity = cityMap.value(arrivalCityName, nullptr);

            if (departureCity && arrivalCity) {
                FlightRoute recommendedRoute = findSingleRoute(departureCity->name, arrivalCity->name, int(user.getAvgTransNum()));

                if (!recommendedRoute.isEmpty()) {
                    recommendations.append(recommendedRoute);
                    qDebug()<<"rec:"<<recommendedRoute.showFlightsInfo();
                    if(recommendations.size() >= RECOM_NUM) return recommendations;
                }
            }
        }
    }
    return recommendations;
}
