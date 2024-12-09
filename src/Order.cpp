#include "Order.h"
#include "Flight.h"
#include "Passenger.h"
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Order::Order(){}

Order::Order(QString id, const Passenger& user, FlightRoute fr, QString m, QString ins, QString st, double pc, QString c)
    : orderId(id), passenger(user), flightroute(fr), meal(m), insurance(ins), status(st) ,price(pc), cabinClass(c){}

QString Order::getOrderId() const {
    return orderId;
}

void Order::setOrderId(const QString& id) {
    orderId = id;
}

const Passenger& Order::getPassenger() const {
    return passenger;
}

void Order::setPassenger(const Passenger& psg) {
    passenger = psg;
}

const FlightRoute& Order::getFlightRoute() const {
    return flightroute;
}

QString Order::getStatus() const {
    return status;
}

void Order::setStatus(const QString& st) {
    status = st;
}

double Order::getPrice() const {
    return price;
}

QString Order::getCabinClass() const {
    return cabinClass;
}

QString Order::getMeal() const {
    return meal;
}

QString Order::getInsurance() const {
    return insurance;
}

QJsonObject Order::toJsonObject() const {
    QJsonObject orderObject;
    orderObject["orderId"] = orderId;
    QJsonObject passengerObject;
    passengerObject["familyName"] = passenger.getFamilyName();
    passengerObject["givenName"] = passenger.getGivenName();
    passengerObject["sex"] = passenger.getSex();
    passengerObject["id"] = passenger.getId();
    passengerObject["phone"] = passenger.getPhone();
    orderObject["passenger"] = passengerObject;

    QJsonArray flightsArray;
    for (const std::shared_ptr<Flight> &flight : flightroute) {
        QJsonObject flightObject;
        flightObject["airline"] = flight->getAirline();
        flightObject["flightNumber"] = flight->getFlightNumber();
        flightObject["departureCity"] = flight->getDepartureCity();
        flightObject["departureTime"] = flight->getDepartureTime();
        flightObject["arrivalCity"] = flight->getArrivalCity();
        flightObject["arrivalTime"] = flight->getArrivalTime();
        flightObject["price"] = flight->getPrice();
        flightObject["remainSeatNum"] = flight->getRemainSeatNum();
        flightsArray.append(flightObject);
    }

    orderObject["flightRoute"] = flightsArray;
    orderObject["meal"] = meal;
    orderObject["insurance"] = insurance;
    orderObject["status"] = status;
    orderObject["price"] = price;
    orderObject["cabinClass"] = cabinClass;
    return orderObject;
}

bool Order::operator==(const Order& other) const {
    return orderId == other.orderId;
}

