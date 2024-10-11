#include "pages/SearchPage.h"
#include <QEvent.h>
#include "ui_searchpage.h"
#include <QMessageBox>

SearchPage::SearchPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::SearchPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    initSearchPage();

    connect(ui->backBtn, SIGNAL(released()), this, SLOT(Back()));

    connect(ui->showCalendarBtn, SIGNAL(released()), this, SLOT(showCalendar()));
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateButtonWithDate(QDate)));

    initializeDepBox();
    connect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    initializeArrBox();
    connect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));

    connect(ui->exchangeBtn, SIGNAL(released()), this, SLOT(Exchange()));

    connect(ui->searchBtn, SIGNAL(released()),this,SLOT(directSearch()));// 直飞的查询
    connect(ui->searchTransBtn, SIGNAL(released()),this,SLOT(tranSearch()));// 加入转机操作的查询

}

SearchPage::~SearchPage() {
    delete ui;
}

void SearchPage::initSearchPage() {
    mainWindow->network.clearData();
    mainWindow->network.readFlightFromFile(FLIGHT_FILE);
    //配置日历
    ui->calendarWidget->setParent(this);
    ui->calendarWidget->hide();
    // 根据内容填写出发地和目的地
    connect(ui->userhintcomb, SIGNAL(currentIndexChanged(int)), this, SLOT(setUserHint(int)));
    initHint();

}

void SearchPage::directSearch() {
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        QMessageBox::warning(this, "提示", "请选择出发日期");
        return;
    }
    if(ui->depBox->currentIndex() == ui->arrBox->currentIndex()) {
        QMessageBox::warning(this, "提示", "出发城市和到达城市相同");
    }
    searchFlights();
    mainWindow->showInfoPage();
}

void SearchPage::tranSearch() {
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        QMessageBox::warning(this, "提示", "请选择出发日期");
        return;
    }
    if(ui->depBox->currentIndex() == ui->arrBox->currentIndex()) {
        QMessageBox::warning(this, "提示", "出发城市和到达城市相同");
    }
    searchFlightsWithTransfers();
    mainWindow->showInfoPage();
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
        ui->depBox->setCurrentIndex(0); // 默认选项
    }
    mainWindow->depCity = ui->depBox->itemText(0);
}

void SearchPage::initializeArrBox() {
    QVector<QString> cityNames = mainWindow->network.getAllCityNames();
    for (const QString &cityName : cityNames) {
        ui->arrBox->addItem(cityName);
    }
    if (!cityNames.isEmpty()) {
        ui->arrBox->setCurrentIndex(0); // 默认选项
    }
    mainWindow->arrCity = ui->arrBox->itemText(0);
}
void SearchPage::showCalendar() {
    ui->calendarWidget->show();
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);  // 隐藏列标题
    ui->calendarWidget->raise();
    ui->calendarWidget->activateWindow();
}

void SearchPage::updateButtonWithDate(const QDate &date) {
    QString dateStr = date.toString("yyyy-MM-dd"); // 格式化日期
    ui->showCalendarBtn->setText(dateStr); // 将日期设置为按钮的文本
    ui->showCalendarBtn->show(); // 确保按钮是可见的
    qDebug()<<"出发日期："<<dateStr;
    mainWindow->selectedDate = date;
}

void SearchPage::mousePressEvent(QMouseEvent *event) {
    if (!ui->calendarWidget->isHidden() && !ui->calendarWidget->geometry().contains(event->pos())) {
        ui->calendarWidget->hide();
    }
    QWidget::mousePressEvent(event); // 调用基类的mousePressEvent
}
void SearchPage::getDep(int index) {
    QString selectedOption = ui->depBox->itemText(index); // 获取选择的文本
    qDebug() << "Selected departure option:" << selectedOption;
    mainWindow->depCity = selectedOption;
}

void SearchPage::getArr(int index) {
    QString selectedOption = ui->arrBox->itemText(index); // 获取选择的文本
    qDebug() << "Selected arrival option:" << selectedOption;
    mainWindow->arrCity = selectedOption;
}

void SearchPage::Exchange() {
    int currentIndex = ui->depBox->currentIndex();
    ui->depBox->setCurrentIndex(ui->arrBox->currentIndex());
    ui->arrBox->setCurrentIndex(currentIndex);
    mainWindow->depCity = ui->depBox->itemText(ui->arrBox->currentIndex());
    mainWindow->arrCity = ui->arrBox->itemText(currentIndex);
}

void SearchPage::searchFlights() {
    QString depCity = mainWindow->depCity;
    QString arrCity = mainWindow->arrCity;
    QDate selectedDate = mainWindow->selectedDate;
    qDebug()<<depCity<<" "<<arrCity<<" "<<selectedDate;
    QVector<Flight> flights = mainWindow->network.searchFlights(depCity, arrCity, selectedDate);
    qDebug() << "找到" << flights.size() << "个匹配的航程";
    mainWindow->searchType = Flight_Ticket_Management_System::DIRECT;
    mainWindow->searchedFlights = flights;
}

void SearchPage::searchFlightsWithTransfers() {
    QString depCity = mainWindow->depCity;
    QString arrCity = mainWindow->arrCity;
    QDate selectedDate = mainWindow->selectedDate;

    QVector<QPair<Flight, Flight>> flights = mainWindow->network.findTransferFlight(depCity, arrCity, selectedDate);
    QVector<Flight> directFlights;
    for (auto flight : flights) {
        directFlights.push_back(flight.first);
        directFlights.push_back(flight.second);
    }
    mainWindow->searchType = Flight_Ticket_Management_System::TRANSFER;
    // 更新表格显示
    qDebug() << "找到" << flights.size() << "个匹配的航程";
    mainWindow->searchedFlights = directFlights;
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

void SearchPage::setUserHint(int index) {
    if(index < 0) {
        return;
    }
    QString hint = ui->userhintcomb->itemText(index);
    QStringList cityNames = hint.split("->");
    ui->depBox->setCurrentText(cityNames[0]);
    ui->arrBox->setCurrentText(cityNames[1]);
}

void SearchPage::initHint() {
    ui->depBox->setCurrentIndex(0);
    ui->arrBox->setCurrentIndex(0);
    QDate today = QDate::currentDate();
    ui->calendarWidget->setSelectedDate(today);
    ui->showCalendarBtn->setText("选择出发日期");

    // 根据用户的订单来给用户一些出发地和到达地的提示
    QVector<QPair<QString, QString>> cityPairs;
    for (auto order : mainWindow->orderManager.getOrders()) {
        // 添加城市
        QPair<QString, QString> cityPair;
        cityPair.first = order.getFlight().getDepartureCity();
        cityPair.second = order.getFlight().getArrivalCity();
        if (!cityPairs.contains(cityPair)) {
            cityPairs.push_back(cityPair);
        }
    }
    ui->userhintcomb->clear();
    // 在界面添加label
    for (auto citypair : cityPairs) {
        QString hint1 =citypair.first + "->" + citypair.second;
        ui->userhintcomb->addItem(hint1);
        QString hint2 =citypair.second + "->" + citypair.first;
        ui->userhintcomb->addItem(hint2);
    }
}



