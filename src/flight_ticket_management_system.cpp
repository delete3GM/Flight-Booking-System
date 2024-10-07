#include "flight_ticket_management_system.h"
#include "Flight.h"
#include "ui_flight_ticket_management_system.h"
#include "FlightNetwork.h"
#include "User.h"
#include "Order.h"
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
#include <iostream>
#include <qlist.h>
#include <qwebengineview.h>



Flight_Ticket_Management_System::Flight_Ticket_Management_System(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Flight_Ticket_Management_System)
{
    ui->setupUi(this);


    // 初始界面显示登录界面
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

    //登陆界面
    initLogin();
    connect(ui->loginBtn, SIGNAL(released()), this, SLOT(Login()));
    //connect(ui->regisBtn, SIGNAL(), this, );

    //切换菜单
    connect(ui->Orders, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

    //配置日历
    ui->calendarWidget->setParent(ui->menuPage);
    ui->calendarWidget->hide();
    connect(ui->showCalendarBtn, SIGNAL(released()), this, SLOT(showCalendar()));
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateButtonWithDate(QDate)));

    //从文件读取数据到图中
    network.readData(FLIGHT_FILE);

    //初始化comboBox
    initializeDepBox();
    connect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    initializeArrBox();
    connect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));

    //交换出发到达城市
    connect(ui->exchangeBtn, SIGNAL(released()), this, SLOT(Exchange()));

    // 搜索
    connect(ui->searchBtn, SIGNAL(released()),this,SLOT(Menu2Info()));
    connect(ui->searchBtn, SIGNAL(released()), this, SLOT(searchFlights())); // 直飞的查询
    connect(ui->searchTransBtn, SIGNAL(released()),this,SLOT(Menu2Info()));
    connect(ui->searchTransBtn, SIGNAL(released()), this, SLOT(searchFlightsWithTransfers())); // 加入转机操作的查询

    //按条件排序
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->default_sort, 1);
    buttonGroup->addButton(ui->time_sort, 2);
    buttonGroup->addButton(ui->early_sort, 3);
    buttonGroup->addButton(ui->price_sort, 4);
    ui->default_sort->setChecked(true);
    buttonGroup->setExclusive(true);
    connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(updateSearch(int)));

    //填写乘客信息
    connect(ui->submitBtn, SIGNAL(released()), this, SLOT(addPassenger()));
    connect(ui->cancelBtn, SIGNAL(released()), this, SLOT(toInfo()));

    // 根据内容填写出发地和目的地
    connect(ui->userhintcomb, SIGNAL(currentIndexChanged(int)), this, SLOT(setUserHint(int)));

    //页面跳转
    connect(ui->menuBtn, SIGNAL(released()), this, SLOT(Start2Menu()));
    connect(ui->userBtn, SIGNAL(released()), this, SLOT(Start2User()));
    connect(ui->exitBtn, SIGNAL(released()),this,SLOT(Exit()));
    connect(ui->backBtn, SIGNAL(released()), this, SLOT(Back2Menu()));
    connect(ui->menu2startBtn, SIGNAL(released()), this, SLOT(toStart()));
    connect(ui->user2startBtn, SIGNAL(released()), this, SLOT(toStart()));
    connect(ui->backtoUser, SIGNAL(released()), this, SLOT(Start2User()));
    connect(ui->graphBtn, SIGNAL(released()), this, SLOT(showMap()));

    connect(ui->airlineComb, SIGNAL(currentIndexChanged(int)), this,
            SLOT(displayMap(int)));
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

    // 加载本地 HTML 文件
    ui->webEngineView->setUrl(QUrl::fromLocalFile(
        "D:/CS/projects/Flight_Ticket_Management_System/src/map.html"));

    
}

Flight_Ticket_Management_System::~Flight_Ticket_Management_System()
{
    delete ui;
}

void Flight_Ticket_Management_System::initLogin()
{
    ui->typeAcnt->setPlaceholderText("请输入身份证号");
    ui->typePsw->setPlaceholderText("请输入密码");
}

void Flight_Ticket_Management_System::loadUserOrders()
{
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

    orderManager.loadOrdersFromFile(filePath);
}

void Flight_Ticket_Management_System::Login()
{
    currentId = ui->typeAcnt->text();
    currentPsw = ui->typePsw->text();
    loadUserOrders();  // 加载用户订单
    toStart();
}

void Flight_Ticket_Management_System::onTabChanged(int index)
{
    QTabWidget *tabWidget = ui->Orders;
    if (index < tabWidget->count()) {
        if (tabWidget->tabText(index) == "全部") {
            qDebug()<<"all orders";
            displayOrders(tabWidget, "");
        }
        if (tabWidget->tabText(index) == "已出票") {
            qDebug()<<"available orders";
            displayOrders(tabWidget, "已支付");
        }
    } else {
        qDebug() << "Invalid tab index";
    }
}

void Flight_Ticket_Management_System::Menu2Info()
{
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        QMessageBox::warning(this, "提示", "请选择出发日期");
        return;
    } else {
        ui->stackedWidget->setCurrentWidget(ui->flightInfoPage);
        ui->calendarWidget->hide();
    }
}

void Flight_Ticket_Management_System::Back2Menu()
{
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
    ui->default_sort->setChecked(true);
}

void Flight_Ticket_Management_System::Start2Menu()
{
    ui->depBox->setCurrentIndex(0);
    ui->arrBox->setCurrentIndex(0);
    QDate today = QDate::currentDate();
    ui->calendarWidget->setSelectedDate(today);
    ui->showCalendarBtn->setText("选择出发日期");
    ui->stackedWidget->setCurrentWidget(ui->menuPage);

    // 根据用户的订单来给用户一些出发地和到达地的提示

    QVector<QPair<QString, QString>> cityPairs;
    for (auto order : orderManager.getOrders()) {
      // 添加城市
      QPair<QString, QString> cityPair;
      cityPair.first = order.getFlight().getDepartureCity();
      cityPair.second = order.getFlight().getArrivalCity();
      if (!cityPairs.contains(cityPair))
        cityPairs.push_back(cityPair);
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

void Flight_Ticket_Management_System::Start2User()
{
    ui->stackedWidget->setCurrentWidget(ui->userPage);
    loadUserOrders();  // 加载用户订单
    ui->Orders->setCurrentIndex(0);
    displayOrders(ui->Orders, "已支付");
}

void Flight_Ticket_Management_System::toStart()
{
    ui->stackedWidget->setCurrentWidget(ui->startPage);
}

void Flight_Ticket_Management_System::toInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->flightInfoPage);
}

void Flight_Ticket_Management_System::setUserHint(int index)
{
    if(index < 0) {
        return;
    }
    QString hint = ui->userhintcomb->itemText(index);
    QStringList cityNames = hint.split("->");
    ui->depBox->setCurrentText(cityNames[0]);
    ui->arrBox->setCurrentText(cityNames[1]);
}

void Flight_Ticket_Management_System::Exit()
{
    this->close();
}

void Flight_Ticket_Management_System::showCalendar()
{
    ui->calendarWidget->show();
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);  // 隐藏列标题
    ui->calendarWidget->raise();
    ui->calendarWidget->activateWindow();
}

void Flight_Ticket_Management_System::updateButtonWithDate(const QDate &date)
{

    QString dateStr = date.toString("yyyy-MM-dd"); // 格式化日期
    ui->showCalendarBtn->setText(dateStr); // 将日期设置为按钮的文本
    ui->showCalendarBtn->show(); // 确保按钮是可见的
    qDebug()<<"date changed : "<<dateStr;
}

void Flight_Ticket_Management_System::mousePressEvent(QMouseEvent *event)
{
    if (!ui->calendarWidget->isHidden() && !ui->calendarWidget->geometry().contains(event->pos())) {
        ui->calendarWidget->hide();
    }
    QMainWindow::mousePressEvent(event); // 调用基类的mousePressEvent
}

void Flight_Ticket_Management_System::initializeDepBox()
{
    QVector<QString> cityNames = network.getAllCityNames();
    for (const QString &cityName : cityNames) {
        ui->depBox->addItem(cityName);
    }
    if (!cityNames.isEmpty()) {
        ui->depBox->setCurrentIndex(0); // 默认选项
    }
}

void Flight_Ticket_Management_System::initializeArrBox()
{
    QVector<QString> cityNames = network.getAllCityNames();
    for (const QString &cityName : cityNames) {
        ui->arrBox->addItem(cityName);
    }
    if (!cityNames.isEmpty()) {
        ui->arrBox->setCurrentIndex(0); // 默认选项
    }
}

QString Flight_Ticket_Management_System::getDep(int index)
{
    QString selectedOption = ui->depBox->itemText(index); // 获取选择的文本
    qDebug() << "Selected departure option:" << selectedOption;
    return selectedOption;
}

QString Flight_Ticket_Management_System::getArr(int index)
{
    QString selectedOption = ui->arrBox->itemText(index); // 获取选择的文本
    qDebug() << "Selected arrival option:" << selectedOption;
    return selectedOption;
}

void Flight_Ticket_Management_System::Exchange()
{
    int currentIndex = ui->depBox->currentIndex();
    ui->depBox->setCurrentIndex(ui->arrBox->currentIndex());
    ui->arrBox->setCurrentIndex(currentIndex);
}

void Flight_Ticket_Management_System::searchFlights()
{
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        return;
    } else {
        QString depCity = getDep(ui->depBox->currentIndex());
        QString arrCity = getArr(ui->arrBox->currentIndex());
        QDate selectedDate = ui->calendarWidget->selectedDate();

        QVector<Flight> flights = network.searchFlights(depCity, arrCity, selectedDate);
        qDebug() << "Search results count:" << flights.size();
        searchType = DIRECT;
        updateTableWidget(flights);
    }
}

void Flight_Ticket_Management_System::updateSearch(int buttonId)
{
    QString depCity = getDep(ui->depBox->currentIndex());
    QString arrCity = getArr(ui->arrBox->currentIndex());
    QDate selectedDate = ui->calendarWidget->selectedDate();

  if(searchType == DIRECT)
  {
    // 搜索航班
    QVector<Flight> flights =
        network.searchFlights(depCity, arrCity, selectedDate);

    // 根据选中的按钮设置排序类型
    SORT_TYPE sortType = SORT_NORMAL;
    switch (buttonId) {
    case 1: // 默认排序
        sortType = SORT_NORMAL;
        qDebug() << "Sorting by normal";
        break;
    case 2: // 按时长排序
        sortType = SORT_BY_DURA;
        qDebug() << "Sorting by dura";
        break;
    case 3: // 按最早排序
        sortType = SORT_BY_TIME;
        qDebug() << "Sorting by time";
        break;
    case 4: // 按价格排序
        sortType = SORT_BY_PRICE;
        qDebug() << "Sorting by price";
        break;
    default:
        qDebug() << "Unknown button ID";
        return;
    }

    if (sortType != SORT_NORMAL) {
        flights = network.sortFlights(flights, sortType);
    }
    // 更新表格显示
    updateTableWidget(flights);
  }
  else if(searchType==TRANSFER)
  {
    QVector<QPair<Flight, Flight>> flights = network.findTransferFlight(depCity, arrCity, selectedDate);

    // 排序
    // 根据选中的按钮设置排序类型
    SORT_TYPE sortType = SORT_NORMAL;
    switch (buttonId) {
    case 1: // 默认排序
        sortType = SORT_NORMAL;
        qDebug() << "Sorting by normal";
        break;
    case 2: // 按时长排序
        sortType = SORT_BY_DURA;
        qDebug() << "Sorting by dura";
        break;
    case 3: // 按最早排序
        sortType = SORT_BY_TIME;
        qDebug() << "Sorting by time";
        break;
    case 4: // 按价格排序
        sortType = SORT_BY_PRICE;
        qDebug() << "Sorting by price";
        break;
    default:
        qDebug() << "Unknown button ID";
        return;
    }

    if (sortType != SORT_NORMAL) {
        flights = network.sortFlights(flights, sortType);
    }


    // 这里将 pair 转换为两个对象进行存储
    QVector<Flight> directFlights;
    for (auto flight : flights) {
      directFlights.push_back(flight.first);
      directFlights.push_back(flight.second);
    }
    qDebug() << "Search results count:" << flights.size();
    searchType = TRANSFER;
    updateTableWidget(directFlights);
  }
}

void Flight_Ticket_Management_System::searchFlightsWithTransfers()
{
    if (ui->showCalendarBtn->text().contains("选择出发日期")) {
        return;
    } else {
        QString depCity = getDep(ui->depBox->currentIndex());
        QString arrCity = getArr(ui->arrBox->currentIndex());
        QDate selectedDate = ui->calendarWidget->selectedDate();

        QVector<QPair<Flight, Flight>> flights =
            network.findTransferFlight(depCity, arrCity, selectedDate);
        QVector<Flight> directFlights;
        for (auto flight : flights) {
          directFlights.push_back(flight.first);
          directFlights.push_back(flight.second);
        }

        searchType = TRANSFER;
        // 更新表格显示
        qDebug() << "Search results count:" << flights.size();
        updateTableWidget(directFlights);
    }
}

void Flight_Ticket_Management_System::updateTableWidget(
    const QVector<Flight> &flights) {
  // 如果是直飞
  if (searchType == DIRECT) {
    QTableWidget *tableWidget = ui->flightTableWidget;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    // 表头
    QStringList headers{"航空公司", "航班号",   "出发城市",
                        "到达城市", "出发时间", "到达时间",
                        "票价",     "余票",     ""};
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置拉伸模式
    QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);

    // 隐藏行标签
    tableWidget->verticalHeader()->setVisible(false);

    if (flights.isEmpty()) {
      QMessageBox::information(this, "搜索结果", "没有找到符合条件的航班。");
      Back2Menu();

    } else {
      // 设置水平和垂直居中 填入数据行
      for (int row = 0; row < flights.size(); row++) {
        const Flight &flight = flights[row];
        tableWidget->insertRow(row);
        QTableWidgetItem *item;

        item = new QTableWidgetItem(flight.getAirline());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 0, item);

        item = new QTableWidgetItem(flight.getFlightNumber());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem(flight.getDepartureCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 2, item);

        item = new QTableWidgetItem(flight.getArrivalCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 3, item);

        item = new QTableWidgetItem(flight.getDepartureTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 4, item);

        item = new QTableWidgetItem(flight.getArrivalTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 5, item);

        item = new QTableWidgetItem(QString::number(flight.getPrice(), 'f', 2));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 6, item);

        item = new QTableWidgetItem(QString::number(flight.getRemainSeatNum()));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 7, item);

        if (flight.getRemainSeatNum() > 0) {
          QPushButton *buy_button = new QPushButton("订票");
          buy_button->setProperty("rowIndex", row); // 设置属性来存储行索引
          connect(buy_button, &QPushButton::clicked, this, [this, row]() {
            handleTicketBooking(row);
            ui->isMale->setChecked(false);
            ui->isFemale->setChecked(false);
            // 如果是改签航班，则把乘客信息直接填入
            if (orderType == RESCHEDULE_ORDER) {
              ui->familyname->setText(
                  rescheduleOrder.getPassenger().getFamilyName());
              ui->givenname->setText(
                  rescheduleOrder.getPassenger().getGivenName());
              ui->passenger_id->setText(rescheduleOrder.getPassenger().getId());
              ui->passenger_phone->setText(
                  rescheduleOrder.getPassenger().getPhone());
            }
          });
          tableWidget->setCellWidget(row, 8, buy_button);
        } else {
          QLabel *no_ticket = new QLabel("无票");
          no_ticket->setAlignment(Qt::AlignCenter); // 设置标签文本居中
          tableWidget->setCellWidget(row, 8, no_ticket);
        }
      }

      // 对于文本较多的列单独设置为 Stretch 模式
      horizontalHeader->setSectionResizeMode(4, QHeaderView::Stretch);
      horizontalHeader->setSectionResizeMode(5, QHeaderView::Stretch);

      // 对于文本较少的列单独设置为 ResizeToContents 模式
      horizontalHeader->setSectionResizeMode(1, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(2, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(3, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    }
  } else {
    // 如果是转机，那么将第一趟和第二趟航班显示在一行，但是订票按钮只显示一个
    QTableWidget *tableWidget = ui->flightTableWidget;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    // 表头
    QStringList headers{
        "航空公司", "航班号",   "出发城市", "到达城市", "出发时间", "到达时间",
        "票价",     "余票",     "航空公司", "航班号",   "出发城市", "到达城市",
        "出发时间", "到达时间", "票价",     "余票",     ""};
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置拉伸模式
    QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);

    // 隐藏行标签
    tableWidget->verticalHeader()->setVisible(false);

    if (flights.isEmpty()) {
      QMessageBox::information(this, "搜索结果", "没有找到符合条件的航班。");
      Back2Menu();

    } else {
      // 设置水平和垂直居中 填入数据行
      for (int row = 0; row < flights.size(); row += 2) {
        const Flight &flight1 = flights[row];
        const Flight &flight2 = flights[row + 1];
        tableWidget->insertRow(row/2);
        QTableWidgetItem *item;

        item = new QTableWidgetItem(flight1.getAirline());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 0, item);

        item = new QTableWidgetItem(flight1.getFlightNumber());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 1, item);

        item = new QTableWidgetItem(flight1.getDepartureCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 2, item);

        item = new QTableWidgetItem(flight1.getArrivalCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 3, item);

        item = new QTableWidgetItem(flight1.getDepartureTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 4, item);

        item = new QTableWidgetItem(flight1.getArrivalTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 5, item);

        item = new QTableWidgetItem(QString::number(flight1.getPrice(), 'f', 2));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 6, item);

        item = new QTableWidgetItem(QString::number(flight1.getRemainSeatNum()));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 7, item);

        item = new QTableWidgetItem(flight2.getAirline());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 8, item);

        item = new QTableWidgetItem(flight2.getFlightNumber());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 9, item);

        item = new QTableWidgetItem(flight2.getDepartureCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 10, item);

        item = new QTableWidgetItem(flight2.getArrivalCity());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 11, item);

        item = new QTableWidgetItem(flight2.getDepartureTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 12, item);

        item = new QTableWidgetItem(flight2.getArrivalTime());
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 13, item);

        item = new QTableWidgetItem(QString::number(flight2.getPrice(), 'f', 2));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 14, item);

        item = new QTableWidgetItem(QString::number(flight2.getRemainSeatNum()));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row/2, 15, item);



        if (flight1.getRemainSeatNum() > 0 && flight2.getRemainSeatNum() > 0) {
          QPushButton *buy_button = new QPushButton("订票");
          buy_button->setProperty("rowIndex", row); // 设置属性来存储行索引
          connect(buy_button, &QPushButton::clicked, this, [this, row]() {
            handleTicketBooking(row);
            ui->isMale->setChecked(false);
            ui->isFemale->setChecked(false);
            // 如果是改签航班，则把乘客信息直接填入
            if (orderType == RESCHEDULE_ORDER) {
              ui->familyname->setText(
                  rescheduleOrder.getPassenger().getFamilyName());
              ui->givenname->setText(
                  rescheduleOrder.getPassenger().getGivenName());
              ui->passenger_id->setText(rescheduleOrder.getPassenger().getId());
              ui->passenger_phone->setText(
                  rescheduleOrder.getPassenger().getPhone());
            }
          });
          tableWidget->setCellWidget(row/2, 16, buy_button);
        } else {
          QLabel *no_ticket = new QLabel("无票");
          no_ticket->setAlignment(Qt::AlignCenter); // 设置标签文本居中
          tableWidget->setCellWidget(row/2, 16, no_ticket);
        }
      }

      // 对于文本较多的列单独设置为 Stretch 模式
      horizontalHeader->setSectionResizeMode(4, QHeaderView::Stretch);
      horizontalHeader->setSectionResizeMode(5, QHeaderView::Stretch);

      // 对于文本较少的列单独设置为 ResizeToContents 模式
      horizontalHeader->setSectionResizeMode(1, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(2, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(3, QHeaderView::ResizeToContents);
      horizontalHeader->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    }
  }
}

void Flight_Ticket_Management_System::handleTicketBooking(int rowIndex)
{
  if (searchType == DIRECT) {
    QTableWidgetItem *item =
        ui->flightTableWidget->item(rowIndex, 1); // 假设航班号在第二列
    QTableWidgetItem *departureCity =
        ui->flightTableWidget->item(rowIndex, 2); // 第三列为出发地
    QTableWidgetItem *arrivalCity =
        ui->flightTableWidget->item(rowIndex, 3); // 第四列为目的地
    if (item) {
      QString flightNumber = item->text();
      // 因为这里存在转机的情况，所以不能根据最终出发地和最终目的地来判断
      QVector<Flight> currentFlights =
          network.searchFlights(departureCity->text(), arrivalCity->text(),
                                ui->calendarWidget->selectedDate());

      for (const Flight &flight : currentFlights) {
        if (flight.getFlightNumber() == flightNumber) {
          selectedFlight = flight;
          qDebug() << selectedFlight.toString();
          break;
        }
      }

      if (selectedFlight.getFlightNumber().isEmpty()) {
        QMessageBox::warning(this, "错误", "未找到对应的航班信息！");
        return;
      }

      // 切换到购票页面
      ui->stackedWidget->setCurrentWidget(ui->buyPage);

      ui->familyname->clear();
      ui->familyname->setPlaceholderText("姓");
      ui->givenname->clear();
      ui->givenname->setPlaceholderText("名");
      ui->passenger_id->clear();
      ui->passenger_id->setPlaceholderText("身份证号");
      ui->passenger_phone->clear();
      ui->passenger_phone->setPlaceholderText("手机号码");

      // 展示航班信息
      ui->flightInfo->setText(selectedFlight.showInfo());
    }
  } else {
    QTableWidgetItem *item1 =
        ui->flightTableWidget->item(rowIndex, 1); // 假设航班号在第二列
    QTableWidgetItem *departureCity1 =
        ui->flightTableWidget->item(rowIndex, 2); // 第三列为出发地
    QTableWidgetItem *arrivalCity1 =
        ui->flightTableWidget->item(rowIndex, 3); // 第四列为目的地

    QTableWidgetItem *item2 =
        ui->flightTableWidget->item(rowIndex, 9); // 假设航班号在第二列
    QTableWidgetItem *departureCity2 =
        ui->flightTableWidget->item(rowIndex, 10); // 第三列为出发地
    QTableWidgetItem *arrivalCity2 =
        ui->flightTableWidget->item(rowIndex, 11); // 第四列为目的地

    
    if (item1 && item2) {
      QString flightNumber1 = item1->text();
      QString flightNumber2 = item2->text();
      // 因为这里存在转机的情况，所以不能根据最终出发地和最终目的地来判断
      QVector<Flight> currentFlights1 =
          network.searchFlights(departureCity1->text(), arrivalCity1->text(),
                                     ui->calendarWidget->selectedDate());

      for (const Flight &flight : currentFlights1) {
        if (flight.getFlightNumber() == flightNumber1 ) {
          selectedFlight = flight;
          qDebug() << selectedFlight.toString();
          break;
        }
      }

      QVector<Flight> currentFlights2 =
          network.searchFlights(departureCity2->text(), arrivalCity2->text(),
                                     ui->calendarWidget->selectedDate());

      for (const Flight &flight : currentFlights2) {
        if (flight.getFlightNumber() == flightNumber2 ) {
          selectedFlight2 = flight;
          qDebug() << selectedFlight2.toString();
          break;
        }
      }

      if (selectedFlight.getFlightNumber().isEmpty() || selectedFlight2.getFlightNumber().isEmpty()) {
        QMessageBox::warning(this, "错误", "未找到对应的航班信息！");
        return;
      }

      // 切换到购票页面
      ui->stackedWidget->setCurrentWidget(ui->buyPage);

      ui->familyname->clear();
      ui->familyname->setPlaceholderText("姓");
      ui->givenname->clear();
      ui->givenname->setPlaceholderText("名");
      ui->passenger_id->clear();
      ui->passenger_id->setPlaceholderText("身份证号");
      ui->passenger_phone->clear();
      ui->passenger_phone->setPlaceholderText("手机号码");

      // 展示航班信息
      ui->flightInfo->setText(selectedFlight.showInfo() + "\n" +  selectedFlight2.showInfo());
    }
  }
}

void Flight_Ticket_Management_System::addPassenger()
{
    QString familyName = ui->familyname->text();
    QString givenName = ui->givenname->text();
    QString passengerId = ui->passenger_id->text();
    QString passengerPhone = ui->passenger_phone->text();
    QString sex = ui->isMale->isChecked() ? "Male" : (ui->isFemale->isChecked() ? "Female" : "");

    if (familyName.isEmpty() || givenName.isEmpty() || passengerId.isEmpty() || passengerPhone.isEmpty() || sex.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    // 如果是改签订单,我们需要判断是不是同一趟航班
    if (orderType == RESCHEDULE_ORDER) {
      if (selectedFlight == rescheduleOrder.getFlight()) {
        QMessageBox::warning(this, "错误", "改签订单不能改签同一趟航班！");
        return;
      }
    }

    // 创建乘客对象
    User passenger(familyName, givenName, sex, passengerId, passengerPhone);
    qDebug() << "乘客信息已提交:" << passenger.toString();
    Order *order = nullptr;
    // 创建订单对象
    if(searchType == DIRECT)
    {
        order = new Order(QUuid::createUuid().toString(), passenger, selectedFlight, "已支付");
    }
    else if(searchType == TRANSFER)
    {
        order = new Order(QUuid::createUuid().toString(), passenger, selectedFlight, "已支付",Order::OrderType::TRANSFER,selectedFlight2);
    }

    Flight thisFlight = selectedFlight;    
    // 减少余票数
    for (City* city : network.getCities()) {
        FlightNode* node = city->flights;
        while (node) {
            if (node->flight->getFlightNumber() == thisFlight.getFlightNumber()) {
                node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() - 1);
                qDebug() << "Updated remaining seats for flight " << thisFlight.getFlightNumber() << " to " << node->flight->getRemainSeatNum();
                break;
            }
            node = node->next;
        }
    }
    if(searchType == TRANSFER)
    {
        thisFlight = selectedFlight2;
        for (City* city : network.getCities()) {
            FlightNode* node = city->flights;
            while (node) {
                if (node->flight->getFlightNumber() == thisFlight.getFlightNumber()) {
                    node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() - 1);
                    qDebug() << "Updated remaining seats for flight " << thisFlight.getFlightNumber() << " to " << node->flight->getRemainSeatNum();
                    break;
                }
                node = node->next;
            }
        }
    }
    qInfo() << "ORDER:" << order->toString();

    // 写回航班文件
    network.writeDataToFile(FLIGHT_FILE);

    // 保存到订单
    QString filePath = ORDER_PATH + currentId + ".txt";
    orderManager.loadOrdersFromFile(filePath);
    orderManager.addOrder(*order); // 添加新订单到管理器
    delete order;
    if (!orderManager.saveOrdersToFile(filePath)) {
        QMessageBox::warning(this, "错误", "保存订单失败！");
        return;
    } else {
        qInfo() << "此订单已保存在" << currentId << ".txt文件中";
    }
    
    if(orderType==RESCHEDULE_ORDER){
        // 修改原订单状态为已改签
        rescheduleOrder.setStatus("已改签");
        orderManager.modifyOrder(orderManager.getOrders().indexOf(rescheduleOrder), rescheduleOrder);
        if (!orderManager.saveOrdersToFile(filePath)) {
            QMessageBox::warning(this, "错误", "保存订单失败！");
            return;
        } else {
            qInfo() << "此订单已保存在" << currentId << ".txt文件中";
        }

        // 增加余票信息
        for (City* city : network.getCities()) {
            FlightNode* node = city->flights;
            while (node) {
                if (node->flight->getFlightNumber() == rescheduleOrder.getFlight().getFlightNumber()) {
                    node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                    qDebug() << "Updated remaining seats for flight" << rescheduleOrder.getFlight().getFlightNumber() << "to" << node->flight->getRemainSeatNum();
                    break;
                }
                node = node->next;
            }
        }

        if(rescheduleOrder.getType() == Order::OrderType::TRANSFER) {
            // 增加余票信息
            for (City *city : network.getCities()) {
              FlightNode *node = city->flights;
              while (node) {
                if (node->flight->getFlightNumber() == rescheduleOrder.getFlight2().getFlightNumber()) {
                  node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                  qDebug() << "Updated remaining seats for flight"
                           << rescheduleOrder.getFlight2().getFlightNumber() << "to"
                           << node->flight->getRemainSeatNum();
                  break;
                }
                node = node->next;
              }
            }
        }
        QMessageBox::information(this, "确认", "改签成功！");
        orderType = NONE;
    }
    else{
        QMessageBox::information(this, "确认", "订票成功！");
    }

    toStart();
    ui->isMale->setCheckable(false);
    ui->isMale->setChecked(false);
    ui->isMale->setCheckable(true);

    ui->isFemale->setCheckable(false);
    ui->isFemale->setChecked(false);
    ui->isFemale->setCheckable(true);
}


void Flight_Ticket_Management_System::displayOrders(QTabWidget* tabWidget, const QString& statusFilter)
{
    QWidget *allTab = tabWidget->widget(1);
    QWidget *paidTab = tabWidget->widget(0);

    if (!allTab || !paidTab) {
        qDebug() << "Error: Tab widget not found!";
        return;
    }

    // 清除之前的内容
    auto clearLayout = [](QVBoxLayout *layout) {
        if (layout) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    delete item->widget();
                }
                delete item;
            }
        }
    };

    // 获取现有的布局
    QVBoxLayout *layoutAll = static_cast<QVBoxLayout*>(allTab->layout());
    QVBoxLayout *layoutPaid = static_cast<QVBoxLayout*>(paidTab->layout());

    // 如果布局不存在，则创建新的布局
    if (!layoutAll) {
        layoutAll = new QVBoxLayout(allTab);
    } else {
        clearLayout(layoutAll);
    }

    if (!layoutPaid) {
        layoutPaid = new QVBoxLayout(paidTab);
    } else {
        clearLayout(layoutPaid);
    }

    // 创建或重用 QTableWidget
    QTableWidget *tableWidgetAll = allTab->findChild<QTableWidget *>();
    QTableWidget *tableWidgetPaid = paidTab->findChild<QTableWidget *>();

    if (!tableWidgetAll) {
        tableWidgetAll = new QTableWidget(allTab);
    } else {
        tableWidgetAll->clearContents();
        tableWidgetAll->setRowCount(0);
    }

    if (!tableWidgetPaid) {
        tableWidgetPaid = new QTableWidget(paidTab);
    } else {
        tableWidgetPaid->clearContents();
        tableWidgetPaid->setRowCount(0);
    }

    // 配置表头
    QStringList headersAll {"乘客姓名", "航班信息", "状态"};
    QStringList headersPaid {"乘客姓名", "航班信息", "退票", "改签"};
    tableWidgetAll->setColumnCount(headersAll.size());
    tableWidgetPaid->setColumnCount(headersPaid.size());
    tableWidgetAll->setHorizontalHeaderLabels(headersAll);
    tableWidgetPaid->setHorizontalHeaderLabels(headersPaid);

    QHeaderView *horizontalHeader = tableWidgetAll->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetPaid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidgetAll->verticalHeader()->setVisible(false);
    tableWidgetPaid->verticalHeader()->setVisible(false);

    for (const Order &order : orderManager.getOrders()) {
        if (statusFilter.isEmpty() || order.getStatus() == statusFilter) {
            QTableWidget *currentTable = (tabWidget->currentWidget() == allTab) ? tableWidgetAll : tableWidgetPaid;
            int rowNum = currentTable->rowCount();
            currentTable->insertRow(rowNum);

            QTableWidgetItem *nameItem = new QTableWidgetItem(order.getPassenger().getFamilyName() + " " + order.getPassenger().getGivenName());

            QTableWidgetItem *flightItem = nullptr;
            if(order.getType()==Order::OrderType::DIRECT)
            {
                flightItem = new QTableWidgetItem(order.getFlight().toString());
            }
            else if(order.getType()==Order::OrderType::TRANSFER)
            {
                flightItem = new QTableWidgetItem(order.getFlight().toString() + "\n" + order.getFlight2().toString());
            }
            QTableWidgetItem *statusItem = new QTableWidgetItem(order.getStatus());

            nameItem->setTextAlignment(Qt::AlignCenter);
            flightItem->setTextAlignment(Qt::AlignCenter);
            statusItem->setTextAlignment(Qt::AlignCenter);

            currentTable->setItem(rowNum, 0, nameItem);
            currentTable->setItem(rowNum, 1, flightItem);
            if(currentTable == tableWidgetAll)
                currentTable->setItem(rowNum, 2, statusItem);

            if (currentTable == tableWidgetPaid) {
                // 添加退票按钮
                QPushButton *refundButton = new QPushButton("退票");
                refundButton->setProperty("orderId", order.getOrderId());
                connect(refundButton, &QPushButton::released, this, [this, refundButton]() {
                    handleRefund(refundButton->property("orderId").toString());
                });
                currentTable->setCellWidget(rowNum, 2, refundButton);

                // 添加改签按钮
                QPushButton *rescheduleButton = new QPushButton("改签");
                rescheduleButton->setProperty("orderId", order.getOrderId());
                connect(rescheduleButton, &QPushButton::released, this, [this, rescheduleButton]() {
                    handleReschedule(rescheduleButton->property("orderId").toString());
                });
                currentTable->setCellWidget(rowNum, 3, rescheduleButton);
            }
        }
    }

    layoutAll->addWidget(tableWidgetAll);
    layoutPaid->addWidget(tableWidgetPaid);
    allTab->setLayout(layoutAll);
    paidTab->setLayout(layoutPaid);

    tableWidgetAll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidgetPaid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidgetAll->resize(tableWidgetAll->sizeHint());
    tableWidgetPaid->resize(tableWidgetPaid->sizeHint());
}

void Flight_Ticket_Management_System::handleRefund(const QString& orderId)
{
    bool orderFound = false;
    for (int i = 0; i < orderManager.getOrders().size(); i++) {
        Order& order = orderManager.getOrders()[i];
        if (order.getOrderId() == orderId) {
            orderFound = true;
            // 检查订单状态
            if (order.getStatus() == "已支付") {
                order.setStatus("已退票");

                // 增加余票数
                QString flightNumber = order.getFlight().getFlightNumber();
                for (City* city : network.getCities()) {
                    FlightNode* node = city->flights;
                    while (node) {
                        if (node->flight->getFlightNumber() == flightNumber) {
                            node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                            qDebug() << "Updated remaining seats for flight" << flightNumber << "to" << node->flight->getRemainSeatNum();
                            break;
                        }
                        node = node->next;
                    }
                }

                // 保存更新后的订单到文件
                QString filePath = ORDER_PATH + currentId + ".txt";
                if (!orderManager.saveOrdersToFile(filePath)) {
                    QMessageBox::warning(this, "错误", "保存订单失败！");
                    return;
                } else qDebug()<<"save "<<filePath;

                network.writeDataToFile(FLIGHT_FILE);
                QMessageBox::information(this, "操作成功", "退票成功！");
                return;
            } else {
                QMessageBox::warning(this, "错误", "此订单不能退票！");
                return;
            }
        }
    }

    if (!orderFound) {
        QMessageBox::warning(this, "错误", "未找到订单！");
    }

}

void Flight_Ticket_Management_System::handleReschedule(const QString& orderId)
{
    bool orderFound = false;
    for (int i = 0; i < orderManager.getOrders().size(); i++) {
        Order& order = orderManager.getOrders()[i];
        if (order.getOrderId() == orderId) {
            orderFound = true;

            if (order.getStatus() == "已支付") {
               Flight flight  = order.getFlight();
               // 设置航班信息到订票页面
               auto departureCity = flight.getDepartureCity();
               auto arrivalCity = flight.getArrivalCity();

               ui->depBox->setCurrentText(departureCity);
               ui->arrBox->setCurrentText(arrivalCity);
               QDate date = QDate::fromString(flight.getDepartureTime().mid(0, 10), "yyyy-MM-dd");
               ui->calendarWidget->setSelectedDate(date);
               ui->showCalendarBtn->setText(date.toString("yyyy-MM-dd"));
               ui->stackedWidget->setCurrentWidget(ui->menuPage);
               rescheduleOrder = order;
               orderType = RESCHEDULE_ORDER;
            } else {
                QMessageBox::warning(this, "错误", "此订单不能改签！");
                return;
            }
            break;
        }
    }

    if (!orderFound) {
        QMessageBox::warning(this, "错误", "未找到订单！");
    }

}

void Flight_Ticket_Management_System::showMap() {
    ui->stackedWidget->setCurrentWidget(ui->mappage);
    ui->airlineComb->clear();
    // 将订单中已支付的航班显示在地图上
    ui->airlineComb->addItem("全部航线");
    auto orders = orderManager.getOrders();
    for (auto order : orders) {

        //qDebug()<<order.toString()<<"  " << Order::orderTypeToString(order.getType()) << "\n";

        if (order.getStatus() == "已支付") {
            if(order.getType() == Order::OrderType::DIRECT) {
                // Flight flight = order.getFlight();
                // QString airline = flight.getAirline();
                // if (!ui->airlineComb->findText(airline)) {
                //     ui->airlineComb->addItem(airline);
                // }
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

void Flight_Ticket_Management_System::displayMap(int index) {

  if (index == -1) {
    return;
  }

  if (index == 0) {
    // 显示所有
    auto orders = orderManager.getOrders();
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

        if(order.getType() == Order::OrderType::TRANSFER)
        {
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
    QList<Order> copyorder = orderManager.getOrders();
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
