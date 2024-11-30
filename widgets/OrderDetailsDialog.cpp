#include "OrderDetailsDialog.h"
#include "Order.h"
#include <QVBoxLayout>

OrderDetailsDialog::OrderDetailsDialog(const Order& order, QWidget* parent) : QDialog(parent), order(order) {
    setupUI();
    resize(300, 200);
    setWindowTitle("订单详情");
}

void OrderDetailsDialog::setupUI() {
    layout = new QVBoxLayout(this);
    orderDetailsLabel = new QLabel(this);

    QString details = QString("订单号: %1<br>"
                              "日期：%2<br>"
                              "航线: %3<br>"
                              "舱位: %4<br>"
                              "餐食：%5<br>"
                              "————————————————<br>"
                              "乘客姓名: %6<br>"
                              "证件号：%7<br>"
                              "联系方式：%8<br>"
                              "————————————————<br>"
                              "订单总价: <b><font size=5>%9</font></b>")
                          .arg(order.getOrderId())
                          .arg(order.getFlightRoute().getDepartureTime() + " —— " + order.getFlightRoute().getArrivalTime())
                          .arg(order.getFlightRoute().showCityPath())
                          .arg(order.getCabinClass())
                          .arg(order.getMeal())
                          .arg(order.getPassenger().getFamilyName() + " " + order.getPassenger().getGivenName())
                          .arg(order.getPassenger().getId())
                          .arg(order.getPassenger().getPhone())
                          .arg("￥ " + QString::number(order.getPrice()));

    orderDetailsLabel->setText(details);
    orderDetailsLabel->setTextFormat(Qt::RichText);
    layout->addWidget(orderDetailsLabel);
}
