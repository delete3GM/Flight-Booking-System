#include "pages/SearchPage.h"
#include <QEvent.h>
#include "ui_searchpage.h"
#include "widgets/CitySelectionDialog.h"
#include "widgets/SearchableComboBox.h"
#include <QMessageBox>
#include <QProcess>
#include <QSocketNotifier>

SearchPage::SearchPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::SearchPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initSearchPage();

    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateButtonWithDate(QDate)));
    connect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    connect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));

    connect(ui->backBtn, SIGNAL(released()), this, SLOT(Back()));
    connect(ui->exchangeBtn, SIGNAL(released()), this, SLOT(Exchange()));
    connect(ui->searchBtn, SIGNAL(released()),this,SLOT(searchFlights()));
    connect(ui->showCalendarBtn, SIGNAL(released()), this, SLOT(showCalendar()));

    //connect(ui->depBtn, SIGNAL(clicked()), this, SLOT(depBox_clicked()));

    //connect(depComboBox, &SearchableComboBox::currentIndexChanged, this, &SearchPage::getDep);
    //connect(arrComboBox, &SearchableComboBox::currentIndexChanged, this, &SearchPage::getArr);
}

SearchPage::~SearchPage() {
    delete ui;
}

void SearchPage::initSearchPage() {
    //depComboBox = new SearchableComboBox(this);
    //arrComboBox = new SearchableComboBox(this);
    //depComboBox->setGeometry(50, 50, 200, 80); // x, y, width, height
    //arrComboBox->setGeometry(150, 100, 200, 80); // x, y, width, height
    mainWindow->network.clearData();
    mainWindow->network.readFlightFromFile(FLIGHT_FILE);
    ui->calendarWidget->hide();

    initializeDepBox();
    initializeArrBox();

    updateDepWeather();
    updateArrWeather();

    //loadCitiesIntoSearchableBoxes();
}

void SearchPage::loadCitiesIntoSearchableBoxes() {
    // QVector<QString> cityNames = mainWindow->network.getAllCityNames();
    // for (const QString &cityName : cityNames) {
    //     qDebug()<<"11";
    //     depComboBox->addItem(cityName);
    //     qDebug()<<"11";
    //     arrComboBox->addItem(cityName);
    //     qDebug()<<cityName;
    // }
    // qDebug()<<"agreer";
    // if (!cityNames.isEmpty()) {
    //     depComboBox->setCurrentIndex(0);
    //     arrComboBox->setCurrentIndex(0);
    // }
    // qDebug()<<"agreer";
    // mainWindow->depCity = depComboBox->currentText();
    // mainWindow->arrCity = arrComboBox->currentText();
}

void SearchPage::updateDepWeather() {
    if (mainWindow->cityInfoMap.contains(mainWindow->depCity)) {
        CityInfo info = mainWindow->cityInfoMap.value(mainWindow->depCity);
        QString weatherInfo = QString("城市: %1\n天气: %2\n温度: %3℃\n风力: %4级")
                                  .arg(mainWindow->depCity)
                                  .arg(info.weather)
                                  .arg(info.temperature)
                                  .arg(info.wind);
        ui->depWeather->setText(weatherInfo);
    } else {
        //ui->depWeather->setText("暂不支持国外城市");
    }
}

void SearchPage::updateArrWeather() {
    if (mainWindow->cityInfoMap.contains(mainWindow->arrCity)) {
        CityInfo info = mainWindow->cityInfoMap.value(mainWindow->arrCity);
        QString weatherInfo = QString("城市: %1\n天气: %2\n温度: %3℃\n风力: %4级")
                                  .arg(mainWindow->arrCity)
                                  .arg(info.weather)
                                  .arg(info.temperature)
                                  .arg(info.wind);
        ui->arrWeather->setText(weatherInfo);
    } else {
        //ui->arrWeather->setText("暂不支持国外城市");
    }
}

void SearchPage::Back() {
    mainWindow->showMenuPage();
}

void SearchPage::initializeDepBox() {
    QVector<QString> cityNames = mainWindow->network.getAllCityNames();
    for (const QString &cityName : cityNames) {
        ui->depBox->addItem(cityName);
    }
    if (!cityNames.isEmpty()) {
        ui->depBox->setCurrentIndex(0);
    }
    mainWindow->depCity = ui->depBox->itemText(0);
    //mainWindow->cities = mainWindow->network.getAllCityNames();
}

void SearchPage::initializeArrBox() {
    QVector<QString> cityNames = mainWindow->network.getAllCityNames();
    for (const QString &cityName : cityNames) {
        ui->arrBox->addItem(cityName);
    }
    if (!cityNames.isEmpty()) {
        ui->arrBox->setCurrentIndex(0);
    }
    mainWindow->arrCity = ui->arrBox->itemText(0);
}


void SearchPage::depBox_clicked() {
    // // 创建并显示CitySelectionDialog
    // CitySelectionDialog dialog(mainWindow->network.getAllCityNames(), this);
    // // 获取按钮的位置
    // QPoint buttonPos = ui->depBtn->mapToGlobal(QPoint(0, 0));

    // dialog.move(buttonPos.x(), buttonPos.y() + ui->depBtn->height());

    // connect(&dialog, &CitySelectionDialog::citySelected, this, &SearchPage::updateDepCity);
    // dialog.exec();
}

void SearchPage::updateDepCity(const QString& city) {
    mainWindow->depCity = city;
    ui->depBox->setCurrentText(city);
    updateDepWeather();
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
    qDebug()<<"出发日期："<<dateStr;
    mainWindow->selectedDate = date;
}

void SearchPage::mousePressEvent(QMouseEvent *event) {
    if (!ui->calendarWidget->isHidden() && !ui->calendarWidget->geometry().contains(event->pos())) {
        ui->calendarWidget->hide();
    }
    QWidget::mousePressEvent(event);
}

void SearchPage::getDep(int index) {
    QString selectedOption = ui->depBox->itemText(index);
    qDebug() << "Selected departure option:" << selectedOption;
    mainWindow->depCity = selectedOption;
    updateDepWeather();
}

void SearchPage::getArr(int index) {
    QString selectedOption = ui->arrBox->itemText(index);
    qDebug() << "Selected arrival option:" << selectedOption;
    mainWindow->arrCity = selectedOption;
    updateArrWeather();
}

void SearchPage::Exchange() {
    disconnect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    disconnect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));

    int currentIndex = ui->depBox->currentIndex();
    ui->depBox->setCurrentIndex(ui->arrBox->currentIndex());
    ui->arrBox->setCurrentIndex(currentIndex);
    mainWindow->depCity = ui->depBox->itemText(ui->arrBox->currentIndex());
    mainWindow->arrCity = ui->arrBox->itemText(currentIndex);

    QString tempWeather = ui->depWeather->text();
    ui->depWeather->setText(ui->arrWeather->text());
    ui->arrWeather->setText(tempWeather);

    connect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    connect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));
}

void SearchPage::searchFlights() {
    if(ui->depBox->currentIndex() == ui->arrBox->currentIndex()) {
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
    qDebug()<<depCity<<" "<<arrCity<<" "<<selectedDate;
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
    loadCitiesIntoComboBox(ui->depBox, mainWindow->network.getAllCityNames());
    loadCitiesIntoComboBox(ui->arrBox, mainWindow->network.getAllCityNames());
    ui->depBox->setCurrentText(dep);
    ui->arrBox->setCurrentText(arr);
    ui->calendarWidget->setSelectedDate(date);
    ui->showCalendarBtn->setText("");
    ui->showCalendarBtn->setText(date.toString("yyyy-MM-dd"));
}


