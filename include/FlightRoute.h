#pragma once

#include <QVector>
#include "Flight.h"

class FlightRoute {
private:
    QVector<Flight*> flights;
public:
    FlightRoute();
    FlightRoute(const FlightRoute& other);
    FlightRoute(const QVector<Flight>& flights);
    ~FlightRoute();
    void append(Flight* flight);
    const QVector<Flight*>& getFlights() const;
    double getTotalPrice() const;
    int getTotalFlightTime() const;
    int getTotalDuration() const;
    QString getDepartureTime() const;
    QString getArrivalTime() const;
    int getFlightCount() const;
    int getTransferCount() const;
    Flight* first() const;
    Flight* last() const;
    bool isEmpty() const;
    void removeLast();

    QString showCityPath() const;
    QString showFlightsInfo() const;

    QVector<Flight*>::iterator begin();
    QVector<Flight*>::iterator end();
    const QVector<Flight*>::const_iterator begin() const;
    const QVector<Flight*>::const_iterator end() const;
    Flight* operator[](int index) const;
    bool operator==(const FlightRoute& other) const;
    FlightRoute& operator=(const FlightRoute& other);
};

