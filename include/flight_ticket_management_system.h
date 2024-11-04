#ifndef FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
#define FLIGHT_TICKET_MANAGEMENT_SYSTEM_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "FlightNetwork.h"
#include "OrderManager.h"
#include "FlightRoute.h"
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
        RESCHEDULE_ORDER,
        CANCELED_ORDER
    };

    enum MAP_TYPE{
        DOMESTIC,
        GLOBAL
    };

public:
    explicit Flight_Ticket_Management_System(QWidget *parent = nullptr);
    ~Flight_Ticket_Management_System();
    void initializeOrdersTable();
    void loadUserOrders();


public:
    QString currentUserId; //当前用户身份证号
    FlightNetwork network; //当前航线图
    FlightRoute selectedFlight; //当前选择航班
    OrderManager orderManager; //管理所有订单2
    Order rescheduleOrder; //要改签的订单
    QVector<FlightRoute> searchedFlights; //搜索出的航班
    QString depCity; //起飞城市
    QString arrCity; //到达城市
    QDate selectedDate; //出发时间

    ORDER_TYPE orderType; //当前订单类型
    MAP_TYPE map_type;

    void showLoginPage();
    void showMenuPage();
    void showSearchPage();
    void showInfoPage();
    void showCheckoutPage();
    void showUserPage();
    void showMapPage();
    void initReschedule(const QString& dep, const QString& arr, const QDate& date);
    void exitWindow();

    QSet<QString> domesitic_city = {"北京","哈尔滨","呼和浩特","乌鲁木齐","广州","拉萨", "昆明","成都","上海","南宁"};
    QSet<QString> global_city = {"伦敦","东京","拉斯维加斯","莫斯科","巴黎","纽约"};

private:
    Ui::Flight_Ticket_Management_System *ui;
    LoginPage *loginPage;
    MenuPage *menuPage;
    SearchPage *searchPage;
    InfoPage *infoPage;
    CheckoutPage *checkoutPage;
    UserPage *userPage;
    MapPage *mapPage;

};
#endif // FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
