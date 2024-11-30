#include "User.h"
#include "Order.h"
#include "Utils.h"
#include "qdatetime.h"

User::User() : id() {
    updateVIPLevel();
    void updateEarlyWeight();
    void updateTransWeight();
    void updatePriceWeight();
}

QString User::getID() const {
    return id;
}

double User::getAvgDepTime() const {
    return avgDepTime;
}

double User::getAvgTransNum() const {
    return avgTransNum;
}

double User::getPriceRatio() const {
    return priceRatio;
}

void User::setID(QString id) {
    this->id = id;
}

void User::setPriceRatio(double ratio) {
    priceRatio = ratio;
}

QVector<QString> User::getFrequentCities() const {
    return frequentCities;
}

void User::updateAvgDepTime() {
    QList<Order> orders = orderManager.getOrders();
    int totalHours = 0;
    foreach (const Order &order, orders) {
        QDateTime departureTime = QDateTime::fromString(order.getFlightRoute().getDepartureTime(), "yyyy-MM-dd HH:mm");
        QTime timePart = departureTime.time();
        totalHours += timePart.hour();
    }
    avgDepTime = orders.isEmpty() ? 1.0 : totalHours / orders.size();
}

void User::updateAvgTransNum() {
    QList<Order> orders = orderManager.getOrders();
    int sum = 0;
    foreach (const Order &order, orders) {
        FlightRoute route = order.getFlightRoute();
        sum += route.getTransferCount();
    }
    double weight = static_cast<double>(sum) / orders.size();
    avgTransNum = orders.isEmpty() ? 1.0 : weight;
}

void User::updatePriceRatio() {
    // 这里应该包含设置 transWeight 的逻辑
    // 例如，可以从用户输入获取，或者基于某种逻辑计算得出
    // 这里只是一个示例，将权重设置为一个固定值
    priceRatio = 1.0; // 默认值，您可以根据需要修改
}

void User::updateFrequentCities() {
    QList<Order> orders = orderManager.getOrders();
    QMap<QString, int> cityFrequency;

    // 遍历所有订单，统计每个城市的访问次数
    foreach (const Order &order, orders) {
        const FlightRoute &route = order.getFlightRoute();
        const QVector<std::shared_ptr<Flight>> flights = route.getFlights();
       // foreach (const Flight* flight, flights) {
            // 假设出发城市是用户访问的城市
        cityFrequency[flights.first()->getDepartureCity()]++;
        cityFrequency[flights.last()->getArrivalCity()]++;
       // }
    }

    // 将QMap转换为QList，以便排序
    QList<QPair<QString, int>> frequencyList;
    for (auto it = cityFrequency.constBegin(); it != cityFrequency.constEnd(); ++it) {
        frequencyList.append(QPair<QString, int>(it.key(), it.value()));
    }

    // 根据访问次数降序排序
    std::sort(frequencyList.begin(), frequencyList.end(),
              [](const std::pair<QString, int> &a, const std::pair<QString, int> &b) {
                  return a.second > b.second;
              });

    // 保留访问次数最多的前5个城市
    frequentCities.clear();
    for (int i = 0; i < frequencyList.size() && i < 5; ++i) {
        frequentCities.append(frequencyList[i].first);
    }
}

void User::updateUserWeight() {
    updateAvgDepTime();
    updateAvgTransNum();
    updatePriceRatio();
    updateFrequentCities();
}

int User::getVIPLevel() const {
    return VIP;
}

void User::loadUserOrders() {
    QString filePath = ORDER_PATH + id + ".json";
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "用户订单文件不存在，将创建新文件:" << filePath;
        file.open(QIODevice::WriteOnly);
    } else {
        qDebug() << "用户文件:" << id + ".json";

        this->orderManager.loadOrdersFromJsonFile(filePath);
    }
}

void User::updateVIPLevel() {
    double cost = orderManager.getTotalConsumption();
    VIP = cost / 15000;
}


