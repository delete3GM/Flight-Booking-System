#include "FlightNetwork.h"
#include <QDebug>
#include <QDateTime>
#include <limits>
#include <queue>



FlightNetwork::FlightNetwork(QObject* parent) : QObject(parent) {}

FlightNetwork::~FlightNetwork()
{
    for (auto* city : cities) {
        delete city;
    }
    cities.clear();
}

void FlightNetwork::addCity(QString cityName)
{
    for (auto* city : cities) {
        if (city->name == cityName) return;
    }
    City* newCity = new City(cityName);
    cities.append(newCity);
    //qDebug() << "City added:" << cityName;
}

void FlightNetwork::addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                              QString arrivalTime, double price, int remainSeat)
{
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
        }
        //尾插法添加边结点（航线）
        else {
            FlightNode* temp = depCity->flights;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
}

QVector<City*> FlightNetwork::getCities() const
{
    return cities;
}


QVector<QString> FlightNetwork::getAllCityNames() const
{
    QVector<QString> cityNames;
    for (auto* city : cities) {
        cityNames.append(city->name);
    }
    return cityNames;
}

void FlightNetwork::readData(const QString& file)
{
    QTextStream* stream = LoadTextFile(file);
    if (!stream) {
        qInfo() << "Stream is null!";
        return;
    }
    //读取数据
    while(!stream->atEnd())
    {
        auto lineData = stream->readLine().split(", ", Qt::SkipEmptyParts);

        // 去除每个字段两端的空格
        for (QString& part : lineData) {
            part = part.trimmed();
        }
        //qInfo()<<lineData;

        // 非空字段的数量
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
            addFlight(lineData[0], lineData[1], lineData[2], lineData[3], lineData[4], lineData[5], lineData[6].toDouble(), lineData[7].toInt());
        }
    }
}

void FlightNetwork::writeDataToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing: " << filename;
        return;
    }
    QTextStream out(&file);

    for (City* city : cities) {
        FlightNode* node = city->flights;
        while (node) {
            out << node->flight->getAirline() << ", "
                << node->flight->getFlightNumber() << ", "
                << node->flight->getDepartureCity() << ", "
                << node->flight->getDepartureTime() << ", "
                << node->flight->getArrivalCity() << ", "
                << node->flight->getArrivalTime() << ", "
                << node->flight->getPrice() << ", "
                << node->flight->getRemainSeatNum() << "\n";
            node = node->next;
        }
    }
    file.close();
}

QVector<Flight> FlightNetwork::searchFlights(QString departureCity, QString arrivalCity, QDate selectedDate)
{
    QVector<Flight> result;
    City* depCity = nullptr;
    for (auto* city : cities) {
        if (city->name == departureCity) {
            depCity = city;
            break;
        }
    }
    if (depCity) {
        FlightNode* node = depCity->flights;
        while (node) {
            QDateTime depDateTime = QDateTime::fromString(node->flight->getDepartureTime(), "yyyy-MM-dd HH:mm");
            if (node->flight->getArrivalCity() == arrivalCity && depDateTime.date() == selectedDate) {
                result.append(*node->flight);
            }
            node = node->next;
        }
    }
    return result;
}

QVector<Flight> FlightNetwork::sortFlights(QVector<Flight> flights, SORT_TYPE sortType)
{
    if (flights.isEmpty()){
        return flights;
    }

    switch (sortType) {
    case SORT_BY_PRICE:
        std::sort(flights.begin(), flights.end(), [](const Flight &a, const Flight &b) {
            return a.getPrice() < b.getPrice();
        });
        break;
    case SORT_BY_DURA:
        std::sort(flights.begin(), flights.end(), [](const Flight &a, const Flight &b) {
            return a.getFlightTime() < b.getFlightTime();
        });
        break;
    case SORT_BY_TIME:
        std::sort(flights.begin(), flights.end(), [](const Flight &a, const Flight &b) {
            return a.getDepartureTime() < b.getDepartureTime();
        });
        break;
    case SORT_BY_SEAT:
        std::sort(flights.begin(), flights.end(), [](const Flight &a, const Flight &b) {
            return a.getRemainSeatNum() < b.getRemainSeatNum();
        });
        break;
    default:
        break;
    }
    return flights;
}

// Dijkstra 需要该！
QVector<Flight> FlightNetwork::findShortestPath(const QString& departureCity, const QString& arrivalCity, QDate selectedDate, int& totalDuration)
{
    QVector<City*> citiesCopy = getCities();
    QVector<Flight> result;
    totalDuration = std::numeric_limits<int>::max();

    // 创建一个布尔数组来记录访问状态
    std::vector<bool> visited(citiesCopy.size(), false);
    // 创建一个距离表来记录源点到每个顶点的最短距离
    std::vector<int> distances(citiesCopy.size(), std::numeric_limits<int>::max());
    // 创建一个前驱表来重建最短路径
    std::vector<City*> predecessors(citiesCopy.size(), nullptr);

    // 创建一个映射来存储城市和它们索引的映射
    std::map<QString, int> cityIndexMap;
    for (int i = 0; i < citiesCopy.size(); ++i) {
        cityIndexMap[citiesCopy[i]->name] = i;
    }

    City* departureCityNode = nullptr;
    City* arrivalCityNode = nullptr;
    for (int i = 0; i < citiesCopy.size(); ++i) {
        if (citiesCopy[i]->name == departureCity) {
            departureCityNode = citiesCopy[i];
            distances[i] = 0; // 源点到自己的距离是0
            predecessors[i] = nullptr;
        }
        if (citiesCopy[i]->name == arrivalCity) {
            arrivalCityNode = citiesCopy[i];
        }
    }

    if (!departureCityNode || !arrivalCityNode) return result; // 出发城市或到达城市不存在

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push(std::make_pair(0, cityIndexMap[departureCity])); // 将源点加入优先队列，距离为0

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentIndex = pq.top().second;
        pq.pop();

        City* currentCity = citiesCopy[currentIndex];

        if (visited[currentIndex]) continue;
        visited[currentIndex] = true;

        for (FlightNode* node = currentCity->flights; node != nullptr; node = node->next) {
            int nextIndex = -1;
            for (int j = 0; j < citiesCopy.size(); ++j) {
                if (citiesCopy[j]->name == node->flight->getArrivalCity()) {
                    nextIndex = j;
                    break;
                }
            }

            if (nextIndex != -1) {
                int distanceThroughU = currentDistance + node->flight->getFlightTime();
                if (distanceThroughU < distances[nextIndex]) {
                    distances[nextIndex] = distanceThroughU;
                    predecessors[nextIndex] = currentCity;
                    pq.push(std::make_pair(distanceThroughU, nextIndex));
                }
            }
        }
    }

    // 重建从出发城市到到达城市的最短路径
    int arrivalIndex = cityIndexMap[arrivalCity];
    if (distances[arrivalIndex] != std::numeric_limits<int>::max()) {
        totalDuration = distances[arrivalIndex];
        City* currentCity = predecessors[arrivalIndex];
        while (currentCity != nullptr) {
            for (FlightNode* node = currentCity->flights; node != nullptr; node = node->next) {
                if (node->flight->getArrivalCity() == arrivalCity) {
                    result.prepend(*node->flight);
                    currentCity = predecessors[cityIndexMap[node->flight->getDepartureCity()]];
                    break;
                }
            }
            if (currentCity == departureCityNode) break; // 到达源点，路径构建完成
        }
    }

    return result;
}

QVector<QPair<Flight, Flight>>
FlightNetwork::findTransferFlight(const QString &departureCity,
                                  const QString &arrivalCity,
                                  QDate selectedDate) {
  // 我们从出发城市的航班中，依次查找到达城市的航班，查看是否有目的地城市的航班，加入到结果列表中
  QVector<QPair<Flight, Flight>> result;
  // 获取当前城市
  City *departureCityNode = nullptr;
  City *arrivalCityNode = nullptr;
  for (auto *city : cities) {
    if (city->name == departureCity) {
      departureCityNode = city;
    }
    if (city->name == arrivalCity) {
      arrivalCityNode = city;
    }
  }

  // 从 departureCityNode 的航班中查找到达城市的航班
  FlightNode *node = departureCityNode->flights;
  while (node) {
    QDateTime depDateTime = QDateTime::fromString(
        node->flight->getDepartureTime(), "yyyy-MM-dd HH:mm");
    // 获取当前航班的到达城市
    QString currentArrivalCity = node->flight->getArrivalCity();
    // 获取当前航班的到达时间
    QDateTime currentArrivalTime = QDateTime::fromString(
        node->flight->getArrivalTime(), "yyyy-MM-dd HH:mm");

    City *currentArrivalCityNode = nullptr;
    for (auto *city : cities) {
      if (city->name == currentArrivalCity) {
        currentArrivalCityNode = city;
        break;
      }
    }

    // 查看当前到达城市是否有飞往目的地的航班，并且起飞时间比落地时间晚
    if (currentArrivalCityNode && currentArrivalTime.date() == selectedDate) {
      FlightNode *transferNode = currentArrivalCityNode->flights;
      while (transferNode) {
        QDateTime transferDepTime = QDateTime::fromString(
            transferNode->flight->getDepartureTime(), "yyyy-MM-dd HH:mm");
        if (transferNode->flight->getArrivalCity() == arrivalCity &&
            transferDepTime > currentArrivalTime && transferDepTime.date() == selectedDate) {
          QPair<Flight, Flight> pair;
          pair.first = *node->flight;
          pair.second = *transferNode->flight;
          result.append(pair);
        }
        transferNode = transferNode->next;
      }
    }
    node = node->next;
  }

  return result;
}

QVector<QPair<Flight, Flight>> FlightNetwork::sortFlights(QVector<QPair<Flight, Flight>> flights, SORT_TYPE sortType){
    if (flights.isEmpty()) {
        return flights;
    }
    
    switch (sortType) {
    case SORT_BY_PRICE:
        std::sort(flights.begin(), flights.end(),
                [](const QPair<Flight, Flight> &a, const QPair<Flight, Flight> &b) {
                    return a.first.getPrice() + a.second.getPrice() < b.first.getPrice() + b.second.getPrice();
                });
        break;
    case SORT_BY_DURA:
        std::sort(flights.begin(), flights.end(),
                [](const QPair<Flight, Flight> &a, const QPair<Flight, Flight> &b) {
                    return a.first.getFlightTime() + a.second.getFlightTime() < b.first.getFlightTime() + b.second.getFlightTime();
                });
        break;
    case SORT_BY_TIME:
        std::sort(flights.begin(), flights.end(),
                [](const QPair<Flight, Flight> &a, const QPair<Flight, Flight> &b) {
                    return a.first.getDepartureTime() < b.first.getDepartureTime();
                });
        break;
    case SORT_BY_SEAT:
        std::sort(flights.begin(), flights.end(),
                [](const QPair<Flight, Flight> &a, const QPair<Flight, Flight> &b) {
                    return a.first.getRemainSeatNum() + a.second.getRemainSeatNum() < b.first.getRemainSeatNum() + b.second.getRemainSeatNum();
                });
        break;
    default:
        break;
    }
    return flights;
}
