#include "pages/UserPage.h"
#include "qboxlayout.h"
#include "qtoolbutton.h"
#include "ui_userpage.h"
#include "widgets/OrderDetailsDialog.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>


UserPage::UserPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::UserPage), mainWindow(mainWindow) {
    ui->setupUi(this);
    initOrderTab();
    ui->user2menuBtn->setIcon(QIcon(":/images/resources/images/back.png"));
    ui->user2menuBtn->setIconSize(QSize(32, 32));
    ui->user2menuBtn->setStyleSheet("QToolButton { border: none; background: none; }");
    ui->user2menuBtn->setAutoRaise(true);

    connect(ui->user2menuBtn, &QPushButton::released, this, &UserPage::user2menu);
    connect(ui->Orders, &QTabWidget::currentChanged, this, &UserPage::onTabChanged);
    connect(ui->domMapBtn, &QPushButton::released, this, &UserPage::showDomesticMap);
    connect(ui->glbMapBtn, &QPushButton::released, this, &UserPage::showGlobalMap);
    connect(ui->clearBtn, &QPushButton::released, this, &UserPage::deleteAllOrder);
}

UserPage::~UserPage() {
    delete ui;
}

void UserPage::user2menu() {
    mainWindow->showMenuPage();
}

void UserPage::showDomesticMap() {
    mainWindow->map_type = Flight_Ticket_Management_System::DOMESTIC;
    mainWindow->showMapPage();
}

void UserPage::showGlobalMap() {
    mainWindow->map_type = Flight_Ticket_Management_System::GLOBAL;
    mainWindow->showMapPage();
}

void UserPage::displayOrders(QTabWidget* tabWidget, const QString& statusFilter) {
    QWidget *currentTab = tabWidget->currentWidget();
    QTreeWidget *targetTreeWidget = currentTab == tabWidget->widget(1) ? allOrdersTreeWidget : paidOrdersTreeWidget;
    targetTreeWidget->clear();

    for (const Order &order : mainWindow->currentUser.orderManager.getOrders()) {
        if (statusFilter.isEmpty() || order.getStatus() == statusFilter) {
            QTreeWidgetItem *parentItem = new QTreeWidgetItem;
            parentItem->setText(0, order.getOrderId());
            parentItem->setText(1, order.getPassenger().getFamilyName() + " " + order.getPassenger().getGivenName());
            parentItem->setText(2, order.getFlightRoute().showCityPath());
            if (ui->Orders->currentIndex() == 1) {
                parentItem->setText(3, order.getStatus());
            } else {
                parentItem->setText(3, "");
            }
            targetTreeWidget->addTopLevelItem(parentItem);

            QToolButton* refundButton = new QToolButton();
            refundButton->setText("退票");
            refundButton->setStyleSheet("QToolButton { color: red; }");
            connect(refundButton, &QToolButton::clicked, this, [this, orderId = order.getOrderId()]() {
                handleRefund(orderId);
            });
            targetTreeWidget->setItemWidget(parentItem,4, refundButton);

            QToolButton* rescheduleButton = new QToolButton();
            rescheduleButton->setText("改签");
            rescheduleButton->setStyleSheet("QToolButton { color: blue; }");
            connect(rescheduleButton, &QToolButton::clicked, this, [this, orderId = order.getOrderId()]() {
                handleReschedule(orderId);
            });
            targetTreeWidget->setItemWidget(parentItem, 5, rescheduleButton);
        }
    }
}

void UserPage::initOrderTab() {
    mainWindow->currentUser.loadUserOrders();
    ui->clearBtn->hide();

    QWidget *paidOrdersTab = ui->Orders->widget(0);
    QWidget *allOrdersTab = ui->Orders->widget(1);

    paidOrdersTreeWidget = new QTreeWidget(paidOrdersTab);
    paidOrdersTreeWidget->setColumnCount(6);
    paidOrdersTreeWidget->setHeaderLabels(QStringList() << "订单号" << "乘客" << "航班" << "" << "" << "");
    paidOrdersTreeWidget->setRootIsDecorated(false);
    paidOrdersTreeWidget->setColumnWidth(0, 290);
    paidOrdersTreeWidget->setColumnWidth(1, 140);
    paidOrdersTreeWidget->setColumnWidth(2, 260);
    paidOrdersTreeWidget->setColumnWidth(3, 0);
    paidOrdersTreeWidget->setColumnWidth(4, 70);
    paidOrdersTreeWidget->setColumnWidth(5, 70);
    QVBoxLayout *paidOrdersLayout = new QVBoxLayout(paidOrdersTab);
    paidOrdersLayout->addWidget(paidOrdersTreeWidget);
    paidOrdersTab->setLayout(paidOrdersLayout);

    allOrdersTreeWidget = new QTreeWidget(allOrdersTab);
    allOrdersTreeWidget->setColumnCount(4);
    allOrdersTreeWidget->setHeaderLabels(QStringList() << "订单号" << "乘客" << "航班" << "状态");
    allOrdersTreeWidget->setRootIsDecorated(false);
    allOrdersTreeWidget->setColumnWidth(0, 290);
    allOrdersTreeWidget->setColumnWidth(1, 140);
    allOrdersTreeWidget->setColumnWidth(2, 300);
    allOrdersTreeWidget->setColumnWidth(3, 60);
    QVBoxLayout *allOrdersLayout = new QVBoxLayout(allOrdersTab);
    allOrdersLayout->addWidget(allOrdersTreeWidget);
    allOrdersTab->setLayout(allOrdersLayout);
    paidOrdersTreeWidget->setStyleSheet(
        "QTreeWidget::item { height: 30px; border-bottom: 1px solid lightgray;}"
        "QTreeWidget::item:selected { background-color: lightblue; }"
        "QTreeWidget::item:hover { background-color: lightblue; }");
    allOrdersTreeWidget->setStyleSheet(
        "QTreeWidget::item { height: 30px; border-bottom: 1px solid lightgray;}"
        "QTreeWidget::item:selected { background-color: lightblue; }"
        "QTreeWidget::item:hover { background-color: lightblue; }");

    displayOrders(ui->Orders, "已支付");
    createContextMenu();
}

void UserPage::onTabChanged(int index) {
    QTabWidget *tabWidget = ui->Orders;
    if (index < tabWidget->count()) {
        if (tabWidget->tabText(index) == "全部") {
            ui->clearBtn->show();
            displayOrders(tabWidget, "");
        }
        if (tabWidget->tabText(index) == "已出票") {
            ui->clearBtn->hide();
            displayOrders(tabWidget, "已支付");
        }
    } else {
        qDebug() << "Invalid tab index";
    }
}

void UserPage::handleRefund(const QString& orderId) {
    bool orderFound = false;
    auto &orders = mainWindow->currentUser.orderManager.getOrders();
    for (auto& order : orders) {
        if (order.getOrderId() == orderId) {
            orderFound = true;
            if (order.getStatus() == "已支付") {
                order.setStatus("已退票");

                const FlightRoute& flightRoute = order.getFlightRoute();
                for (const std::shared_ptr<Flight> &flight : flightRoute) {
                    QString flightNumber = flight->getFlightNumber();
                    for (City* city : mainWindow->network.getCities()) {
                        FlightNode* node = city->flightsHead;
                        while (node) {
                            if (node->flight->getFlightNumber() == flightNumber) {
                                node->flight->setRemainSeatNum(node->flight->getRemainSeatNum() + 1);
                                //qDebug() << "Updated remaining seats for flight" << flightNumber << "to" << node->flight->getRemainSeatNum();
                                break;
                            }
                            node = node->next;
                        }
                    }
                }
                QString filePath = ORDER_PATH + mainWindow->currentUser.getID() + ".json";
                if (!mainWindow->currentUser.orderManager.saveOrdersToJsonFile(filePath)) {
                    QMessageBox::warning(this, "错误", "保存订单失败！");
                    return;
                } else {
                    qDebug() << "已保存到" << filePath;
                }
                mainWindow->network.writeFlightToFile(FLIGHT_FILE);
                QMessageBox::information(this, "操作成功", "退票成功！");
                displayOrders(ui->Orders, "已支付");
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
    auto &orders = mainWindow->currentUser.orderManager.getOrders();
    for (auto& order : orders) {
        if (order.getOrderId() == orderId) {
            orderFound = true;

            if (order.getStatus() == "已支付") {
                order.setStatus("已改签");

                mainWindow->orderType = Flight_Ticket_Management_System::RESCHEDULE_ORDER;
                mainWindow->rescheduleOrder = order;
                const FlightRoute& flightRoute = order.getFlightRoute();
                if (!flightRoute.isEmpty()) {
                    auto departureCity = flightRoute.first()->getDepartureCity();
                    auto arrivalCity = flightRoute.last()->getArrivalCity();
                    auto departureDate = QDate::fromString(flightRoute.first()->getDepartureTime().section(" ", 0, 0), "yyyy-MM-dd");
                    mainWindow->initReschedule(departureCity, arrivalCity, departureDate);
                } else {
                    QMessageBox::warning(this, "错误", "该订单没有包含任何航班！");
                    return;
                }
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

void UserPage::deleteAllOrder() {
    QTabWidget *tabWidget = ui->Orders;
    displayOrders(tabWidget, "exception");
    QMessageBox::information(this, "删除成功", "已删除所有订单记录！");
    mainWindow->currentUser.orderManager.clear();

    QString filePath = ORDER_PATH + mainWindow->currentUser.getID() + ".json";
    clearJsonFile(filePath);
}

void UserPage::createContextMenu() {
    QMenu *contextMenu = new QMenu(this);
    QAction *detailsAction = new QAction("详细信息", this);
    contextMenu->addAction(detailsAction);
    connect(detailsAction, &QAction::triggered, this, &UserPage::onDetailsActionTriggered);

    paidOrdersTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(paidOrdersTreeWidget, &QTreeWidget::customContextMenuRequested, [this, contextMenu](const QPoint&pos) {
        QTreeWidgetItem *item = paidOrdersTreeWidget->itemAt(pos);
        if (item) {
            contextMenu->exec(paidOrdersTreeWidget->viewport()->mapToGlobal(pos));
        }
    });

    QMenu *contextMenu2 = new QMenu(this);
    QAction *deleteAction = new QAction("删除订单", this);
    contextMenu2->addAction(deleteAction);
    connect(deleteAction, &QAction::triggered, this, &UserPage::deleteSingleOrder);

    allOrdersTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(allOrdersTreeWidget, &QTreeWidget::customContextMenuRequested, [this, contextMenu2](const QPoint& pos) {
        QTreeWidgetItem *item = allOrdersTreeWidget->itemAt(pos);
        if (item) {
            contextMenu2->exec(allOrdersTreeWidget->viewport()->mapToGlobal(pos));
        }
    });
}

void UserPage::deleteSingleOrder() {
    QTreeWidgetItem *currentItem = allOrdersTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "错误", "未选择订单！");
        return;
    }

    QString orderId = currentItem->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "您确定要删除此订单吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        auto &orders = mainWindow->currentUser.orderManager.getOrders();
        auto it = std::remove_if(orders.begin(), orders.end(), [&orderId](const Order&order) {
            return order.getOrderId() == orderId;
        });

        if (it != orders.end()) {
            orders.erase(it, orders.end());
            QString filePath = ORDER_PATH + mainWindow->currentUser.getID() + ".json";
            if (!mainWindow->currentUser.orderManager.saveOrdersToJsonFile(filePath)) {
                QMessageBox::warning(this, "错误", "保存订单失败！");
                return;
            }

            displayOrders(ui->Orders, "");
            QMessageBox::information(this, "删除成功", "订单已成功删除！");
        } else {
            QMessageBox::warning(this, "错误", "未找到订单！");
        }
    }
}

void UserPage::onDetailsActionTriggered() {
    QTreeWidgetItem *currentItem = paidOrdersTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "错误", "未选择订单！");
        return;
    }

    QString orderId = currentItem->text(0);
    auto &orders = mainWindow->currentUser.orderManager.getOrders();
    for (const auto& order : orders) {
        if (order.getOrderId() == orderId) {
            showOrderDetails(orderId);
            return;
        }
    }
    QMessageBox::warning(this, "错误", "未找到订单！");
}

void UserPage::showOrderDetails(const QString& orderId) {
    auto &orders = mainWindow->currentUser.orderManager.getOrders();
    for (const auto& order : orders) {
        if (order.getOrderId() == orderId) {
            OrderDetailsDialog dialog(order, this);
            dialog.exec();
            return;
        }
    }
    QMessageBox::warning(this, "错误", "未找到订单！");
}


