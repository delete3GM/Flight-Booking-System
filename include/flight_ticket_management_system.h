#ifndef FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
#define FLIGHT_TICKET_MANAGEMENT_SYSTEM_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "FlightNetwork.h"
#include "FlightRoute.h"
#include "User.h"
#include "Utils.h"
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

    enum MAP_TYPE{
        DOMESTIC,
        GLOBAL
    };

private:
    Ui::Flight_Ticket_Management_System *ui;
    LoginPage *loginPage;
    MenuPage *menuPage;
    SearchPage *searchPage;
    InfoPage *infoPage;
    CheckoutPage *checkoutPage;
    UserPage *userPage;
    MapPage *mapPage;



public:
    User currentUser; //登录用户
    FlightNetwork network; //航线网络
    FlightRoute selectedFlight; //当前选择航班
    Order rescheduleOrder; //要改签的订单
    QVector<FlightRoute> recommendationFlights; //推荐航班
    QVector<FlightRoute> searchedFlights; //搜索出的航班
    QString depCity; //起飞城市
    QString arrCity; //到达城市
    QDate selectedDate; //出发时间
    QString selectedClass; //舱位
    QMap<QString, CityInfo> cityInfoMap; //城市信息

    ORDER_TYPE orderType; //当前订单类型
    MAP_TYPE map_type; //当前地图类型



public:
    explicit Flight_Ticket_Management_System(QWidget *parent = nullptr);
    ~Flight_Ticket_Management_System();
    void initializeOrdersTable();
    void initReschedule(const QString& dep, const QString& arr, const QDate& date);

    void showLoginPage();
    void showMenuPage();
    void showSearchPage();
    void showInfoPage();
    void showCheckoutPage();
    void showUserPage();
    void showMapPage();
    void exitWindow();


};
#endif // FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
