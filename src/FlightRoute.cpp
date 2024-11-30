#include "FlightRoute.h"
#include "Flight.h"
#include "Utils.h"

FlightRoute::FlightRoute() {}



FlightRoute::FlightRoute(const FlightRoute& other) {
    *this = other; // 使用拷贝赋值操作符
}

// FlightRoute::FlightRoute(const QVector<Flight>& initFlights) {
//     foreach (const Flight& flight, initFlights) {
//         flights.append(new Flight(flight));
//     }
// }

void FlightRoute::append(std::shared_ptr<Flight> flight) {
    flights.push_back(flight);
}

const QVector<std::shared_ptr<Flight>>& FlightRoute::getFlights() const {
    return flights;
}

double FlightRoute::getTotalPrice() const {
    double total = 0.0;
    for (const std::shared_ptr<Flight> &flight : flights) {
        total += flight->getPrice();
    }
    return total;
}

int FlightRoute::getTotalFlightTime() const {
    int totalDuration = 0;
    for (const std::shared_ptr<Flight> &flight : flights) {
        totalDuration += flight->getFlightTime();
    }
    return totalDuration;
}

QString FlightRoute::getDepartureTime() const {
    if (!flights.isEmpty()) {
        return flights.first()->getDepartureTime();
    }
    return QString();
}

QString FlightRoute::getArrivalTime() const {
    if (!flights.isEmpty()) {
        return flights.last()->getArrivalTime();
    }
    return QString();
}

int FlightRoute::getTotalDuration() const {
    return Duration(this->getDepartureTime(), this->getArrivalTime());
}

int FlightRoute::getFlightCount() const {
    return flights.size();
}

int FlightRoute::getTransferCount() const {
    return flights.size() - 1;
}

bool FlightRoute::isEmpty() const {
    return flights.isEmpty();
}

void FlightRoute::removeLast() {
    if (!flights.isEmpty()) {
        flights.removeLast();
    }
}

bool FlightRoute::isDomestic() const{
    for (std::shared_ptr<Flight> flight : flights) {
        if (foreignCities.contains(flight->getDepartureCity()) ||
            foreignCities.contains(flight->getArrivalCity())) {
            return false;
        }
    }
    return true;
}

QString FlightRoute::showCityPath() const {
    QString path;
    if (flights.isEmpty()) {
        return path;
    }
    path += flights.first()->getDepartureCity();
    for (int i = 0; i < flights.size() - 1; ++i) {
        path += " -> ";
        path += flights[i]->getArrivalCity();
    }
    path += " -> ";
    path += flights.last()->getArrivalCity();
    return path;
}

int FlightRoute::getTotalTransferTime() const {
    int totalTransferTime = 0;

    for (int i = 1; i < flights.size(); ++i) {
        const std::shared_ptr<Flight> prevFlight = flights[i - 1];
        const std::shared_ptr<Flight> currentFlight = flights[i];
        QDateTime prevArrivalTime = QDateTime::fromString(prevFlight->getArrivalTime(), "yyyy-MM-dd HH:mm");
        QDateTime currentDepartureTime = QDateTime::fromString(currentFlight->getDepartureTime(), "yyyy-MM-dd HH:mm");

        if (prevArrivalTime.isValid() && currentDepartureTime.isValid()) {
            qint64 transferTime = prevArrivalTime.secsTo(currentDepartureTime);
            if (transferTime > 0) {
                totalTransferTime += transferTime;
            }
        }
    }
    return totalTransferTime;
}

QString FlightRoute::showFlightsInfo() const {
    QString flightInfo = "";
    for(std::shared_ptr<Flight> flight : flights) {
        flightInfo += flight->showInfo() + "\n";
    }
    return flightInfo;
}

std::shared_ptr<Flight> FlightRoute::first() const {
    if (flights.isEmpty()) {
        return nullptr;
    }
    return flights.first();
}

std::shared_ptr<Flight> FlightRoute::last() const {
    if (flights.isEmpty()) {
        return nullptr;
    }
    return flights.last();
}

QVector<std::shared_ptr<Flight>>::iterator FlightRoute::begin() {
    return flights.begin();
}

QVector<std::shared_ptr<Flight>>::iterator FlightRoute::end() {
    return flights.end();
}

const QVector<std::shared_ptr<Flight>>::const_iterator FlightRoute::begin() const {
    return flights.begin();
}

const QVector<std::shared_ptr<Flight>>::const_iterator FlightRoute::end() const {
    return flights.end();
}

FlightRoute& FlightRoute::operator=(const FlightRoute& other) {
    if (this != &other) { // 自我赋值检查
        //qDeleteAll(flights); // 释放旧资源
        flights.clear();
        for (const std::shared_ptr<Flight> &flight : other.flights) {
            flights.push_back(flight); // 深拷贝
        }
    }
    return *this;
}

std::shared_ptr<Flight> FlightRoute::operator[](int index) const {
    if (index < 0 || index >= flights.size()) {
        return nullptr;
    }
    return flights[index];
}

bool FlightRoute::operator==(const FlightRoute& other) const {
    if (flights.size() != other.flights.size()) {
        return false;
    }
    for (int i = 0; i < flights.size(); ++i) {
        if (*flights[i] != *other.flights[i]) {
            return false;
        }
    }
    return true;
}



