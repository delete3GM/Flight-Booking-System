#include "flight_ticket_management_system.h"
#include "ui_flight_ticket_management_system.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QDate>
#include <QPainter>
#include <QUuid>
#include <QListWidget>
#include <QRadioButton>
#include <QStandardItemModel>
#include <QUuid>
#include <qcontainerfwd.h>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <qdatetime.h>
#include <QTimer>
#include <qlist.h>
#include <qwebengineview.h>
#include "pages/LoginPage.h"
#include "pages/MenuPage.h"
#include "pages/SearchPage.h"
#include "pages/InfoPage.h"
#include "pages/CheckoutPage.h"
#include "pages/UserPage.h"
#include "pages/MapPage.h"


Flight_Ticket_Management_System::Flight_Ticket_Management_System(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Flight_Ticket_Management_System) {
    ui->setupUi(this);

    showLoginPage();
}

Flight_Ticket_Management_System::~Flight_Ticket_Management_System() {
    delete ui;
}

void Flight_Ticket_Management_System::exitWindow() {
    this->close();
}

void Flight_Ticket_Management_System::showLoginPage() {
    loginPage = new LoginPage(this);
    setCentralWidget(loginPage);
}

void Flight_Ticket_Management_System::showMenuPage() {
    menuPage = new MenuPage(this);
    setCentralWidget(menuPage);
}

void Flight_Ticket_Management_System::showSearchPage() {
    searchPage = new SearchPage(this);
    setCentralWidget(searchPage);
}

void Flight_Ticket_Management_System::showInfoPage() {
    infoPage = new InfoPage(this);
    setCentralWidget(infoPage);
    infoPage->updateTableWidget(searchedFlights);
}

void Flight_Ticket_Management_System::showCheckoutPage() {
    checkoutPage = new CheckoutPage(this);
    setCentralWidget(checkoutPage);
}

void Flight_Ticket_Management_System::showUserPage() {
    userPage = new UserPage(this);
    setCentralWidget(userPage);
}

void Flight_Ticket_Management_System::showMapPage() {
    mapPage = new MapPage(this);
    setCentralWidget(mapPage);
}

void Flight_Ticket_Management_System::initReschedule(const QString& dep, const QString& arr, const QDate& selectDate) {
    searchPage = new SearchPage(this);
    searchPage->setDataForReschedule(dep, arr, selectDate);
    setCentralWidget(searchPage);
}

void Flight_Ticket_Management_System::loadUserOrders() {
    QString filePath = ORDER_PATH + currentId + ".txt";
    QFile file(filePath);

    if (!file.exists()) {
        // 若不存在创建一个新文件
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Cannot create file: " << filePath;
            return;
        }
        file.close();
        qInfo() << "创建新文件: " << filePath;
    }
    qInfo() << "用户文件: " << currentId + ".txt";
    orderManager.loadOrdersFromFile(filePath);
}


