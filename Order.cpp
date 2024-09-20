#include "Order.h"
#include "User.h"
#include "qdebug.h"
#include <QString>

Order::Order(){}

Order::Order(QString id, const User& user, const Flight& flt, QString st)
    : orderId(id), passenger(user), flight(flt), status(st) {}

// Getter 和 Setter 方法
QString Order::getOrderId() const
{
    return orderId;
}

void Order::setOrderId(const QString& id)
{
    orderId = id;
}

const User& Order::getPassenger() const
{
    return passenger;
}

void Order::setPassenger(const User& user)
{
    passenger = user;
}

const Flight& Order::getFlight() const
{
    return flight;
}

void Order::setFlight(const Flight& flt)
{
    flight = flt;
}

QString Order::getStatus() const
{
    return status;
}

void Order::setStatus(const QString& st)
{
    status = st;
}

QString Order::toString() const
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
}
