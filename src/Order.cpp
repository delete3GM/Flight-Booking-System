#include "Order.h"
#include "Flight.h"
#include "User.h"
#include <QString>

Order::Order(){}

Order::Order(QString id, const User& user, const Flight& flt, QString st, OrderType tp, const Flight& flt2)
    : orderId(id), passenger(user), flight(flt), status(st),flight2(flt2),type(tp) {}

QString Order::getOrderId() const {
    return orderId;
}

void Order::setOrderId(const QString& id) {
    orderId = id;
}

const User& Order::getPassenger() const {
    return passenger;
}

void Order::setPassenger(const User& user) {
    passenger = user;
}

const Flight& Order::getFlight() const {
    return flight;
}

void Order::setFlight(const Flight& flt) {
    flight = flt;
}

const Flight& Order::getFlight2() const {
    return flight2;
}

void Order::setFlight2(const Flight& flt) {
    flight2 = flt;
}

QString Order::getStatus() const {
    return status;
}

void Order::setStatus(const QString& st) {
    status = st;
}

QString Order::toString() const {
    if(type == OrderType::DIRECT)
    {
        return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15")
            .arg(orderId)
            .arg(passenger.getFamilyName())
            .arg(passenger.getGivenName())
            .arg(passenger.getSex())
            .arg(passenger.getId())
            .arg(passenger.getPhone())
            .arg(flight.getAirline())
            .arg(flight.getFlightNumber())
            .arg(flight.getDepartureCity())
            .arg(flight.getDepartureTime())
            .arg(flight.getArrivalCity())
            .arg(flight.getArrivalTime())
            .arg(flight.getPrice(), 0, 'f', 2)
            .arg(flight.getRemainSeatNum())
            .arg(status);
    } else {
        return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23")
            .arg(orderId)
            .arg(passenger.getFamilyName())
            .arg(passenger.getGivenName())
            .arg(passenger.getSex())
            .arg(passenger.getId())
            .arg(passenger.getPhone())
            .arg(flight.getAirline())
            .arg(flight.getFlightNumber())
            .arg(flight.getDepartureCity())
            .arg(flight.getDepartureTime())
            .arg(flight.getArrivalCity())
            .arg(flight.getArrivalTime())
            .arg(flight.getPrice(), 0, 'f', 2)
            .arg(flight.getRemainSeatNum())
            .arg(flight2.getAirline())
            .arg(flight2.getFlightNumber())
            .arg(flight2.getDepartureCity())
            .arg(flight2.getDepartureTime())
            .arg(flight2.getArrivalCity())
            .arg(flight2.getArrivalTime())
            .arg(flight2.getPrice(), 0, 'f', 2)
            .arg(flight2.getRemainSeatNum())
            .arg(status);
    }
}


bool Order::operator==(const Order& other) const {
    return orderId == other.orderId;
}

Order::OrderType Order::getType() const {
    return type;
}

QString Order::orderTypeToString(OrderType type) {
    switch (type) {
    case OrderType::DIRECT:
        return "DIRECT";
    case OrderType::TRANSFER:
        return "TRANSFER";
    default:
        return "UNKNOWN";
    }
}
