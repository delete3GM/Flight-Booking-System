#include "FlightNetwork.h"
#include "FlightRoute.h"
#include "Utils.h"
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QElapsedTimer>

FlightNetwork::FlightNetwork(QObject* parent) : QObject(parent) {}

FlightNetwork::~FlightNetwork() {
    for (auto* city : cities) {
        delete city;
    }
    cities.clear();
}

void FlightNetwork::addCity(QString cityName) {
    for (auto* city : cities) {
        if (city->name == cityName) return;
    }
    City* newCity = new City(cityName);
    cities.append(newCity);
}

void FlightNetwork::addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                              QString arrivalTime, double price, int remainSeat) {
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
    Flight* newFlight = new Flight(airline, flightNumber, departureCity, arrivalCity, departureTime, arrivalTime, price, remainSeat);
    FlightNode* newNode = new FlightNode(newFlight);
    if(depCity){
        if (!depCity->flights) {
            depCity->flights = newNode;
        } else {
            FlightNode* temp = depCity->flights;
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
        auto lineData = stream->readLine().split(", ", Qt::SkipEmptyParts);

        // 去除每个字段两端的空格
        for (QString& part : lineData) {
            part = part.trimmed();
        }

        int nonEmptyCount = std::count_if(lineData.begin(), lineData.end(), [](const QString &str) {
            return !str.isEmpty();
        });
        if (nonEmptyCount == 8) {
            if (!lineData[2].isEmpty()) {
                addCity(lineData[2]);
            }
            if (!lineData[4].isEmpty()) {
                addCity(lineData[4]);
            }
            addFlight(lineData[0], lineData[1], lineData[2], lineData[3], lineData[4],
                      lineData[5], lineData[6].toDouble(), lineData[7].toInt());
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
        FlightNode* node = city->flights;
        while (node) {
            out << QString("%1, %2, %3, %4, %5, %6, %7, %8\n")
            .arg(node->flight->getAirline())
                .arg(node->flight->getFlightNumber())
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
    QVector<FlightRoute> allPaths;
    cityMap.clear();
    for (auto* city : cities) {
        cityMap[city->name] = city;
    }
    City* startCity = cityMap.value(departureCity, nullptr);
    if (!startCity) {
        return allPaths;
    }
    FlightRoute path;
    QSet<City*> visited;

    QElapsedTimer timer;
    timer.start();
    dfs(departureCity, arrivalCity, selectedDate, startCity, path, allPaths, visited, 0);
    qint64 elapsedTime = timer.elapsed();
    qDebug() << "DFS total time:" << elapsedTime << "ms";

    return allPaths;
}

void FlightNetwork::dfs(const QString& departureCity,
                        const QString& arrivalCity,
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

    if (currentCity->name == arrivalCity && !path.isEmpty()) {
        allPaths.append(path);
        visited.remove(currentCity);
        return;
    }
   int lastArrivalTime = !path.isEmpty() ?
                              QDateTime::fromString(path.last()->getArrivalTime(), "yyyy-MM-dd HH:mm").toSecsSinceEpoch() :
                              QDateTime(selectedDate, QTime(0, 0)).toSecsSinceEpoch();

    for (FlightNode* node = currentCity->flights; node; node = node->next) {
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
        path.append(node->flight);
        dfs(departureCity, arrivalCity, selectedDate, nextCity, path, allPaths, visited, depth + 1);
        path.removeLast();
    }
    visited.remove(currentCity);
}

QVector<FlightRoute> FlightNetwork::sortFlights(QVector<FlightRoute> flights, SORT_TYPE sortType) {
    if (flights.isEmpty()) {
        return flights;
    }
    auto calculateTotalPrice = [](const FlightRoute& flightPath) {
        return std::accumulate(flightPath.begin(), flightPath.end(), 0.0,
                               [](double sum, Flight* flight) { return sum + flight->getPrice(); });
    };
    auto calculateTotalDuration = [](const FlightRoute& flightPath) {
        return Duration(flightPath.first()->getDepartureTime(), flightPath.last()->getArrivalTime());
    };
    auto calculateEarliestDepartureTime = [](const FlightRoute& flightPath) {
        return QDateTime::fromString(flightPath.first()->getDepartureTime(), Qt::ISODate);
    };
    auto calculateTotalSeats = [](const FlightRoute& flightPath) {
        return std::accumulate(flightPath.begin(), flightPath.end(), 0,
                               [](int sum, Flight* flight) { return sum + flight->getRemainSeatNum(); });
    };

    switch (sortType) {
    case SORT_BY_PRICE:
        std::sort(flights.begin(), flights.end(),
                  [calculateTotalPrice](const FlightRoute& a, const FlightRoute& b) {
                      return calculateTotalPrice(a) < calculateTotalPrice(b);
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
    case SORT_BY_SEAT:
        std::sort(flights.begin(), flights.end(),
                  [calculateTotalSeats](const FlightRoute& a, const FlightRoute& b) {
                      return calculateTotalSeats(a) < calculateTotalSeats(b);
                  });
        break;
    default:
        break;
    }
    return flights;
}


