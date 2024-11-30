#ifndef USER_H
#define USER_H

#include "Order.h"
#include "OrderManager.h"

class User {

private:
    QString id;
    int VIP = 0;

    double avgDepTime = 1; //平均
    double avgTransNum = 1; //通常转机的次数
    double priceRatio = 1; //当前订票价格/航线平均价格 xi+1=ax+(1-a)xi

    QVector<QString> frequentCities;
    void updateAvgDepTime();
    void updateAvgTransNum();
    void updatePriceRatio();
    void updateFrequentCities();

public:
    OrderManager orderManager; // 管理用户订单

public:
    User();

    QString getID() const;
    int getVIPLevel() const;

    void setID(QString id);
    void updateVIPLevel();

    double getAvgDepTime() const;
    double getAvgTransNum() const;
    double getPriceRatio() const;
    void setPriceRatio(double ratio);
    QVector<QString> getFrequentCities() const;

    void updateUserWeight();
    void loadUserOrders();

};

#endif // USER_H
