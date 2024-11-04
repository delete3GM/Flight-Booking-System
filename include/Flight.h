#pragma once
#include <QString>
#include <QTextStream>

class Flight {
private:
    QString airline;
    QString flightNumber;
    QString departureCity;
    QString departureTime;
    QString arrivalCity;
    QString arrivalTime;
    double price = 0;
    int remainSeatNum = 0;

public:
    Flight();
    Flight(QString a, QString f, QString dCity, QString depTime, QString aCity, QString arrTime, double p, int remainSeat);

    QString getAirline() const;
    QString getFlightNumber() const;
    QString getDepartureCity() const;
    QString getDepartureTime() const;
    QString getArrivalCity() const;
    QString getArrivalTime() const;
    double getPrice() const;
    int getRemainSeatNum() const;
    int getFlightTime() const;

    void setAirline(const QString &a);
    void setFlightNumber(const QString &f);
    void setDepartureCity(const QString &d);
    void setDepartureTime(const QString &depTime);
    void setArrivalCity(const QString &aCity);
    void setArrivalTime(const QString &arrTime);
    void setPrice(double p);
    void setRemainSeatNum(int remainSeat);

    QString toString() const;
    QString showInfo();

    bool operator == (const Flight& other) const;
    bool operator != (const Flight& other) const;


};

