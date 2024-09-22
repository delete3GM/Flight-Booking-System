#include "FlightNetwork.h"
#include <QDebug>
#include <QDateTime>



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

void FlightNetwork::readData(QTextStream* stream)
{

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
