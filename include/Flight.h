#pragma once
#include <QString>
#include <QTextStream>

class Flight
{
private:
    QString airline;
    QString flightNumber;
    QString departureCity;
    QString departureTime;
    QString arrivalCity;
    QString arrivalTime;
    double price;
    int remainSeatNum;

public:
    Flight();
    Flight(QString a, QString f, QString dCity, QString depTime, QString aCity, QString arrTime, double p, int remainSeat)
        : airline(a), flightNumber(f), departureCity(dCity), departureTime(depTime), arrivalCity(aCity), arrivalTime(arrTime),
        price(p), remainSeatNum(remainSeat) {}

    QString getAirline() const;
    void setAirline(const QString &a);

    QString getFlightNumber() const;
    void setFlightNumber(const QString &f);

    QString getDepartureCity() const;
    void setDepartureCity(const QString &d);

    QString getDepartureTime() const;
    void setDepartureTime(const QString &depTime);

    QString getArrivalCity() const;
    void setArrivalCity(const QString &aCity);

    QString getArrivalTime() const;
    void setArrivalTime(const QString &arrTime);

    double getPrice() const;
    void setPrice(double p);

    int getRemainSeatNum() const;
    void setRemainSeatNum(int remainSeat);

    int getFlightTime() const;

    QString toString() const;

    QString showInfo();

    bool operator == (const Flight& other) const;


};

