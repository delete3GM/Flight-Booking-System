#pragma once

#include <QString>
#include "FlightRoute.h"
#include "Passenger.h"
#include "Flight.h"

class Order {

private:
    QString orderId;
    Passenger passenger;
    FlightRoute flightroute;
    QString status;


public:

    Order();
    Order(QString id, const Passenger& passenger, FlightRoute flightroute, QString status);
    QString getOrderId() const;
    const Passenger& getPassenger() const;
    const FlightRoute &getFlightRoute() const;
    QString getStatus() const;
    void setOrderId(const QString& id);
    void setPassenger(const Passenger& passenger);
    void setStatus(const QString& st);

    QJsonObject toJsonObject() const;

    bool operator == (const Order& other) const;

};
