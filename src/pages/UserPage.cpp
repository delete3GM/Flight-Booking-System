#include "pages/UserPage.h"
#include "qboxlayout.h"
#include "ui_userpage.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>


UserPage::UserPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::UserPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    initOrderTab();

    connect(ui->user2menuBtn, SIGNAL(released()), this, SLOT(user2menu()));
    connect(ui->Orders, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    connect(ui->graphBtn, SIGNAL(released()), this, SLOT(user2map()));

}

UserPage::~UserPage() {
    delete ui;
}

void UserPage::user2menu() {
    mainWindow->showMenuPage();
}

void UserPage::user2map() {
    mainWindow->showMapPage();
}

void UserPage::handleRefund(const QString& orderId) {
    bool orderFound = false;
    for (int i = 0; i < mainWindow->orderManager.getOrders().size(); i++) {
        Order& order = mainWindow->orderManager.getOrders()[i];
        if (order.getOrderId() == orderId) {
            orderFound = true;
            // 检查订单状态
            if (order.getStatus() == "已支付") {
                order.setStatus("已退票");

                // 增加余票数
                QString flightNumber = order.getFlight().getFlightNumber();
                for (City* city : mainWindow->network.getCities()) {
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
                QString filePath = ORDER_PATH + mainWindow->currentId + ".txt";
                if (!mainWindow->orderManager.saveOrdersToFile(filePath)) {
                    QMessageBox::warning(this, "错误", "保存订单失败！");
                    return;
                } else qDebug()<<"已保存到"<<filePath;

                mainWindow->network.writeFlightToFile(FLIGHT_FILE);
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

void UserPage::handleReschedule(const QString& orderId) {
    bool orderFound = false;
    for (int i = 0; i < mainWindow->orderManager.getOrders().size(); i++) {
        Order& order = mainWindow->orderManager.getOrders()[i];
        if (order.getOrderId() == orderId) {
            orderFound = true;

            if (order.getStatus() == "已支付") {
                mainWindow->orderType = Flight_Ticket_Management_System::RESCHEDULE_ORDER;
                mainWindow->rescheduleOrder = order;
                Flight flight  = order.getFlight();
                // 设置航班信息到订票页面
                auto departureCity = flight.getDepartureCity();
                auto arrivalCity = flight.getArrivalCity();
                mainWindow->initReschedule(departureCity, arrivalCity, QDate::fromString(flight.getDepartureTime().section(" ", 0, 0), "yyyy-MM-dd"));
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

void UserPage::displayOrders(QTabWidget* tabWidget, const QString& statusFilter) {
    QWidget *allTab = tabWidget->widget(1);
    QWidget *paidTab = tabWidget->widget(0);

    if (!allTab || !paidTab) {
        qDebug() << "未找到Tab widget";
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

    for (const Order &order : mainWindow->orderManager.getOrders()) {
        if (statusFilter.isEmpty() || order.getStatus() == statusFilter) {
            QTableWidget *currentTable = (tabWidget->currentWidget() == allTab) ? tableWidgetAll : tableWidgetPaid;
            int rowNum = currentTable->rowCount();
            currentTable->insertRow(rowNum);

            QTableWidgetItem *nameItem = new QTableWidgetItem(order.getPassenger().getFamilyName() + " " + order.getPassenger().getGivenName());

            QTableWidgetItem *flightItem = nullptr;
            if(order.getType()==Order::OrderType::DIRECT) {
                flightItem = new QTableWidgetItem(order.getFlight().toString());
            }
            else if(order.getType()==Order::OrderType::TRANSFER) {
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
                connect(refundButton, &QPushButton::released, this, [ refundButton, this]() {
                    handleRefund(refundButton->property("orderId").toString());
                });
                currentTable->setCellWidget(rowNum, 2, refundButton);

                // 添加改签按钮
                QPushButton *rescheduleButton = new QPushButton("改签");
                rescheduleButton->setProperty("orderId", order.getOrderId());
                connect(rescheduleButton, &QPushButton::released, this, [ rescheduleButton, this]() {
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

void UserPage::initOrderTab() {
    mainWindow->loadUserOrders();  // 加载用户订单
    ui->Orders->setCurrentIndex(0);
    displayOrders(ui->Orders, "已支付");
}

void UserPage::onTabChanged(int index) {
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

