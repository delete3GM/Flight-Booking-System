# pragma once

#include <QString>
#include "User.h"
#include "Flight.h"

class Order {
private:
    QString orderId;
    User passenger;
    Flight flight;
    QString status;   // 订单状态（如：已支付、未支付、已取消）

public:
    // 构造函数
    Order();
    Order(QString id, const User& user, const Flight& flt, QString status);

    // Getter 和 Setter 方法
    QString getOrderId() const;
    void setOrderId(const QString& id);

    const User& getPassenger() const;
    void setPassenger(const User& user);

    const Flight& getFlight() const;
    void setFlight(const Flight& flt);

    QString getStatus() const;
    void setStatus(const QString& st);

    QString toString() const;

};
