#include "OrderManager.h"
#include "Order.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QList<Order>& OrderManager::getOrders()
{
    return this->orders;
}

void OrderManager::addOrder(const Order& order) {
    orders.append(order);
}

void OrderManager::modifyOrder(int index, const Order& newOrder) {
    if (index >= 0 && index < orders.size()) {
        orders[index] = newOrder;
    }
}

bool OrderManager::saveOrdersToFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing: " << filePath;
        return false;
    }
    QTextStream out(&file);
    for (const Order& order : orders) {
        out << order.toString() << "\n";
    }
    file.close();
    return true;
}

void OrderManager::loadOrdersFromFile(const QString& filePath) {
    //从文件中逐行读取订单，并加载到orders中
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading: " << filePath;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 15) {
            User user(fields[1], fields[2], fields[3], fields[4], fields[5]);
            Flight flight(fields[6], fields[7], fields[8], fields[9], fields[10], fields[11], fields[12].toDouble(), fields[13].toInt());
            Order order(fields[0], user, flight, fields[14]);

            // 检查订单是否已存在
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
        else if(fields.size()==23)
        {
            User user(fields[1], fields[2], fields[3], fields[4], fields[5]);
            Flight flight1(fields[6], fields[7], fields[8], fields[9], fields[10], fields[11], fields[12].toDouble(), fields[13].toInt());
            Flight flight2(fields[14], fields[15], fields[16], fields[17], fields[18], fields[19], fields[20].toDouble(), fields[21].toInt());
            Order order(fields[0], user, flight1, fields[22], Order::OrderType::TRANSFER, flight2);

            // 检查订单是否已存在
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
    file.close();
}
