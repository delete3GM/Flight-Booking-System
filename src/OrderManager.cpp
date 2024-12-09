#include "OrderManager.h"
#include "Order.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

void OrderManager::clear() {
    orders.clear();
}

QList<Order>& OrderManager::getOrders() {
    return this->orders;
}

int OrderManager::getOrderNum() {
    return orders.size();
}

void OrderManager::addOrder(const Order& order) {
    orders.append(order);
}

void OrderManager::modifyOrder(int index, const Order& newOrder) {
    if (index >= 0 && index < orders.size()) {
        orders[index] = newOrder;
    }
}

bool OrderManager::saveOrdersToJsonFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Cannot open file for writing: " << filePath;
        return false;
    }
    QJsonArray ordersArray;
    for (const Order& order : orders) {
        ordersArray.append(order.toJsonObject());
    }
    QJsonDocument doc(ordersArray);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void OrderManager::loadOrdersFromJsonFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading: " << filePath;
        return;
    }
    QTextStream in(&file);
    QString jsonText = in.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON";
        return;
    }
    QJsonArray ordersArray = doc.array();
    qDebug() << "Number of orders:" << ordersArray.size();
    for (const QJsonValue &value : ordersArray) {
        QJsonObject obj = value.toObject();

        QString orderId = obj["orderId"].toString();
        Passenger passenger(obj["passenger"].toObject()["familyName"].toString(),
                            obj["passenger"].toObject()["givenName"].toString(),
                            obj["passenger"].toObject()["sex"].toString(),
                            obj["passenger"].toObject()["id"].toString(),
                            obj["passenger"].toObject()["phone"].toString());

        QVector<Flight> flights;
        QJsonArray flightsArray = obj["flightRoute"].toArray();
        for (const QJsonValue &flightValue : flightsArray) {
            QJsonObject flightObj = flightValue.toObject();
            Flight flight(flightObj["airline"].toString(), flightObj["flightNumber"].toString(),
                          flightObj["aircraftType"].toString(),flightObj["departureCity"].toString(),
                          flightObj["departureTime"].toString(),  flightObj["arrivalCity"].toString(),
                          flightObj["arrivalTime"].toString(), flightObj["price"].toDouble(),
                          flightObj["remainSeatNum"].toInt());
            flights.append(flight);
        }
        QString meal = obj["meal"].toString();
        QString insurance = obj["insurance"].toString();
        QString status = obj["status"].toString();
        double price = obj["price"].toDouble();
        QString cabinClass = obj["cabinClass"].toString();
        FlightRoute flightRoute(flights);
        Order order(orderId, passenger, flightRoute, meal, insurance, status, price, cabinClass);

        bool exists = false;
        for (const Order& existingOrder : orders) {
            if (existingOrder.getOrderId() == order.getOrderId()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            orders.append(order);
        }
    }
}

double OrderManager::getTotalConsumption() {
    double total = 0;
    for (const Order& order : orders) {
        if(order.getStatus() == "已支付")
            total += order.getPrice();
    }
    return total;
}

