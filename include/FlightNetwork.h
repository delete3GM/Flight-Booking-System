#pragma once

#include <QObject>
#include <QVector>
#include <Qstring>
#include <QDebug>
#include <QTextStream>
#include "Flight.h"
#include "Common.h"
#include "City.h"

class FlightNetwork : public QObject
{
    Q_OBJECT
public:
    explicit FlightNetwork(QObject* parent = nullptr);
    ~FlightNetwork();

    void addCity(QString cityName);
    void addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                   QString arrivalTime, double price, int remainSeat);
    QVector<QString> getAllCityNames() const;
    void readData(QTextStream* stream);
    QVector<Flight> searchFlights(QString departureCity, QString arrivalCity, QDate selectedDate);
    QVector<Flight> sortFlights(QVector<Flight> flights, SORT_TYPE sortType);

private:
    QVector<City*> cities;  // 存储城市对象的向量
};

