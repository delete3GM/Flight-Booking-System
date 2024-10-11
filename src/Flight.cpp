#include "Flight.h"
#include "Utils.h"
#include <QTextStream>
#include <QDateTime>

Flight::Flight(){}



QString Flight::getAirline() const {
    return airline;
}

QString Flight::getFlightNumber() const {
    return flightNumber;
}

QString Flight::getDepartureCity() const {
    return departureCity;
}

QString Flight::getDepartureTime() const {
    return departureTime;
}

QString Flight::getArrivalCity() const {
    return arrivalCity;
}

QString Flight::getArrivalTime() const {
    return arrivalTime;
}

double Flight::getPrice() const {
    return price;
}

int Flight::getRemainSeatNum() const {
    return remainSeatNum;
}

int Flight::getFlightTime() const {
    return Duration(departureTime, arrivalTime);
}

void Flight::setAirline(const QString &a) {
    airline = a;
}

void Flight::setFlightNumber(const QString &f) {
    flightNumber = f;
}

void Flight::setDepartureCity(const QString &d) {
    departureCity = d;
}

void Flight::setDepartureTime(const QString &depTime) {
    departureTime = depTime;
}

void Flight::setArrivalCity(const QString &aCity) {
    arrivalCity = aCity;
}

void Flight::setArrivalTime(const QString &arrTime) {
    arrivalTime = arrTime;
}

void Flight::setPrice(double p) {
    price = p;
}

void Flight::setRemainSeatNum(int remainSeat) {
    remainSeatNum = remainSeat;
}

QString Flight::toString() const {
    return QString("%1, %2, %3, %4, %5, %6, %7, %8")
    .arg(airline)
        .arg(flightNumber)
        .arg(departureCity)
        .arg(departureTime)
        .arg(arrivalCity)
        .arg(arrivalTime)
        .arg(QString::number(price))  // 将 double 转换为 QString
        .arg(QString::number(remainSeatNum));
}

QString Flight::showInfo() {
    if (price == 0) {
        return QString("");
    }
    return QString("%1 %2\n%3->%5\n%4->%6\n")
        .arg(airline)
        .arg(flightNumber)
        .arg(departureCity)
        .arg(departureTime)
        .arg(arrivalCity)
        .arg(arrivalTime);
}

bool Flight::operator == (const Flight& other) const {
  return airline == other.airline && flightNumber == other.flightNumber &&
         departureCity == other.departureCity &&
         departureTime == other.departureTime &&
         arrivalCity == other.arrivalCity && arrivalTime == other.arrivalTime;
}
