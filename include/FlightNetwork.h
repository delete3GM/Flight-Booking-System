#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QTextStream>
#include "FlightRoute.h"
#include "User.h"
#include "Utils.h"
#include "City.h"

class FlightNetwork : public QObject {
    Q_OBJECT

private:
    QVector<City*> cities;
    QHash<QString, City*> cityMap;
    QMap<QPair<QString, QString>, double> directFlightPrices;

public:
    explicit FlightNetwork(QObject* parent = nullptr);
    void clearData();

    QVector<City*> getCities()const;
    QVector<QString> getAllCityNames() const;
    void addCity(QString cityName);
    void addFlight(QString airline, QString flightNumber, QString aircratType, QString departureCity, QString arrivalCity,
                   QString departureTime, QString arrivalTime, double price, int remainSeat);
    QVector<FlightRoute> sortFlights(User user, QVector<FlightRoute> flights, SORT_TYPE sortType);

    QVector<FlightRoute> searchFlightsDFS(const QString& departureCity, const QString& arrivalCity, const QDate& selectedDate);
    void dfs(const QString& arrivalCity, const QDate& selectedDate, City* currentCity,
             FlightRoute& path, QVector<FlightRoute>& allPaths, QSet<City*>& visited,int depth);
    double getDirectFlightPrice(const QString& departureCity, const QString& arrivalCity);
    void precomputeDirectFlightPrices();

    QVector<FlightRoute> searchRecommendation(const QVector<QString>& cityList, const User user);
    FlightRoute findSingleRoute(const QString& departureCity, const QString& arrivalCity, const int depth);
    bool dfs2(const QString& arrivalCity, City* currentCity, FlightRoute& path, QSet<City*>& visited, int depth);

    void readFlightFromFile(const QString& file);
    void writeFlightToFile(const QString& filename);
};

