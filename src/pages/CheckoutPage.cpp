#include "pages/CheckoutPage.h"
#include "qboxlayout.h"
#include "ui_checkoutpage.h"
#include <QMessageBox>
#include <QTimer>
#include "Utils.h"


CheckoutPage::CheckoutPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
     : QWidget(parent), ui(new Ui::CheckoutPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initCheckout();

    connect(ui->submitBtn, &QPushButton::released, this, &CheckoutPage::addPassenger);
    connect(ui->cancelBtn, &QPushButton::released, this, &CheckoutPage::checkout2info);
    connect(ui->noFoodBtn, &QPushButton::released, this, &CheckoutPage::changeNoFood);
    connect(ui->normalFoodBtn, &QPushButton::released, this, &CheckoutPage::changeNormalFood);
    connect(ui->plusFoodBtn, &QPushButton::released, this, &CheckoutPage::changePlusFood);
}

CheckoutPage::~CheckoutPage() {
    delete ui;
}

void CheckoutPage::initCheckout() {
    if (mainWindow->orderType == Flight_Ticket_Management_System::RESCHEDULE_ORDER) {
        ui->familyname->setText(mainWindow->rescheduleOrder.getPassenger().getFamilyName());
        ui->givenname->setText(mainWindow->rescheduleOrder.getPassenger().getGivenName());
        ui->isMale->setChecked(mainWindow->rescheduleOrder.getPassenger().getSex() == "Male");
        ui->isFemale->setChecked(mainWindow->rescheduleOrder.getPassenger().getSex() == "Female");
        ui->passenger_id->setText(mainWindow->rescheduleOrder.getPassenger().getId());
        ui->passenger_phone->setText(mainWindow->rescheduleOrder.getPassenger().getPhone());
    } else {
        ui->familyname->clear();
        ui->familyname->setPlaceholderText("姓");
        ui->givenname->clear();
        ui->givenname->setPlaceholderText("名");
        ui->passenger_id->clear();
        ui->passenger_id->setPlaceholderText("身份证号");
        ui->passenger_phone->clear();
        ui->passenger_phone->setPlaceholderText("手机号码");
    }
    ui->flightInfo->setText(detailInfo());
    ui->historyArea->setWidgetResizable(true);
    ui->foodLbl->setText("已选择：无餐食\n需额外支付 0 元");
    loadPassengerHistory();
    initPassengerArea();
}

QString CheckoutPage::detailInfo() {
    QString info = mainWindow->selectedFlight.showFlightsInfo();

    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    double basePrice = mainWindow->selectedFlight.getTotalPrice() * discount;

    if(mainWindow->selectedClass == "经济舱") {
        basePrice *= 1.0;
    } else if(mainWindow->selectedClass == "商务舱") {
        basePrice *= 1.8;
    } else {
        basePrice *= 2.4;
    }

    info += "待支付￥" + QString::number(basePrice, 'f', 2);

    double mealPrice = 0;
    if(meal == "无餐食") {
        mealPrice = 0;
    } else if(meal == "标准餐") {
        mealPrice = NORMAL_MEAL_PRICE;
    } else {
        mealPrice = PLUS_MEAL_PRICE;
    }

    if(mealPrice > 0) {
        double totalPrice = basePrice + mealPrice;
        QString finalPrice = " + " + QString::number(mealPrice, 'f', 2) + " = " + QString::number(totalPrice, 'f', 2);
        info += finalPrice;
    }
    return info;
}

void CheckoutPage::checkout2info() {
    mainWindow->showInfoPage();
}

void CheckoutPage::initPassengerArea() {
    QVBoxLayout *passengerLayout = new QVBoxLayout(ui->passengerContainer);
    passengerLayout->setAlignment(Qt::AlignTop);
    while (QLayoutItem* item = passengerLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    foreach (const Passenger &passenger, passengerList) {
        QPushButton *button = new QPushButton(passenger.getFamilyName() + " " + passenger.getGivenName(), this);
        passengerLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, passenger]() {
            fillPassengerInfo(passenger);
        });
    }
    ui->passengerContainer->setLayout(passengerLayout);
}

void CheckoutPage::loadPassengerHistory() {
    QList<Order> orders = mainWindow->currentUser.orderManager.getOrders();
    passengerList.clear();
    foreach (const Order &order, orders) {
        Passenger passenger = order.getPassenger();
        if (!passengerList.contains(passenger)) {
            passengerList.append(passenger);
        }
    }
}

void CheckoutPage::fillPassengerInfo(const Passenger &passenger) {
    ui->familyname->setText(passenger.getFamilyName());
    ui->givenname->setText(passenger.getGivenName());
    if (passenger.getSex() == "Male") {
        ui->isMale->setChecked(true);
    } else if (passenger.getSex() == "Female") {
        ui->isFemale->setChecked(true);
    }
    ui->passenger_id->setText(passenger.getId());
    ui->passenger_phone->setText(passenger.getPhone());
}

void CheckoutPage::addPassenger() {
    Passenger passenger = createPassenger();
    if (passenger.getFamilyName() == "") {
        return;
    }
    Order order = createOrder(passenger);
    if (mainWindow->orderType == Flight_Ticket_Management_System::RESCHEDULE_ORDER) {
        handleRescheduleOrder();
    } else {
        handleNewOrder(order);
    }
    mainWindow->showMenuPage();
    resetGenderRadioButtons();
}

Passenger CheckoutPage::createPassenger() {
    QString familyName = ui->familyname->text();
    QString givenName = ui->givenname->text();
    QString passengerId = ui->passenger_id->text();
    QString passengerPhone = ui->passenger_phone->text();
    QString sex = ui->isMale->isChecked() ? "Male" : (ui->isFemale->isChecked() ? "Female" : "");

    if (familyName.isEmpty() || givenName.isEmpty() || passengerId.isEmpty() || passengerPhone.isEmpty() || sex.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完整信息！");
        return Passenger();
    }
    return Passenger(familyName, givenName, sex, passengerId, passengerPhone);
}

Order CheckoutPage::createOrder(const Passenger& passenger) {
    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    double basePrice = mainWindow->selectedFlight.getTotalPrice() * discount;
    double cabinPriceMultiplier = cabinPrice[mainWindow->selectedClass];
    double cabinPrice = basePrice * cabinPriceMultiplier;
    double mealExtraCost = mealPrice[meal];

    double totalPrice = cabinPrice + mealExtraCost;
    Order order(QUuid::createUuid().toString(), passenger, mainWindow->selectedFlight, meal, "已支付", totalPrice,
                mainWindow->selectedClass);
    updateFlightSeats(order);
    saveOrderToFile(order);
    return order;
}

void CheckoutPage::handleRescheduleOrder() {
    mainWindow->rescheduleOrder.setStatus("已改签");

    double oldPrice = mainWindow->rescheduleOrder.getPrice();
    double newPrice = mainWindow->selectedFlight.getTotalPrice();
    double priceDifference = newPrice - oldPrice;

    QString text = "";
    if(priceDifference > 0) {
        text="Reschedule Success!\nYou have paid "+ QString::number(priceDifference) +" CNY!";
    } else if(priceDifference == 0) {
        text="Reschedule Success!";
    } else if(priceDifference < 0) {
        text="Reschedule Success!\nYou have received a refund of "+ QString::number(-1 * priceDifference) +" CNY!";
    }
    showQRCode(text);
    mainWindow->currentUser.orderManager.modifyOrder(
        mainWindow->currentUser.orderManager.getOrders().indexOf(mainWindow->rescheduleOrder), mainWindow->rescheduleOrder);
    increaseSeatsForRescheduledFlight();

    QMessageBox::information(this, "确认", "改签成功！");
    mainWindow->orderType = Flight_Ticket_Management_System::NONE;
    mainWindow->network.writeFlightToFile(FLIGHT_FILE);
}

void CheckoutPage::handleNewOrder(const Order& order) {
    QString text="Payment Success!\nYou have paid "+ QString::number(order.getPrice()) +" CNY!";
    showQRCode(text);
    mainWindow->network.writeFlightToFile(FLIGHT_FILE);
    QMessageBox::information(this, "确认", "订票成功！");
}

void CheckoutPage::updateFlightSeats(const Order& order) {
    for (const std::shared_ptr<Flight> &flight : order.getFlightRoute()) {
        for (City* city : mainWindow->network.getCities()) {
            FlightNode* node = city->flightsHead;
            while (node) {
                if (node->flight->getFlightNumber() == flight->getFlightNumber()) {
                    node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() - 1);
                    qDebug() << "Updated remaining seats for flight " << flight->getFlightNumber() << " to " << node->flight->getRemainSeatNum();
                    break;
                }
                node = node->next;
            }
        }
    }
}

void CheckoutPage::increaseSeatsForRescheduledFlight() {
    for (const std::shared_ptr<Flight> &flight : mainWindow->rescheduleOrder.getFlightRoute()) {
        increaseSeatsForFlight(flight);
    }
}

void CheckoutPage::increaseSeatsForFlight(const std::shared_ptr<Flight> flight) {
    for (City* city : mainWindow->network.getCities()) {
        FlightNode* node = city->flightsHead;
        while (node) {
            if (node->flight->getFlightNumber() == flight->getFlightNumber()) {
                node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                break;
            }
            node = node->next;
        }
    }
}

bool CheckoutPage::saveOrderToFile(const Order& order) {
    QString filePath = ORDER_PATH + mainWindow->currentUser.getID() + ".json";
    mainWindow->currentUser.orderManager.addOrder(order);
    return mainWindow->currentUser.orderManager.saveOrdersToJsonFile(filePath);
}

void CheckoutPage::resetGenderRadioButtons() {
    ui->isMale->setCheckable(false);
    ui->isMale->setChecked(false);
    ui->isMale->setCheckable(true);
    ui->isFemale->setCheckable(false);
    ui->isFemale->setChecked(false);
    ui->isFemale->setCheckable(true);
}

void CheckoutPage::changeNoFood() {
    ui->foodLbl->setText("请选择餐食：无餐食\n需额外支付 0 元");
    meal = "无餐食";
    ui->flightInfo->setText(detailInfo());
}

void CheckoutPage::changeNormalFood() {
    ui->foodLbl->setText("请选择餐食：标准餐\n需额外支付 " + QString::number(NORMAL_MEAL_PRICE) + "元");
    meal = "标准餐";
    ui->flightInfo->setText(detailInfo());
}

void CheckoutPage::changePlusFood() {
    ui->foodLbl->setText("请选择餐食：豪华餐\n需额外支付 " + QString::number(PLUS_MEAL_PRICE) + "元");
    meal = "豪华餐";
    ui->flightInfo->setText(detailInfo());
}

