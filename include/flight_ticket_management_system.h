#ifndef FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
#define FLIGHT_TICKET_MANAGEMENT_SYSTEM_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "FlightNetwork.h"
#include "OrderManager.h"
#include <QButtonGroup>
#include <QAbstractButton>
#include <QCalendarWidget>
#include <QTabWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <qdatetime.h>
#include <qmap.h>
#include <QWebEnginePage>
#include <QWebChannel>


QT_BEGIN_NAMESPACE
namespace Ui {
    class Flight_Ticket_Management_System;
}
QT_END_NAMESPACE

class LoginPage;
class MenuPage;
class SearchPage;
class InfoPage;
class CheckoutPage;
class UserPage;
class MapPage;

class Flight_Ticket_Management_System : public QMainWindow {
    Q_OBJECT

public:
    enum ORDER_TYPE {
        NONE,
        NEW_ORDER,
        RESCHEDULE_ORDER
    };

    enum SEARCH_TYPE{
        DIRECT,
        TRANSFER
    };

public:
    explicit Flight_Ticket_Management_System(QWidget *parent = nullptr);
    ~Flight_Ticket_Management_System();
    void initializeOrdersTable();
    void loadUserOrders();


public:
    QString currentId; //当前用户身份证号
    FlightNetwork network; //当前航线图
    Flight selectedFlight; //当前选择航班
    Flight selectedFlight2; //当前选择航班
    OrderManager orderManager; //管理所有订单
    ORDER_TYPE orderType; //当前订单类型
    Order rescheduleOrder; //要改签的订单
    SEARCH_TYPE searchType; //当前搜索类型
    QVector<Flight> searchedFlights; //搜索出的航班
    QString depCity; //起飞城市
    QString arrCity; //到达城市
    QDate selectedDate; //出发时间

    // change pages
    void showLoginPage();
    void showMenuPage();
    void showSearchPage();
    void showInfoPage();
    void showCheckoutPage();
    void showUserPage();
    void showMapPage();
    void initReschedule(const QString& dep, const QString& arr, const QDate& date);
    void exitWindow();

private:
    Ui::Flight_Ticket_Management_System *ui;
    LoginPage *loginPage;
    MenuPage *menuPage;
    SearchPage *searchPage;
    InfoPage *infoPage;
    CheckoutPage *checkoutPage;
    UserPage *userPage;
    MapPage *mapPage;

private slots:


};
#endif // FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
