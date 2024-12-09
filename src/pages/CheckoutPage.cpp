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
    ui->mealBox->setCurrentIndex(0);
    ui->insuranceBox->setCurrentIndex(0);

    ui->flightInfoLbl->setText(mainWindow->selectedFlight.showFlightsInfo());
    ui->priceLbl->setTextFormat(Qt::RichText);
    ui->priceLbl->setText(priceInfo());

    ui->historyArea->setWidgetResizable(true);

    loadPassengerHistory();
    initPassengerArea();

    initMealBox();
    initInsBox();
}

void CheckoutPage::initMealBox() {
    ui->mealBox->addItem("无餐食", QVariant("无餐食"));
    ui->mealBox->addItem("标准餐              35元", QVariant("标准餐"));
    ui->mealBox->addItem("豪华餐              80元", QVariant("豪华餐"));

    connect(ui->mealBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CheckoutPage::updateMealSelection);
}

void CheckoutPage::initInsBox() {
    ui->insuranceBox->addItem("无保险", QVariant("无保险"));
    ui->insuranceBox->addItem("基础保险           80元", QVariant("基础保险"));
    ui->insuranceBox->addItem("尊享保险          150元", QVariant("尊享保险"));

    connect(ui->insuranceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CheckoutPage::updateInsSelection);
}

void CheckoutPage::updateMealSelection(int index) {
    meal = ui->mealBox->itemData(index).toString();
    ui->priceLbl->setText(priceInfo());
}

void CheckoutPage::updateInsSelection(int index) {
    insurance = ui->insuranceBox->itemData(index).toString();
    ui->priceLbl->setText(priceInfo());
}

QString CheckoutPage::priceInfo() {
    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    double basePrice = mainWindow->selectedFlight.getTotalPrice() * discount;
    double cabinPriceMultiplier = cabinPrice[mainWindow->selectedClass];
    basePrice *= cabinPriceMultiplier;

    double fuelCost = mainWindow->selectedFlight.getTotalPrice() * FUEL_RATIO;
    double mealCost = mealPrice[meal];
    double insuranceCost = insurancePrice[insurance];

    double totalPrice = basePrice + fuelCost + mealCost + insuranceCost;

    QString info = QString(
                       "<font size='5' color='#111'><b>订单总价：￥%1</b></font><br>"
                       "<font color='#555'>票价：￥%2</font><br>"
                       "<font color='#555'>基建燃油：￥%3</font><br>"
                       "<font color='#555'>餐食：%4（+￥%5）</font><br>"
                       "<font color='#555'>保险：%6（+￥%7）</font>"
                       )
                       .arg(QString::number(totalPrice, 'f', 2))
                       .arg(QString::number(basePrice, 'f', 2))
                       .arg(QString::number(fuelCost, 'f', 2))
                       .arg(meal)
                       .arg(QString::number(mealCost, 'f', 2))
                       .arg(insurance)
                       .arg(QString::number(insuranceCost, 'f', 2));
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

void CheckoutPage::getSelectedPriceRatio() {
    if (mainWindow->selectedFlight.getFlightCount() == 0 || mainWindow->searchedFlights.isEmpty()) {
        qWarning() << "推荐航班无法计算得分";
        return;
    }

    double totalPrice = 0.0;
    for (const auto& route : mainWindow->searchedFlights) {
        double price = route.getTotalPrice();
        totalPrice += price;
    }
    double basePrice = totalPrice / mainWindow->searchedFlights.size();
    double selectedPrice = mainWindow->selectedFlight.getTotalPrice();
    double ratio = selectedPrice / basePrice;
    mainWindow->currentUser.updatePriceRatio(ratio);
}

void CheckoutPage::addPassenger() {
    Passenger passenger = createPassenger();
    if (passenger.getFamilyName() == "") {
        return;
    }
    Order order = createOrder(passenger);
    getSelectedPriceRatio();
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
    double fuelCost = mainWindow->selectedFlight.getTotalPrice() * FUEL_RATIO;
    double mealExtraCost = mealPrice[meal];
    double insuranceCost = insurancePrice[insurance];

    double totalPrice = cabinPrice + fuelCost + mealExtraCost + insuranceCost;
    Order order(QUuid::createUuid().toString(), passenger, mainWindow->selectedFlight, meal, insurance, "已支付", totalPrice,
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


