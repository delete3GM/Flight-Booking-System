#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "Flight.h"
#include "Common.h"
#include "City.h"

class FlightNetwork : public QObject
{
    Q_OBJECT

private:
    QVector<City*> cities;  // 存储城市对象的向量

public:
    explicit FlightNetwork(QObject* parent = nullptr);
    ~FlightNetwork();

    QVector<City*> getCities()const;
    QVector<QString> getAllCityNames() const;
    void addCity(QString cityName);
    void addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                   QString arrivalTime, double price, int remainSeat);
    void readData(const QString& file);
    void writeDataToFile(const QString& filename);
    QVector<Flight> searchFlights(QString departureCity, QString arrivalCity, QDate selectedDate);
    QVector<Flight> sortFlights(QVector<Flight> flights, SORT_TYPE sortType);
    QVector<Flight> findShortestPath(const QString& departureCity, const QString& arrivalCity, QDate selectedDate, int& totalDuration);

};

