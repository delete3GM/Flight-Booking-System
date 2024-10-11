#include "pages/CheckoutPage.h"
#include "ui_checkoutpage.h"
#include <QMessageBox>

CheckoutPage::CheckoutPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
     : QWidget(parent), ui(new Ui::CheckoutPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    initCheckout();

    connect(ui->submitBtn, SIGNAL(released()), this, SLOT(addPassenger()));
    connect(ui->cancelBtn, SIGNAL(released()), this, SLOT(checkout2info()));
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
    if(mainWindow->searchType == Flight_Ticket_Management_System::TRANSFER){
        QString totalPrice = "待支付￥" + QString::number(mainWindow->selectedFlight.getPrice()+mainWindow->selectedFlight2.getPrice());
        ui->flightInfo->setText(mainWindow->selectedFlight.showInfo() + "\n" +  mainWindow->selectedFlight2.showInfo() + "\n" + totalPrice);
    } else {
        ui->flightInfo->setText(mainWindow->selectedFlight.showInfo());
    }
}

void CheckoutPage::checkout2info() {
    mainWindow->showInfoPage();
}

void CheckoutPage::addPassenger() {
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
    if (mainWindow->orderType == Flight_Ticket_Management_System::RESCHEDULE_ORDER) {
        if (mainWindow->selectedFlight == mainWindow->rescheduleOrder.getFlight()) {
            QMessageBox::warning(this, "错误", "改签订单不能改签同一趟航班！");
            return;
        }
    }
    // 创建乘客对象
    User passenger(familyName, givenName, sex, passengerId, passengerPhone);
    qDebug() << "乘客信息已提交:" << passenger.toString();
    Order *order = nullptr;
    // 创建订单对象
    if(mainWindow->searchType == Flight_Ticket_Management_System::DIRECT) {
        order = new Order(QUuid::createUuid().toString(), passenger, mainWindow->selectedFlight, "已支付");
    }
    else if(mainWindow->searchType == Flight_Ticket_Management_System::TRANSFER) {
        order = new Order(QUuid::createUuid().toString(), passenger, mainWindow->selectedFlight, "已支付",Order::OrderType::TRANSFER,mainWindow->selectedFlight2);
    }
    Flight thisFlight = mainWindow->selectedFlight;
    // 减少余票数
    for (City* city : mainWindow->network.getCities()) {
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
    if(mainWindow->searchType == Flight_Ticket_Management_System::TRANSFER) {
        thisFlight = mainWindow->selectedFlight2;
        for (City* city : mainWindow->network.getCities()) {
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

    // 保存到订单
    QString filePath = ORDER_PATH + mainWindow->currentId + ".txt";
    mainWindow->orderManager.loadOrdersFromFile(filePath);
    mainWindow->orderManager.addOrder(*order); // 添加新订单到管理器
    delete order;
    if (!mainWindow->orderManager.saveOrdersToFile(filePath)) {
        QMessageBox::warning(this, "错误", "保存订单失败！");
        return;
    } else {
        qInfo() << "此订单已保存在" << mainWindow->currentId << ".txt文件中";
    }
    if(mainWindow->orderType==Flight_Ticket_Management_System::RESCHEDULE_ORDER){
        // 修改原订单状态为已改签
        mainWindow->rescheduleOrder.setStatus("已改签");
        mainWindow->orderManager.modifyOrder(mainWindow->orderManager.getOrders().indexOf(mainWindow->rescheduleOrder), mainWindow->rescheduleOrder);
        if (!mainWindow->orderManager.saveOrdersToFile(filePath)) {
            QMessageBox::warning(this, "错误", "保存订单失败！");
            return;
        } else {
            qInfo() << "此订单已保存在" << mainWindow->currentId << ".txt文件中";
        }

        // 增加余票信息
        for (City* city : mainWindow->network.getCities()) {
            FlightNode* node = city->flights;
            while (node) {
                if (node->flight->getFlightNumber() == mainWindow->rescheduleOrder.getFlight().getFlightNumber()) {
                    node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                    qDebug() << "Updated remaining seats for flight" << mainWindow->rescheduleOrder.getFlight().getFlightNumber() << "to" << node->flight->getRemainSeatNum();
                    break;
                }
                node = node->next;
            }
        }
        if(mainWindow->rescheduleOrder.getType() == Order::OrderType::TRANSFER) {
            // 增加余票信息
            for (City *city : mainWindow->network.getCities()) {
                FlightNode *node = city->flights;
                while (node) {
                    if (node->flight->getFlightNumber() == mainWindow->rescheduleOrder.getFlight2().getFlightNumber()) {
                        node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                        qDebug() << "Updated remaining seats for flight"<< mainWindow->rescheduleOrder.getFlight2().getFlightNumber() << "to"<< node->flight->getRemainSeatNum();
                        break;
                    }
                    node = node->next;
                }
            }
        }
        QMessageBox::information(this, "确认", "改签成功！");
        mainWindow->orderType = Flight_Ticket_Management_System::NONE;
        // 写回航班文件
        mainWindow->network.writeFlightToFile(FLIGHT_FILE);
    }
    else{
        QMessageBox::information(this, "确认", "订票成功！");
        // 写回航班文件
        mainWindow->network.writeFlightToFile(FLIGHT_FILE);
    }
    mainWindow->showMenuPage();
    ui->isMale->setCheckable(false);
    ui->isMale->setChecked(false);
    ui->isMale->setCheckable(true);

    ui->isFemale->setCheckable(false);
    ui->isFemale->setChecked(false);
    ui->isFemale->setCheckable(true);
}
