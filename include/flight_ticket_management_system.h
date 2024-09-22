#ifndef FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
#define FLIGHT_TICKET_MANAGEMENT_SYSTEM_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "FlightNetwork.h"
#include "Order.h"
#include "OrderManager.h"
#include "User.h"
#include <QButtonGroup.h>
#include <QAbstractButton.h>
#include <QCalendarWidget>
#include <QTabWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class Flight_Ticket_Management_System;
}
QT_END_NAMESPACE

class Flight_Ticket_Management_System : public QMainWindow
{
    Q_OBJECT

public:
    explicit Flight_Ticket_Management_System(QWidget *parent = nullptr);
    ~Flight_Ticket_Management_System();
    void initLogin();
    void initializeOrdersTable();
    void loadUserOrders();


public:
    QString currentId; //当前用户身份证号
    QString currentPsw; //当前用户密码
    FlightNetwork network; //当前航线图
    Flight selectedFlight; //当前选择航班
    OrderManager orderManager; //管理所有订单


private:
    Ui::Flight_Ticket_Management_System *ui;

private slots:
    void Login();
    void Menu2Info();
    void Back2Menu();
    void Start2Menu();
    void Start2User();
    void toStart();
    void toInfo();
    void Exit();

    void initializeDepBox();
    void initializeArrBox();
    QString getDep(int index);
    QString getArr(int index);
    void Exchange();

    void searchFlights();
    void updateSearch(int buttonId);
    void updateTableWidget(const QVector<Flight>& flights);
    void showCalendar();
    void updateButtonWithDate(const QDate &date);

    void handleTicketBooking(int rowIndex);
    void addPassenger();
    void displayOrders(QTabWidget* tabWidget, const QString& statusFilter = "");
    void onTabChanged(int index);
    void handleRefund(const QString& orderId);
    void handleReschedule(const QString& orderId);
    void showMap();




protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标点击事件

};
#endif // FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
