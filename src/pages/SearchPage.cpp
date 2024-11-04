#include "pages/SearchPage.h"
#include <QEvent.h>
#include "ui_searchpage.h"
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
}

SearchPage::~SearchPage() {
    delete ui;
}

void SearchPage::initSearchPage() {
    mainWindow->network.clearData();
    mainWindow->network.readFlightFromFile(FLIGHT_FILE);
    initializeDepBox();
    initializeArrBox();
    ui->calendarWidget->hide();

    connect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateDepWeather);
    callPythonScript(mainWindow->depCity);

    connect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateArrWeather);
    callPythonScript(mainWindow->arrCity);
}

void SearchPage::callPythonScript(const QString &city) {
    QProcess *process = new QProcess(this);
    QString program = "D:/CS/projects/Flight_Ticket_Management_System/scripts/weather.exe";
    QStringList arguments;
    arguments << city;

    process->start(program, arguments);

    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
        QByteArray outputBytes = process->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(outputBytes);
        qDebug() << "Output from Python executable:" << output;

        emit pythonScriptOutputReceived(output);
    });
    connect(process, &QProcess::finished, this, [process]() {
        process->deleteLater();
    });
}

void SearchPage::updateDepWeather(const QString &output) {
    ui->depWeather->setText(output);
}

void SearchPage::updateArrWeather(const QString &output) {
    ui->arrWeather->setText(output);
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
    QString cityName = mainWindow->depCity;
    disconnect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateArrWeather);
    connect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateDepWeather);

    callPythonScript(cityName);
}

void SearchPage::getArr(int index) {
    QString selectedOption = ui->arrBox->itemText(index);
    qDebug() << "Selected arrival option:" << selectedOption;
    mainWindow->arrCity = selectedOption;
    QString cityName = mainWindow->arrCity;
    disconnect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateDepWeather);
    connect(this, &SearchPage::pythonScriptOutputReceived, this, &SearchPage::updateArrWeather);
    callPythonScript(cityName);
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

