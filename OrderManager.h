#pragma once

#include <QList>
#include <QString>
#include "Order.h"

class OrderManager {
private:
    QList<Order> orders;

public:
    QList<Order> getOrders();
    void addOrder(const Order& order);
    void modifyOrder(int index, const Order& newOrder);
    void saveOrdersToFile(const QString& filePath);
    void loadOrdersFromFile(const QString& filePath);
};
