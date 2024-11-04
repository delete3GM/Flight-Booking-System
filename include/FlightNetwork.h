#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QTextStream>
#include "FlightRoute.h"
#include "Utils.h"
#include "City.h"

class FlightNetwork : public QObject {
    Q_OBJECT

private:
    QVector<City*> cities;
    QMap<QString, City*> cityMap;

public:
    explicit FlightNetwork(QObject* parent = nullptr);
    ~FlightNetwork();
    void clearData();

    QVector<City*> getCities()const;
    QVector<QString> getAllCityNames() const;
    void addCity(QString cityName);
    void addFlight(QString airline, QString flightNumber, QString departureCity, QString arrivalCity, QString departureTime,
                   QString arrivalTime, double price, int remainSeat);
    QVector<FlightRoute> sortFlights(QVector<FlightRoute> flights, SORT_TYPE sortType);

    QVector<FlightRoute> searchFlightsDFS(const QString& departureCity, const QString& arrivalCity, const QDate& selectedDate);
    void dfs(const QString& departureCity, const QString& arrivalCity, const QDate& selectedDate, City* currentCity,
             FlightRoute& path, QVector<FlightRoute>& allPaths, QSet<City*>& visited,int depth);

    void readFlightFromFile(const QString& file);
    void writeFlightToFile(const QString& filename);
};

