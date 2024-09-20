#include "flight_ticket_management_system.h"
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


Flight_Ticket_Management_System::Flight_Ticket_Management_System(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Flight_Ticket_Management_System)
{
    ui->setupUi(this);

    //登陆界面
    initLogin();
    connect(ui->loginBtn, SIGNAL(released()), this, SLOT(Login()));

    connect(ui->Orders, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

    //配置日历
    ui->calendarWidget->setParent(ui->menuPage);
    ui->calendarWidget->hide();
    connect(ui->showCalendarBtn, SIGNAL(released()), this, SLOT(showCalendar()));
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(updateButtonWithDate(QDate)));

    //从文件读取数据到图中
    QTextStream* stream = LoadTextFile(FLIGHT_FILE);
    if (stream) {
        network.readData(stream);
        delete stream; // 确保释放资源
    }
    else {
        qInfo() << "Failed to load data file!";
    }

    //初始化comboBox
    initializeDepBox();
    connect(ui->depBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getDep(int)));
    initializeArrBox();
    connect(ui->arrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getArr(int)));

    //交换出发到达城市
    connect(ui->exchangeBtn, SIGNAL(released()), this, SLOT(Exchange()));

    // 搜索
    connect(ui->searchBtn, SIGNAL(released()),this,SLOT(Menu2Info()));
    connect(ui->searchBtn, SIGNAL(released()), this, SLOT(searchFlights()));

    //按条件排序
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->default_sort, 1);
    buttonGroup->addButton(ui->time_sort, 2);
    buttonGroup->addButton(ui->early_sort, 3);
    buttonGroup->addButton(ui->price_sort, 4);
    ui->default_sort->setChecked(true);
    buttonGroup->setExclusive(true);  // 确保按钮互斥
    connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(updateSearch(int)));

    //填写乘客信息
    connect(ui->submitBtn, SIGNAL(released()), this, SLOT(addPassenger()));
    connect(ui->cancelBtn, SIGNAL(released()), this, SLOT(toInfo()));



    //页面跳转
    connect(ui->menuBtn, SIGNAL(released()), this, SLOT(Start2Menu()));
    connect(ui->userBtn, SIGNAL(released()), this, SLOT(Start2User()));
    connect(ui->exitBtn, SIGNAL(released()),this,SLOT(Exit()));
    connect(ui->backBtn, SIGNAL(released()), this, SLOT(Back2Menu()));
    connect(ui->menu2startBtn, SIGNAL(released()), this, SLOT(toStart()));
    connect(ui->user2startBtn, SIGNAL(released()), this, SLOT(toStart()));

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
    QString filePath = "order/" + currentId + ".txt";
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
    //loadUserOrders();  // 加载用户订单
    toStart();
}

void Flight_Ticket_Management_System::onTabChanged(int index)
{
    QTabWidget *tabWidget = ui->Orders;
    if (tabWidget->tabText(index) == "全部") {
        displayAllOrders();
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
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
}

void Flight_Ticket_Management_System::Start2User()
{
    ui->stackedWidget->setCurrentWidget(ui->userPage);
    loadUserOrders();  // 加载用户订单
    ui->Orders->setCurrentIndex(0);
}

void Flight_Ticket_Management_System::toStart()
{
    ui->stackedWidget->setCurrentWidget(ui->startPage);
}

void Flight_Ticket_Management_System::toInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->flightInfoPage);
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
        updateTableWidget(flights);
    }
}

void Flight_Ticket_Management_System::updateSearch(int buttonId)
{
    QString depCity = getDep(ui->depBox->currentIndex());
    QString arrCity = getArr(ui->arrBox->currentIndex());
    QDate selectedDate = ui->calendarWidget->selectedDate();

    // 搜索航班
    QVector<Flight> flights = network.searchFlights(depCity, arrCity, selectedDate);

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

void Flight_Ticket_Management_System::updateTableWidget(const QVector<Flight>& flights)
{
    QTableWidget *tableWidget = ui->flightTableWidget;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    //表头
    QStringList headers {"航空公司", "航班号", "出发城市", "到达城市", "出发时间", "到达时间", "票价", "余票", ""};
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    //设置拉伸模式
    QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);

    //隐藏行标签
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

            if (flight.getRemainSeatNum()) {
                QPushButton *buy_button = new QPushButton("订票");
                buy_button->setProperty("rowIndex", row); // 设置属性来存储行索引
                connect(buy_button, &QPushButton::clicked, this, [this, row]() {
                    handleTicketBooking(row);
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
}


void Flight_Ticket_Management_System::handleTicketBooking(int rowIndex)
{
    QTableWidgetItem* item = ui->flightTableWidget->item(rowIndex, 1); // 假设航班号在第二列
    if (item) {
        QString flightNumber = item->text();
        QVector<Flight> currentFlights = network.searchFlights(getDep(ui->depBox->currentIndex()), getArr(ui->arrBox->currentIndex()), ui->calendarWidget->selectedDate());
        for (const Flight& flight : currentFlights) {
            if (flight.getFlightNumber() == flightNumber) {
                selectedFlight = flight;
                qDebug()<<selectedFlight.toString();
                break;
            }
        }

        if (selectedFlight.getFlightNumber().isEmpty()) {
            QMessageBox::warning(this, "错误", "未找到对应的航班信息！");
            return;
        }

        // 切换到购票页面
        ui->stackedWidget->setCurrentWidget(ui->buyPage);
        ui->familyname->setPlaceholderText("姓");
        ui->givenname->setPlaceholderText("名");
        ui->passenger_id->setPlaceholderText("身份证号");
        ui->passenger_phone->setPlaceholderText("手机号码");

        // 展示航班信息
        ui->flightInfo->setText(selectedFlight.showInfo());
    }

}

void Flight_Ticket_Management_System::addPassenger()
{
    QString familyName = ui->familyname->text();
    QString givenName = ui->givenname->text();
    QString passengerId = ui->passenger_id->text();
    QString passengerPhone = ui->passenger_phone->text();

    // 直接获取性别按钮的选中状态
    QString sex;
    if (ui->isMale->isChecked()) {
        sex = "Male";
    } else if (ui->isFemale->isChecked()) {
        sex = "Female";
    }

    if (familyName.isEmpty() || givenName.isEmpty() || passengerId.isEmpty() || passengerPhone.isEmpty() || sex.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return;
    }

    // 创建乘客对象
    User passenger(familyName, givenName, sex, passengerId, passengerPhone);
    qDebug() << "乘客信息已提交:" << passenger.toString();

    // 创建订单对象
    Flight thisFlight = selectedFlight;
    Order order(QUuid::createUuid().toString(), passenger, thisFlight, "未支付");


    qInfo() << "ORDER:"<<order.toString();
    orderManager.addOrder(order); // 添加订单到管理器

    // 保存订单到文件
    QString filePath = "order/" + currentId + ".txt";
    orderManager.saveOrdersToFile(filePath);
    qInfo() << "此订单已保存在" << currentId << ".txt文件中";

    QMessageBox::information(this, "确认", "订票成功！");
    toStart();
}


void Flight_Ticket_Management_System::displayAllOrders()
{

    qDebug()<<"查看全部订单";
    QWidget *allTab = ui->Orders->widget(1);  // 获取 "全部" 标签页

    // 检查是否存在布局，并删除所有子控件
    QVBoxLayout *layout = static_cast<QVBoxLayout*>(allTab->layout());
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    } else {
        layout = new QVBoxLayout(allTab);
    }

    QTableWidget *tableWidget = new QTableWidget(allTab);
    tableWidget->setParent(allTab);

    // 表头
    QStringList headers {"乘客姓名", "航班信息", "退票", "改签"};
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置拉伸模式
    QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);

    // 隐藏行标签
    tableWidget->verticalHeader()->setVisible(false);


    for (int row = 0; row < orderManager.getOrders().size(); row++) {
        const Order &order = orderManager.getOrders()[row];

        tableWidget->insertRow(row);

        QTableWidgetItem *nameItem = new QTableWidgetItem(order.getPassenger().getFamilyName() + " " + order.getPassenger().getGivenName());
        nameItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 0, nameItem);

        QTableWidgetItem *flightItem = new QTableWidgetItem(order.getFlight().getAirline() + " " + order.getFlight().getFlightNumber());
        flightItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 1, flightItem);

        QPushButton *refundButton = new QPushButton("退票");
        connect(refundButton, &QPushButton::clicked, this, [this, row]() { handleRefund(row); });
        tableWidget->setCellWidget(row, 2, refundButton);

        QPushButton *rescheduleButton = new QPushButton("改签");
        connect(rescheduleButton, &QPushButton::clicked, this, [this, row]() { handleReschedule(row); });
        tableWidget->setCellWidget(row, 3, rescheduleButton);
    }

    layout->addWidget(tableWidget);
    allTab->setLayout(layout);

    // 调整表格的大小以适应标签页
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->resize(tableWidget->sizeHint());
}

void Flight_Ticket_Management_System::handleRefund(int rowIndex)
{
    // 处理退票逻辑
    const Order &order = orderManager.getOrders()[rowIndex];
    qDebug() << "Refunding order: " << order.getOrderId();
    // 这里可以添加退票的逻辑，例如更新订单状态，退款等





    QMessageBox::information(this, "退票", "已成功退票。");
}

void Flight_Ticket_Management_System::handleReschedule(int rowIndex)
{
    // 处理改签逻辑
    const Order &order = orderManager.getOrders()[rowIndex];
    qDebug() << "Rescheduling order: " << order.getOrderId();
    // 这里可以添加改签的逻辑
    QMessageBox::information(this, "改签", "已成功改签。");
}

void Flight_Ticket_Management_System::showMap()
{



}
