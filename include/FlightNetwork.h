#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include "Flight.h"
#include "Utils.h"
#include "City.h"

class FlightNetwork : public QObject
{
    Q_OBJECT

private:
    QVector<City*> cities;

public:
    explicit FlightNetwork(QObject* parent = nullptr);
    ~FlightNetwork();
    void clearData();

    QVector<City*> getCities()const;
    QVector<QString> getAllCityNames() const;
    void addCity(QString cityName);
    void addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                   QString arrivalTime, double price, int remainSeat);
    void readFlightFromFile(const QString& file);
    void writeFlightToFile(const QString& filename);
    QVector<Flight> searchFlights(QString departureCity, QString arrivalCity, QDate selectedDate);
    QVector<Flight> sortFlights(QVector<Flight> flights, SORT_TYPE sortType);
    QVector<QPair<Flight, Flight>> sortFlights(QVector<QPair<Flight, Flight>> flights, SORT_TYPE sortType);
    QVector<QPair<Flight, Flight>> findTransferFlight(const QString& departureCity, const QString& arrivalCity, QDate selectedDate);

};

