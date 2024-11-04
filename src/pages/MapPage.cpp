#include "pages/MapPage.h"
#include "ui_mappage.h"
#include <qwebengineview.h>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <iostream>
#include <QSurfaceFormat>

MapPage::MapPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::MapPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initMapPage();

    connect(ui->airlineComb, &QComboBox::currentIndexChanged, this, &MapPage::displayMap);
    connect(ui->backUserBtn, &QPushButton::released, this, &MapPage::map2user);
    connect(ui->startDateEdit, &QDateEdit::dateChanged, this, &MapPage::filterFlights);
    connect(ui->endDateEdit, &QDateEdit::dateChanged, this, &MapPage::filterFlights);
}

MapPage::~MapPage() {
    delete ui;
}

void MapPage::map2user() {
    mainWindow->showUserPage();
}

void MapPage::initWebEngine() {
    if(mainWindow->map_type == Flight_Ticket_Management_System::DOMESTIC) {
        ui->webEngineView->setUrl(QUrl::fromLocalFile("D:/CS/projects/Flight_Ticket_Management_System/web/html/domestic_routes.html"));
    } else {
        ui->webEngineView->setUrl(QUrl::fromLocalFile("D:/CS/projects/Flight_Ticket_Management_System/web/html/global_routes.html"));
    }
    configureWebEngine();
}

void MapPage::configureWebEngine() {
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::LocalStorageEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::WebGLEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::AllowRunningInsecureContent, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::ErrorPageEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::PluginsEnabled, false);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::AllowGeolocationOnInsecureOrigins, false);
}

void MapPage::initMapPage() {
    QDate specificStartDate(2024, 1, 1); // 设置开始日期为2024年1月1日
    QDate specificEndDate(2024, 12, 31);   // 设置结束日期为2024年1月31日
    ui->startDateEdit->setDate(specificStartDate); // 设置开始日期
    ui->endDateEdit->setDate(specificEndDate);     // 设置结束日期
    initWebEngine();
    showMap();
}

void MapPage::showMap() {
    filterFlights();
    connect(ui->webEngineView, &QWebEngineView::loadFinished, this,
            [&](bool load) {
                if (load) {
                    displayMap(0);
                }
            });
}

void MapPage::displayMap(int index) {
    if (index < 0 || index >= addedOrders.size()) {
        return;
    }
    if (mainWindow->map_type == Flight_Ticket_Management_System::DOMESTIC) {
        processLeafletMap(index, addedOrders);
    } else if (mainWindow->map_type == Flight_Ticket_Management_System::GLOBAL) {
        processCesiumMap(index, addedOrders);
    }
}

void MapPage::processLeafletMap(int index, QList<Order> orders) {
    ui->webEngineView->page()->runJavaScript("clearGeodesicLines()");
    if (index == 0) { // 显示所有航线
        for (const auto& order : orders) {
            const FlightRoute& flightRoute = order.getFlightRoute();
            for (const Flight* flight : flightRoute) {
                QString depCity = flight->getDepartureCity();
                QString arrCity = flight->getArrivalCity();
                float depCity_x = cityCoordinates.value(depCity, {0, 0}).first;
                float depCity_y = cityCoordinates.value(depCity, {0, 0}).second;
                float arrCity_x = cityCoordinates.value(arrCity, {0, 0}).first;
                float arrCity_y = cityCoordinates.value(arrCity, {0, 0}).second;
                QString flightInfo = flight->getAirline() + " " + flight->getFlightNumber();
                QString jsCode = QString("drawGeodesicLine(%1, %2, %3, %4, '%5');")
                                     .arg(depCity_x)
                                     .arg(depCity_y)
                                     .arg(arrCity_x)
                                     .arg(arrCity_y)
                                     .arg(flightInfo);
                ui->webEngineView->page()->runJavaScript(jsCode);
            }
        }
    } else {
        const Order& order = orders.at(index - 1);
        qDebug()<<order.getFlightRoute().showCityPath();
        const FlightRoute& flightRoute = order.getFlightRoute();
        for (const Flight* flight : flightRoute) {
            QString depCity = flight->getDepartureCity();
            QString arrCity = flight->getArrivalCity();
            float depCity_x = cityCoordinates.value(depCity, {0, 0}).first;
            float depCity_y = cityCoordinates.value(depCity, {0, 0}).second;
            float arrCity_x = cityCoordinates.value(arrCity, {0, 0}).first;
            float arrCity_y = cityCoordinates.value(arrCity, {0, 0}).second;
            QString flightInfo = flight->getAirline() + " " + flight->getFlightNumber();
            QString jsCode = QString("drawGeodesicLine(%1, %2, %3, %4, '%5');")
                                 .arg(depCity_x)
                                 .arg(depCity_y)
                                 .arg(arrCity_x)
                                 .arg(arrCity_y)
                                 .arg(flightInfo);
            ui->webEngineView->page()->runJavaScript(jsCode);
            }
    }
}

void MapPage::processCesiumMap(int index, QList<Order> orders) {
    if (index == 0) {
        for (const auto& order : orders) {

            const FlightRoute& flightRoute = order.getFlightRoute();
            for (const Flight* flight : flightRoute) {
                QString depCity = flight->getDepartureCity();
                QString arrCity = flight->getArrivalCity();
                float depCity_x = cityCoordinates.value(depCity, {0, 0}).first;
                float depCity_y = cityCoordinates.value(depCity, {0, 0}).second;
                float arrCity_x = cityCoordinates.value(arrCity, {0, 0}).first;
                float arrCity_y = cityCoordinates.value(arrCity, {0, 0}).second;
                QString flightInfo = flight->getAirline() + " " + flight->getFlightNumber();

                QString jsCode = QString("drawPolyline(%1, %2, %3, %4, '%5');")
                                     .arg(depCity_x)
                                     .arg(depCity_y)
                                     .arg(arrCity_x)
                                     .arg(arrCity_y)
                                     .arg(flightInfo);
                ui->webEngineView->page()->runJavaScript(jsCode, [](const QVariant &result) {
                    std::cout << "JavaScript execution result: "
                              << result.toString().toStdString();
                });
            }
        }
    } else {
        const Order& order = orders.at(index - 1);
        //ui->webEngineView->page()->runJavaScript("clearGeodesicLines()");
        const FlightRoute& flightRoute = order.getFlightRoute();
        for (const Flight* flight : flightRoute) {
            QString depCity = flight->getDepartureCity();
            QString arrCity = flight->getArrivalCity();
            float depCity_x = cityCoordinates.value(depCity, {0, 0}).first;
            float depCity_y = cityCoordinates.value(depCity, {0, 0}).second;
            float arrCity_x = cityCoordinates.value(arrCity, {0, 0}).first;
            float arrCity_y = cityCoordinates.value(arrCity, {0, 0}).second;
            QString flightInfo = flight->getAirline() + " " + flight->getFlightNumber();
            QString jsCode = QString("drawPolyline(%1, %2, %3, %4, '%5');")
                                 .arg(depCity_x)
                                 .arg(depCity_y)
                                 .arg(arrCity_x)
                                 .arg(arrCity_y)
                                 .arg(flightInfo);
            ui->webEngineView->page()->runJavaScript(jsCode, [](const QVariant &result) {
                std::cout << "JavaScript execution result: "
                          << result.toString().toStdString();
            });
        }

    }
}

void MapPage::filterFlights() {
    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();

    addedOrders.clear();
    ui->airlineComb->clear();
    ui->airlineComb->addItem("全部航线");
    ui->webEngineView->page()->runJavaScript("clearGeodesicLines()");

    QList<Order> filteredOrders;
    auto orders = mainWindow->orderManager.getOrders();
    for (const Order& order : orders) {
        if (order.getStatus() == "已支付") {
            FlightRoute flightRoute = order.getFlightRoute();
            QDate firstDepartureDate = QDate::fromString(flightRoute.first()->getDepartureTime().split(" ")[0], "yyyy-MM-dd");
            QDate lastArrivalDate = QDate::fromString(flightRoute.last()->getArrivalTime().split(" ")[0], "yyyy-MM-dd");
            if (firstDepartureDate >= startDate && lastArrivalDate <= endDate) {
                ui->airlineComb->addItem(flightRoute.showCityPath());
                filteredOrders.append(order);
            }
        }
    }
    addedOrders = filteredOrders;
    displayMap(0);
}
