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

class Flight_Ticket_Management_System : public QMainWindow
{
    Q_OBJECT


     // 用于指示当前订单时新添加的订单还是改签订单
    enum ORDER_TYPE {
        NONE, // 默认值
        NEW_ORDER, // 新订单
        RESCHEDULE_ORDER // 改签订单
    };

    enum SEARCH_TYPE{
        DIRECT,
        TRANSFER
    };

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
    Flight selectedFlight2; //当前选择航班
    OrderManager orderManager; //管理所有订单
    ORDER_TYPE orderType; //当前订单类型
    Order rescheduleOrder; //要改签的订单
    SEARCH_TYPE searchType; //当前搜索类型


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
    void setUserHint(int index);

    void initializeDepBox();
    void initializeArrBox();
    QString getDep(int index);
    QString getArr(int index);
    void Exchange();

    void searchFlights();
    void updateSearch(int buttonId);
    void searchFlightsWithTransfers();
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
    void displayMap(int index);




protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标点击事件

    QMap<QString, QPair<float, float>> cityCoordinates = {
        QPair<QString, QPair<float, float>>(
            "北京", QPair<float, float>(39.9042f, 116.4074f)),
        QPair<QString, QPair<float, float>>(
            "上海", QPair<float, float>(31.2304f, 121.4737f)),
        QPair<QString, QPair<float, float>>(
            "重庆", QPair<float, float>(29.5630f, 106.5516f)),
        QPair<QString, QPair<float, float>>(
            "南宁", QPair<float, float>(22.8195f, 108.3150f)),
        QPair<QString, QPair<float, float>>(
            "南京", QPair<float, float>(32.0603f, 118.7969f)),
        QPair<QString, QPair<float, float>>(
            "杭州", QPair<float, float>(30.2741f, 120.1551f)),
        QPair<QString, QPair<float, float>>(
            "广州", QPair<float, float>(23.1291f, 113.2644f)),
        QPair<QString, QPair<float, float>>(
            "昆明", QPair<float, float>(25.0389f, 102.7183f)),
        QPair<QString, QPair<float, float>>(
            "西安", QPair<float, float>(34.3416f, 108.9398f)),
        QPair<QString, QPair<float, float>>(
            "深圳", QPair<float, float>(22.5431f, 114.0579f)),
        QPair<QString, QPair<float, float>>(
            "成都", QPair<float, float>(30.5728f, 104.0668f))};

  private:
    float departureCity_x = 0;
    float departureCity_y = 0;
    float arrivalCity_x = 0;
    float arrivalCity_y = 0;
};
#endif // FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
