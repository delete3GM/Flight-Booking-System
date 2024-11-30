#pragma once

#include <QList>
#include <QString>
#include "Order.h"

class OrderManager {
private:
    QList<Order> orders;

public:
    QList<Order>& getOrders();
    int getOrderNum();
    double getTotalConsumption();
    void addOrder(const Order& order);
    void modifyOrder(int index, const Order& newOrder);
    bool saveOrdersToJsonFile(const QString& filePath);
    void loadOrdersFromJsonFile(const QString& filePath);
    void clear();
};
