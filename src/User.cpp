#include "User.h"
#include "Order.h"
#include "Utils.h"
#include "qdatetime.h"

User::User() : id() {
    updateVIPLevel();

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

double User::getAvgTransTime() const {
    return avgTransTime;
}

void User::setID(QString id) {
    this->id = id;
}

void User::setPassword(QString psw) {
    this->password = psw;
}

void User::setPriceRatio(double ratio) {
    priceRatio = ratio;
}

QVector<QString> User::getFrequentCities() const {
    return frequentCities;
}

QVector<QString> User::getFrequentAirTypes() const {
    return frequentAirTypes;
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
    avgTransNum = orders.isEmpty() ? 0 : sum / orders.size();
}

void User::updateAvgTransTime() {
    QList<Order> orders = orderManager.getOrders();
    int sum = 0;
    foreach (const Order &order, orders) {
        FlightRoute route = order.getFlightRoute();
        sum += route.getTotalTransferTime();
    }
    avgTransTime = orders.isEmpty() ? 0 : sum / orders.size();
}

void User::updatePriceRatio(const double selectedPriceRatio) {
    qDebug()<<selectedPriceRatio;
    double preRatio = priceRatio;
    double curRatio = SMOOTHING_FACTOR * selectedPriceRatio + (1 - SMOOTHING_FACTOR) * preRatio;
    priceRatio = curRatio;
    savePriceRatioToFile();
}

void User::savePriceRatioToFile() {
    QString filePath = USER_FILE;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading: " << filePath;
        return;
    }
    QTextStream in(&file);
    QStringList lines;
    bool found = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.size() >= 3 && fields[0] == id) {
            QString newPasswordHash = hashPassword(password);
            QString newLine = id + " " + newPasswordHash + " " + QString::number(priceRatio, 'f', 2);
            lines.append(newLine);
            found = true;
        } else {
            lines.append(line);
        }
    }
    file.close();

    if (found) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qWarning() << "Cannot open file for writing: " << filePath;
            return;
        }
        QTextStream out(&file);

        for (const QString &line : lines) {
            out << line << "\n";
        }
        file.close();
    } else {
        qWarning() << "User ID not found in file.";
    }
}

void User::updateFrequentCities() {
    QList<Order> orders = orderManager.getOrders();
    QMap<QString, int> cityFrequency;

    foreach (const Order &order, orders) {
        const FlightRoute &route = order.getFlightRoute();
        const QVector<std::shared_ptr<Flight>> flights = route.getFlights();
        cityFrequency[flights.first()->getDepartureCity()]++;
        cityFrequency[flights.last()->getArrivalCity()]++;
    }

    QList<QPair<QString, int>> frequencyList;
    for (auto it = cityFrequency.constBegin(); it != cityFrequency.constEnd(); ++it) {
        frequencyList.append(QPair<QString, int>(it.key(), it.value()));
    }

    std::sort(frequencyList.begin(), frequencyList.end(),
              [](const std::pair<QString, int> &a, const std::pair<QString, int> &b) {
                  return a.second > b.second;
              });

    frequentCities.clear();
    for (int i = 0; i < frequencyList.size() && i < 5; ++i) {
        frequentCities.append(frequencyList[i].first);
    }
}

void User::updateFrequentAirTypes() {
    QList<Order> orders = orderManager.getOrders();
    QMap<QString, int> airFrequency;

    foreach (const Order &order, orders) {
        const FlightRoute &route = order.getFlightRoute();
        const QVector<std::shared_ptr<Flight>> flights = route.getFlights();
        foreach(const std::shared_ptr<Flight> &f, flights) {
             airFrequency[f->getAircraftType()]++;
        }
    }

    QList<QPair<QString, int>> frequencyList;
    for (auto it = airFrequency.constBegin(); it != airFrequency.constEnd(); ++it) {
        frequencyList.append(QPair<QString, int>(it.key(), it.value()));
    }

    std::sort(frequencyList.begin(), frequencyList.end(),
              [](const std::pair<QString, int> &a, const std::pair<QString, int> &b) {
                  return a.second > b.second;
              });

    frequentAirTypes.clear();
    for (int i = 0; i < frequencyList.size() && i < 5; ++i) {
        frequentAirTypes.append(frequencyList[i].first);
    }
}

void User::updateUserWeight() {
    updateAvgDepTime();
    updateAvgTransNum();
    updateAvgTransTime();
    updateFrequentCities();
    updateFrequentAirTypes();
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

void User::loadPriceRatio() {
    QString filePath = USER_FILE;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading: " << filePath;
        return;
    }
    QTextStream in(&file);
    QString line;
    // 遍历文件的每一行
    while (!in.atEnd()) {
        line = in.readLine(); // 读取一行数据
        QStringList fields = line.split(" "); // 按照空格分割每一行的数据
        if (fields.size() >= 3) { // 确保有足够的字段
            // 检查当前行的用户名是否与用户的id匹配
            if (fields[0] == id) {
                bool ok;
                priceRatio = fields[2].toDouble(&ok); // 将第三个字段（priceRatio）转换为double类型
                if (!ok) {
                    qWarning() << "Failed to parse priceRatio from file. Line content:" << line;
                    priceRatio = 0; // 如果转换失败，设置priceRatio为0或默认值
                }
                break; // 找到匹配的用户后退出循环
            }
        }
    }
    file.close();

    // 输出调试信息
    if (qIsNaN(priceRatio)) {
        qWarning() << "priceRatio is NaN for user" << id << ". Check the file format.";
    } else {
        qDebug() << "Loaded priceRatio for user" << id << ": " << priceRatio;
    }
}

void User::updateVIPLevel() {
    double cost = orderManager.getTotalConsumption();
    int level = 0;
    double totalRequired = 10000;

    while (level < 10) {
        if (cost >= totalRequired) {
            level++;
            totalRequired += (level + 1) * 5000;
        } else {
            break;
        }
    }
    VIP = level;
}


