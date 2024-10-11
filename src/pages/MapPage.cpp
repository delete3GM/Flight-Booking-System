#include "pages/MapPage.h"
#include "ui_mappage.h"
#include <qwebengineview.h>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <iostream>

MapPage::MapPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::MapPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    // 加载本地 HTML 文件
    ui->webEngineView->setUrl(QUrl::fromLocalFile("D:/CS/projects/Flight_Ticket_Management_System/web/html/map.html"));

    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);
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
        QWebEngineSettings::PluginsEnabled, true);
    ui->webEngineView->settings()->setAttribute(
        QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);

    showMap();

    connect(ui->airlineComb, SIGNAL(currentIndexChanged(int)), this, SLOT(displayMap(int)));
    connect(ui->backUserBtn, SIGNAL(released()), this, SLOT(map2user()));
}

MapPage::~MapPage() {
    delete ui;
}

void MapPage::map2user() {
    mainWindow->showUserPage();
}

void MapPage::showMap() {
    ui->airlineComb->clear();
    // 将订单中已支付的航班显示在地图上
    ui->airlineComb->addItem("全部航线");
    auto orders = mainWindow->orderManager.getOrders();
    for (auto order : orders) {

        //qDebug()<<order.toString()<<"  " << Order::orderTypeToString(order.getType()) << "\n";

        if (order.getStatus() == "已支付") {
            if(order.getType() == Order::OrderType::DIRECT) {
                Flight flight = order.getFlight();
                QString item;
                item.append(flight.getDepartureCity());
                item.append("->");
                item.append(flight.getArrivalCity());
                ui->airlineComb->addItem(item);
            }
            else if(order.getType() == Order::OrderType::TRANSFER) {
                Flight flight1 = order.getFlight();
                Flight flight2 = order.getFlight2();
                QString item;
                item.append(flight1.getDepartureCity());
                item.append("->");
                item.append(flight1.getArrivalCity());
                item.append("->");
                item.append(flight2.getArrivalCity());
                ui->airlineComb->addItem(item);
            }
        }
    }
    connect(ui->webEngineView, &QWebEngineView::loadFinished, this,
            [&](bool load) {
                if (load) {
                    displayMap(0);
                }
            });
}

void MapPage::displayMap(int index) {
    if (index == -1) {
        return;
    }
    if (index == 0) {
        // 显示所有
        auto orders = mainWindow->orderManager.getOrders();
        for (auto order : orders) {
            if (order.getStatus() == "已支付") {
                Flight flight = order.getFlight();
                QString depCity = flight.getDepartureCity();
                QString arrCity = flight.getArrivalCity();
                departureCity_x = cityCoordinates[depCity].first;
                departureCity_y = cityCoordinates[depCity].second;
                arrivalCity_x = cityCoordinates[arrCity].first;
                arrivalCity_y = cityCoordinates[arrCity].second;
                QString jsCode =
                    QString("drawGeodesicLineAll(%1, %2, %3, %4, '%5');")
                        .arg(departureCity_x)
                        .arg(departureCity_y)
                        .arg(arrivalCity_x)
                        .arg(arrivalCity_y)
                        .arg(flight.getAirline() + flight.getFlightNumber());

                ui->webEngineView->page()->runJavaScript(
                    jsCode, [](const QVariant &result) {
                        std::cout << "JavaScript execution result:"
                                  << result.toString().toStdString();
                    });

                if(order.getType() == Order::OrderType::TRANSFER) {
                    Flight flight2 = order.getFlight2();
                    QString depCity2 = flight2.getDepartureCity();
                    QString arrCity2 = flight2.getArrivalCity();
                    departureCity_x = cityCoordinates[depCity2].first;
                    departureCity_y = cityCoordinates[depCity2].second;
                    arrivalCity_x = cityCoordinates[arrCity2].first;
                    arrivalCity_y = cityCoordinates[arrCity2].second;
                    QString jsCode2 = QString("drawGeodesicLineAll(%1, %2, %3, %4, '%5');")
                                          .arg(departureCity_x)
                                          .arg(departureCity_y)
                                          .arg(arrivalCity_x)
                                          .arg(arrivalCity_y)
                                          .arg(flight2.getAirline() + flight2.getFlightNumber());

                    ui->webEngineView->page()->runJavaScript(
                        jsCode2, [](const QVariant &result) {
                            std::cout << "JavaScript execution result:"
                                      << result.toString().toStdString();
                        });
                }
            }
        }
    } else {
        QList<Order> copyorder = mainWindow->orderManager.getOrders();
        QList<Order> filterorders;
        std::copy_if(copyorder.begin(), copyorder.end(),
                     std::back_inserter(filterorders),
                     [](Order order) { return order.getStatus() == "已支付"; });
        Flight flight = filterorders[index - 1].getFlight();
        QString depCity = flight.getDepartureCity();
        QString arrCity = flight.getArrivalCity();
        departureCity_x = cityCoordinates[depCity].first;
        departureCity_y = cityCoordinates[depCity].second;
        arrivalCity_x = cityCoordinates[arrCity].first;
        arrivalCity_y = cityCoordinates[arrCity].second;
        QString jsCode = QString("drawGeodesicLine(%1, %2, %3, %4, '%5');")
                             .arg(departureCity_x)
                             .arg(departureCity_y)
                             .arg(arrivalCity_x)
                             .arg(arrivalCity_y)
                             .arg(flight.getAirline() + flight.getFlightNumber());

        ui->webEngineView->page()->runJavaScript(
            jsCode, [](const QVariant &result) {
                std::cout << "JavaScript execution result:"
                          << result.toString().toStdString();
            });

        if(filterorders[index - 1].getType() == Order::OrderType::TRANSFER)
        {
            Flight flight2 = filterorders[index - 1].getFlight2();
            QString depCity2 = flight2.getDepartureCity();
            QString arrCity2 = flight2.getArrivalCity();
            departureCity_x = cityCoordinates[depCity2].first;
            departureCity_y = cityCoordinates[depCity2].second;
            arrivalCity_x = cityCoordinates[arrCity2].first;
            arrivalCity_y = cityCoordinates[arrCity2].second;
            QString jsCode2 = QString("drawGeodesicLineAll(%1, %2, %3, %4, '%5');")
                                  .arg(departureCity_x)
                                  .arg(departureCity_y)
                                  .arg(arrivalCity_x)
                                  .arg(arrivalCity_y)
                                  .arg(flight2.getAirline() + flight2.getFlightNumber());

            ui->webEngineView->page()->runJavaScript(
                jsCode2, [](const QVariant &result) {
                    std::cout << "JavaScript execution result:"
                              << result.toString().toStdString();
                });
        }
    }
}
