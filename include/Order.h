#pragma once

#include <QString>
#include "User.h"
#include "Flight.h"

class Order {
public:
    enum class OrderType {
        DIRECT,
        TRANSFER,
    };

    static QString orderTypeToString(OrderType type);

private:
    QString orderId;
    User passenger;
    Flight flight;
    Flight flight2; // 转机时的第二班航班
    QString status;   // 订单状态（如：已支付、未支付、已取消）
    OrderType type = OrderType::DIRECT;

public:
    // 构造函数
    Order();
    Order(QString id, const User& user, const Flight& flt, QString status, OrderType type = OrderType::DIRECT, const Flight& flt2 = Flight());

    // Getter 和 Setter 方法
    QString getOrderId() const;
    void setOrderId(const QString& id);

    const User& getPassenger() const;
    void setPassenger(const User& user);

    const Flight& getFlight() const;
    void setFlight(const Flight& flt);

    const Flight& getFlight2() const;
    void setFlight2(const Flight& flt);

    QString getStatus() const;
    void setStatus(const QString& st);

    QString toString() const;

    bool operator == (const Order& other) const;

    OrderType getType() const;

};
