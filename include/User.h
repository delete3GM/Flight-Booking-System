#ifndef USER_H
#define USER_H

#include "Order.h"
#include "OrderManager.h"

class User {

private:
    QString id;
    QString password;
    int VIP = 0;

    double avgDepTime = 0;
    double avgTransNum = 0;
    double avgTransTime = 0;
    double priceRatio = 1; // 当前订票价格/航线平均价格 递推：xi+1=ax+(1-a)xi

    QVector<QString> frequentCities;
    QVector<QString> frequentAirTypes;
    void updateAvgDepTime();
    void updateAvgTransNum();
    void updateAvgTransTime();
    void updateFrequentCities();
    void updateFrequentAirTypes();

public:
    OrderManager orderManager;

public:
    User();

    QString getID() const;
    int getVIPLevel() const;

    void setID(QString id);
    void setPassword(QString psw);
    void updateVIPLevel();

    double getAvgDepTime() const;
    double getAvgTransNum() const;
    double getAvgTransTime() const;
    double getPriceRatio() const;
    void setPriceRatio(double ratio);
    void updatePriceRatio(const double selectedPriceRatio);
    QVector<QString> getFrequentCities() const;
    QVector<QString> getFrequentAirTypes() const;

    void updateUserWeight();
    void loadUserOrders();
    void loadPriceRatio();
    void savePriceRatioToFile();

};

#endif // USER_H
