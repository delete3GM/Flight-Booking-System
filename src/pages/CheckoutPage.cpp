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
    //connect(passengerButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onPassengerButtonClicked(int)));
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
    ui->flightInfo->setText(mainWindow->selectedFlight.showFlightsInfo());
    ui->historyArea->setWidgetResizable(true);
    loadPassengerHistory();
    initPassengerArea();
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
    QList<Order> orders = mainWindow->orderManager.getOrders();
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
        handleRescheduleOrder(order);
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
    Order order(QUuid::createUuid().toString(), passenger, mainWindow->selectedFlight, "已支付");
    updateFlightSeats(order);
    saveOrderToFile(order);
    return order;
}

void CheckoutPage::handleRescheduleOrder(const Order& order) {
    mainWindow->rescheduleOrder.setStatus("已改签");

    double oldPrice = mainWindow->rescheduleOrder.getFlightRoute().getTotalPrice();
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
    mainWindow->orderManager.modifyOrder(mainWindow->orderManager.getOrders().indexOf(mainWindow->rescheduleOrder), mainWindow->rescheduleOrder);
    increaseSeatsForRescheduledFlight();
    QMessageBox::information(this, "确认", "改签成功！");
    mainWindow->orderType = Flight_Ticket_Management_System::NONE;
    mainWindow->network.writeFlightToFile(FLIGHT_FILE);
}

void CheckoutPage::handleNewOrder(const Order& order) {
    QString text="Payment Success!\nYou have paid "+ QString::number(order.getFlightRoute().getTotalPrice()) +" CNY!";
    showQRCode(text);
    mainWindow->network.writeFlightToFile(FLIGHT_FILE);
    QMessageBox::information(this, "确认", "订票成功！");
}

void CheckoutPage::updateFlightSeats(const Order& order) {
    for (const Flight* flight : order.getFlightRoute()) {
        for (City* city : mainWindow->network.getCities()) {
            FlightNode* node = city->flights;
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
    for (const Flight* flight : mainWindow->rescheduleOrder.getFlightRoute()) {
        increaseSeatsForFlight(flight);
    }
}

void CheckoutPage::increaseSeatsForFlight(const Flight* flight) {
    for (City* city : mainWindow->network.getCities()) {
        FlightNode* node = city->flights;
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
    QString filePath = ORDER_PATH + mainWindow->currentUserId + ".json";
    mainWindow->orderManager.addOrder(order);
    return mainWindow->orderManager.saveOrdersToJsonFile(filePath);
}

void CheckoutPage::resetGenderRadioButtons() {
    ui->isMale->setCheckable(false);
    ui->isMale->setChecked(false);
    ui->isMale->setCheckable(true);
    ui->isFemale->setCheckable(false);
    ui->isFemale->setChecked(false);
    ui->isFemale->setCheckable(true);
}

