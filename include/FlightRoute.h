#pragma once

#include <QVector>
#include "Flight.h"

class FlightRoute {
private:
    QVector<std::shared_ptr<Flight>> flights;
public:
    FlightRoute();
    FlightRoute(const FlightRoute& other);
    //FlightRoute(const QVector<Flight>& flights);
    FlightRoute(const QVector<Flight>& initFlights) {
        for (const Flight& flight : initFlights) {
            flights.push_back(std::make_shared<Flight>(flight));
        }
    }


    const QVector<std::shared_ptr<Flight>>& getFlights() const;
    double getTotalPrice() const;
    int getTotalFlightTime() const;
    int getTotalDuration() const;
    int getTotalTransferTime() const;
    QString getDepartureTime() const;
    QString getArrivalTime() const;
    int getFlightCount() const;
    int getTransferCount() const;

    void append(std::shared_ptr<Flight> flight);
    bool isEmpty() const;
    void removeLast();
    bool isDomestic() const;

    QString showCityPath() const;
    QString showFlightsInfo() const;

    std::shared_ptr<Flight> first() const;
    std::shared_ptr<Flight> last() const;

    QVector<std::shared_ptr<Flight>>::iterator begin();
    QVector<std::shared_ptr<Flight>>::iterator end();
    const QVector<std::shared_ptr<Flight>>::const_iterator begin() const;
    const QVector<std::shared_ptr<Flight>>::const_iterator end() const;

    std::shared_ptr<Flight> operator[](int index) const;
    bool operator==(const FlightRoute& other) const;
    FlightRoute& operator=(const FlightRoute& other);
};

