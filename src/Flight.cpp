#include "Flight.h"
#include "Common.h"
#include <QTextStream>
#include <QDateTime>

Flight::Flight(){}

QString Flight::getAirline() const { return airline; }
void Flight::setAirline(const QString &a) { airline = a; }

QString Flight::getFlightNumber() const { return flightNumber; }
void Flight::setFlightNumber(const QString &f) { flightNumber = f; }

QString Flight::getDepartureCity() const { return departureCity; }
void Flight::setDepartureCity(const QString &d) { departureCity = d; }

QString Flight::getDepartureTime() const { return departureTime; }
void Flight::setDepartureTime(const QString &depTime) { departureTime = depTime; }

QString Flight::getArrivalCity() const { return arrivalCity; }
void Flight::setArrivalCity(const QString &aCity) { arrivalCity = aCity; }

QString Flight::getArrivalTime() const { return arrivalTime; }
void Flight::setArrivalTime(const QString &arrTime) { arrivalTime = arrTime; }

double Flight::getPrice() const { return price; }
void Flight::setPrice(double p) { price = p; }

int Flight::getRemainSeatNum() const { return remainSeatNum; }
void Flight::setRemainSeatNum(int remainSeat) { remainSeatNum = remainSeat; }

int Flight::getFlightTime() const { return Duration(departureTime, arrivalTime); }

QString Flight::toString() const
{
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

QString Flight::showInfo()
{
    return QString("%1\n%2\n%3\n%4\n%5\n%6\n\n待支付￥%7")
        .arg(airline)
        .arg(flightNumber)
        .arg(departureCity)
        .arg(departureTime)
        .arg(arrivalCity)
        .arg(arrivalTime)
        .arg(QString::number(price));

}


bool Flight::operator == (const Flight& other) const
{
  return airline == other.airline && flightNumber == other.flightNumber &&
         departureCity == other.departureCity &&
         departureTime == other.departureTime &&
         arrivalCity == other.arrivalCity && arrivalTime == other.arrivalTime;
}
