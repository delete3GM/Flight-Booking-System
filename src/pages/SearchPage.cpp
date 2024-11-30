#include "pages/SearchPage.h"
#include <QEvent.h>
#include "qtoolbutton.h"
#include "qwebengineview.h"
#include "ui_searchpage.h"
#include "widgets/CabinClassBookingDialog.h"
#include <QMessageBox>
#include <QProcess>
#include <QSocketNotifier>

SearchPage::SearchPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::SearchPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initSearchPage();

    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateButtonWithDate(QDate)));

    connect(ui->backBtn, SIGNAL(released()), this, SLOT(Back()));
    connect(ui->exchangeBtn, SIGNAL(released()), this, SLOT(Exchange()));
    connect(ui->searchBtn, SIGNAL(released()),this,SLOT(searchFlights()));
    connect(ui->showCalendarBtn, SIGNAL(released()), this, SLOT(showCalendar()));

}

SearchPage::~SearchPage() {
    delete ui;
}

void SearchPage::initSearchPage() {

    mainWindow->network.clearData();
    mainWindow->network.readFlightFromFile(FLIGHT_FILE);
    ui->calendarWidget->hide();

    mainWindow->currentUser.updateUserWeight();
    ui->backBtn->setIcon(QIcon(":/images/resources/images/back.png"));
    ui->backBtn->setIconSize(QSize(32, 32));
    ui->backBtn->setStyleSheet("QToolButton { border: none; background: none; }");
    ui->backBtn->setAutoRaise(true);

    ui->exchangeBtn->setFixedSize(32, 32);
    QIcon icon(":/images/resources/images/exchange.png");
    ui->exchangeBtn->setIcon(icon);
    ui->exchangeBtn->setIconSize(QSize(24, 24));
    ui->exchangeBtn->setStyleSheet(R"(
        QPushButton {
            border-style: outset;
            border-width: 2px;
            border-radius: 5px;
            border-color: #8B7355;
        }
    )");

    ui->graphLbl->setText("<a href='#'>查看航线图</a>");
    connect(ui->graphLbl, &QLabel::linkActivated, this, &SearchPage::showGraph);

    depComboBox = new SearchableComboBox(this);
    arrComboBox = new SearchableComboBox(this);
    QVector<QString> cityNames = mainWindow->network.getAllCityNames();
    depComboBox->initializeBox(cityNames, mainWindow->cityInfoMap);
    arrComboBox->initializeBox(cityNames, mainWindow->cityInfoMap);
    QRect depGeometry(50, 80, 140, 40);
    QRect arrGeometry(280, 80, 140, 40);
    depComboBox->setGeometry(depGeometry);
    arrComboBox->setGeometry(arrGeometry);

    connect(depComboBox, &SearchableComboBox::citySelected, this, [this](const QString&city) {
        mainWindow->depCity = city;
        updateCityWeather(ui->depWeatherLbl, city);
    });
    connect(arrComboBox, &SearchableComboBox::citySelected, this, [this](const QString&city) {
        mainWindow->arrCity = city;
        updateCityWeather(ui->arrWeatherLbl, city);
    });

    if (!cityNames.isEmpty()) {
        QString firstCity = cityNames.first();
        depComboBox->setCurrentText(firstCity);
        arrComboBox->setCurrentText(firstCity);

        mainWindow->depCity = firstCity;
        mainWindow->arrCity = firstCity;

        updateCityWeather(ui->depWeatherLbl, firstCity);
        updateCityWeather(ui->arrWeatherLbl, firstCity);
    }
    displayRecommendation();
}

void SearchPage::Back() {
    mainWindow->showMenuPage();
}

void SearchPage::updateCityWeather(QLabel* weatherLabel, const QString& cityName) {
    QLayoutItem* child;
    QLayout* existingLayout = weatherLabel->layout();

    if (existingLayout) {
        while ((child = existingLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                delete child->widget();
            }
            delete child;
        }
        delete existingLayout;
    }

    weatherLabel->clear();

    if (mainWindow->cityInfoMap.contains(cityName)) {
        CityInfo info = mainWindow->cityInfoMap.value(cityName);

        QVBoxLayout* layout = new QVBoxLayout(weatherLabel);
        layout->setContentsMargins(0, 0, 0, 0);

        QString weatherInfo = QString("<span style='font-weight:bold; font-size:14px;'>%1</span>  %2<br>温度: %3℃<br>风力%4级")
                                  .arg(cityName)
                                  .arg(info.weather)
                                  .arg(info.temperature)
                                  .arg(info.wind);

        QPixmap weatherPixmap;
        if (info.weather == "晴") {
            weatherPixmap = QPixmap(":/images/resources/images/sun_icon.png");
        } else if (info.weather == "小雨" || info.weather == "中雨" || info.weather == "大雨") {
            weatherPixmap = QPixmap(":/images/resources/images/rain_icon.png");
        } else if (info.weather == "阴" || info.weather == "多云") {
            weatherPixmap = QPixmap(":/images/resources/images/cloud_icon.png");
        } else if(info.weather == "雾" || info.weather == "霾") {
            weatherPixmap = QPixmap(":/images/resources/images/smog_icon.png");
        } else if(info.weather == "小雪" || info.weather == "中雪" || info.weather == "大雪") {
            weatherPixmap = QPixmap(":/images/resources/images/snow_icon.png");
        }

        QLabel* iconLabel = new QLabel(weatherLabel);
        iconLabel->setStyleSheet("QLabel { border: none; background-color: transparent; }");
        iconLabel->setPixmap(weatherPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(iconLabel);

        QLabel* textLabel = new QLabel(weatherInfo, weatherLabel);
        textLabel->setTextFormat(Qt::RichText);
        textLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(textLabel);

        weatherLabel->setLayout(layout);
    }
}

void SearchPage::showCalendar() {
    ui->calendarWidget->show();
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->raise();
    ui->calendarWidget->activateWindow();
}

void SearchPage::updateButtonWithDate(const QDate &date) {
    QString dateStr = date.toString("yyyy-MM-dd");
    ui->showCalendarBtn->setText(dateStr);
    ui->showCalendarBtn->show();
    mainWindow->selectedDate = date;
}

void SearchPage::mousePressEvent(QMouseEvent *event) {
    if (!ui->calendarWidget->isHidden() && !ui->calendarWidget->geometry().contains(event->pos())) {
        ui->calendarWidget->hide();
    }
    QWidget::mousePressEvent(event);
}

void SearchPage::Exchange() {
    int currentIndex = depComboBox->currentIndex();
    depComboBox->setCurrentIndex(arrComboBox->currentIndex());
    arrComboBox->setCurrentIndex(currentIndex);
    mainWindow->depCity = depComboBox->itemText(arrComboBox->currentIndex());
    mainWindow->arrCity = arrComboBox->itemText(currentIndex);

    QString tempWeather = ui->depWeatherLbl->text();
    ui->depWeatherLbl->setText(ui->arrWeatherLbl->text());
    ui->arrWeatherLbl->setText(tempWeather);
}

void SearchPage::searchFlights() {
    if(depComboBox->currentIndex() == arrComboBox->currentIndex()) {
        QMessageBox::warning(this, "提示", "出发城市和到达城市相同");
        return;
    }
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        QMessageBox::warning(this, "提示", "请选择出发日期");
        return;
    }
    QString depCity = mainWindow->depCity;
    QString arrCity = mainWindow->arrCity;
    QDate selectedDate = mainWindow->selectedDate;
    QVector<FlightRoute> flights = mainWindow->network.searchFlightsDFS(depCity, arrCity, selectedDate);
    qDebug() << "找到" << flights.size() << "个匹配的航程";
    mainWindow->searchedFlights = flights;
    mainWindow->showInfoPage();
}

void SearchPage::loadCitiesIntoComboBox(QComboBox* comboBox, const QVector<QString>& cityNames) {
    comboBox->clear();
    for (const QString &cityName : cityNames) {
        comboBox->addItem(cityName);
    }
}

void SearchPage::setDataForReschedule(const QString& dep, const QString& arr, const QDate& date) {
    loadCitiesIntoComboBox(depComboBox, mainWindow->network.getAllCityNames());
    loadCitiesIntoComboBox(arrComboBox, mainWindow->network.getAllCityNames());
    depComboBox->setCurrentText(dep);
    arrComboBox->setCurrentText(arr);
    ui->calendarWidget->setSelectedDate(date);
    ui->showCalendarBtn->setText("");
    ui->showCalendarBtn->setText(date.toString("yyyy-MM-dd"));
}

void SearchPage::loadRecommendation() {
    QVector<QString> cityList = mainWindow->currentUser.getFrequentCities();
    QVector<FlightRoute> tempRecommendation = mainWindow->network.searchRecommendation(cityList);
    mainWindow->recommendationFlights = mainWindow->network.sortFlights(
        mainWindow->currentUser, tempRecommendation, SORT_BY_PERSON);
}

void SearchPage::displayRecommendation() {
    loadRecommendation();
    QVector<FlightRoute> flights = mainWindow->recommendationFlights;

    ui->recTree->clear();
    ui->recTree->setColumnCount(6);
    ui->recTree->setHeaderLabels({"  城市路径", "起飞时间", "降落时间", "总用时", "总票价", "操作"});
    ui->recTree->setUpdatesEnabled(false);
    ui->recTree->header()->setStyleSheet("QHeaderView::section { border: none; }");

    int row = 0;
    for (auto& flightPath : flights) {
        addFlightPathToTree(ui->recTree, flightPath, row);
    }
    ui->recTree->setColumnWidth(0, 130);
    ui->recTree->setColumnWidth(1, 75);
    ui->recTree->setColumnWidth(2, 75);
    ui->recTree->setColumnWidth(3, 70);
    ui->recTree->setColumnWidth(4, 60);
    ui->recTree->setColumnWidth(5, 40);
    ui->recTree->setUpdatesEnabled(true);
}

void SearchPage::addFlightPathToTree(QTreeWidget* treeWidget, const FlightRoute& flightPath, int& row) {
    QString cityPath = flightPath.first()->getDepartureCity()+"->"+flightPath.last()->getArrivalCity();

    QString departureTime0 = flightPath.first()->getDepartureTime();
    QDateTime depDateTime = QDateTime::fromString(departureTime0, "yyyy-MM-dd HH:mm");
    QString departureDate = depDateTime.date().toString("yyyy-MM-dd");
    QString departureDateTime = depDateTime.time().toString("HH:mm");

    QString arrivalTime0 = flightPath.last()->getArrivalTime();
    QDateTime arrDateTime = QDateTime::fromString(arrivalTime0, "yyyy-MM-dd HH:mm");
    QString arrivalDate = arrDateTime.date().toString("yyyy-MM-dd");
    QString arrivalDateTime = arrDateTime.time().toString("HH:mm");

    QString departureTime = departureDate + "\n" + departureDateTime;
    QString arrivalTime = arrivalDate + "\n" + arrivalDateTime;

    qint64 durationMins = flightPath.getTotalDuration();
    int hours = durationMins / 60;
    int minutes = durationMins % 60;
    QString totalDuration = QString("%1h %2min").arg(hours).arg(minutes);
    if(flightPath.getFlightCount() == 1) totalDuration += "\n(直)";
    else totalDuration += "\n(转)";

    QString totalPrice = QString::number(flightPath.getTotalPrice(), 'f', 2);

    QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
    item->setText(0, cityPath);
    item->setText(1, departureTime);
    item->setText(2, arrivalTime);
    item->setText(3, totalDuration);
    item->setText(4, totalPrice);

    QPushButton* bookButton = new QPushButton("去订票", treeWidget);
    bookButton->setProperty("row", row);
    connect(bookButton, &QPushButton::clicked, this, [this, row]() {
        onBookButtonClicked(row);
    });
    treeWidget->setItemWidget(item, 5, bookButton);
    row++;
}

void SearchPage::onBookButtonClicked(int row) {
    qDebug() << "Booking ticket for row:" << row;
    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    CabinClassBookingDialog dialog(mainWindow->recommendationFlights[row], discount, this);
    connect(&dialog, &CabinClassBookingDialog::cabinClassSelected, this, [ row, this](const QString& cabinClass) {
        handleCabinClassSelected(cabinClass, row);
    });
    if (dialog.exec() == QDialog::Accepted) {
        mainWindow->selectedFlight = mainWindow->recommendationFlights[row];
        mainWindow->showCheckoutPage();
    }
}

void SearchPage::handleCabinClassSelected(const QString& cabinClass, int row) {
    mainWindow->selectedClass = cabinClass;
    mainWindow->selectedFlight = mainWindow->recommendationFlights[row];
    mainWindow->showCheckoutPage();
}

void SearchPage::showGraph() {
    QWebEngineView *webEngineView = new QWebEngineView(this);
    webEngineView->setUrl(QUrl::fromLocalFile(
        "D:/CS/projects/Flight_Ticket_Management_System/web/html/domestic_routes.html"));

    configWebEngine(webEngineView);

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("航线图");
    dialog->resize(800, 500);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(webEngineView);

    connect(webEngineView, &QWebEngineView::loadFinished, this, [=](bool ok) {
        if (ok) {
            webEngineView->page()->runJavaScript("clearGeodesicLines()");

            QString depCity = mainWindow->depCity;
            City* departureCity = nullptr;
            for (City* city : mainWindow->network.getCities()) {
                if (city->name == depCity) {
                    departureCity = city;
                    break;
                }
            }
            FlightNode* currentNode = departureCity->flightsHead;
            while (currentNode != nullptr) {
                std::shared_ptr<Flight> flight = currentNode->flight;

                QString arrCity = flight->getArrivalCity();

                const CityInfo& departureInfo = mainWindow->cityInfoMap.value(depCity, {0, 0, "", "", ""});
                const CityInfo& arrivalInfo = mainWindow->cityInfoMap.value(arrCity, {0, 0, "", "", ""});
                double depCity_x = departureInfo.latitude;
                double depCity_y = departureInfo.longitude;
                double arrCity_x = arrivalInfo.latitude;
                double arrCity_y = arrivalInfo.longitude;
                QString flightInfo = flight->getAirline() + " " + flight->getFlightNumber();

                QString jsCode = QString("drawGeodesicLine(%1, %2, %3, %4, '%5', '%6');")
                                     .arg(depCity_x)
                                     .arg(depCity_y)
                                     .arg(arrCity_x)
                                     .arg(arrCity_y)
                                     .arg(flightInfo)
                                     .arg("END_ONLY");


                webEngineView->page()->runJavaScript(jsCode);

                currentNode = currentNode->next;
            }
        }
    });

    dialog->exec();
}

