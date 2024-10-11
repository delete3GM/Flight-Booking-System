#include "pages/InfoPage.h"
#include "ui_infopage.h"
#include <QMessageBox>

InfoPage::InfoPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::InfoPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    //按条件排序
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->default_sort, 1);
    buttonGroup->addButton(ui->time_sort, 2);
    buttonGroup->addButton(ui->early_sort, 3);
    buttonGroup->addButton(ui->price_sort, 4);
    ui->default_sort->setChecked(true);
    buttonGroup->setExclusive(true);

    connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(updateSearch(int)));
    connect(ui->backBtn, SIGNAL(released()), this, SLOT(info2search()));

}

InfoPage::~InfoPage() {
    delete ui;
}

void InfoPage::info2search() {
    mainWindow->showSearchPage();
    ui->default_sort->setChecked(true);
}

void InfoPage::handleTicketBooking(int rowIndex) {
    if (mainWindow->searchType == Flight_Ticket_Management_System::DIRECT) {
        QTableWidgetItem *item = ui->flightTableWidget->item(rowIndex, 1); // 假设航班号在第二列
        QTableWidgetItem *departureCity = ui->flightTableWidget->item(rowIndex, 2); // 第三列为出发地
        QTableWidgetItem *arrivalCity = ui->flightTableWidget->item(rowIndex, 3); // 第四列为目的地
        if (item) {
            QString flightNumber = item->text();
            // 因为这里存在转机的情况，所以不能根据最终出发地和最终目的地来判断
            QVector<Flight> currentFlights =
                mainWindow->network.searchFlights(departureCity->text(), arrivalCity->text(), mainWindow->selectedDate);

            for (const Flight &flight : currentFlights) {
                if (flight.getFlightNumber() == flightNumber) {
                    mainWindow->selectedFlight = flight;
                    qDebug() << mainWindow->selectedFlight.toString();
                    break;
                }
            }
            if (mainWindow->selectedFlight.getFlightNumber().isEmpty()) {
                QMessageBox::warning(this, "错误", "未找到对应的航班信息！");
                return;
            }
        }
    } else {
        QTableWidgetItem *item1 = ui->flightTableWidget->item(rowIndex, 1); // 假设航班号在第二列
        QTableWidgetItem *departureCity1 = ui->flightTableWidget->item(rowIndex, 2); // 第三列为出发地
        QTableWidgetItem *arrivalCity1 = ui->flightTableWidget->item(rowIndex, 3); // 第四列为目的地

        QTableWidgetItem *item2 = ui->flightTableWidget->item(rowIndex, 9); // 假设航班号在第二列
        QTableWidgetItem *departureCity2 = ui->flightTableWidget->item(rowIndex, 10); // 第三列为出发地
        QTableWidgetItem *arrivalCity2 = ui->flightTableWidget->item(rowIndex, 11); // 第四列为目的地

        if (item1 && item2) {
            QString flightNumber1 = item1->text();
            QString flightNumber2 = item2->text();
            // 因为这里存在转机的情况，所以不能根据最终出发地和最终目的地来判断
            QVector<Flight> currentFlights1 =
                mainWindow->network.searchFlights(departureCity1->text(), arrivalCity1->text(), mainWindow->selectedDate);

            for (const Flight &flight : currentFlights1) {
                if (flight.getFlightNumber() == flightNumber1 ) {
                    mainWindow->selectedFlight = flight;
                    qDebug() << mainWindow->selectedFlight.toString();
                    break;
                }
            }
            QVector<Flight> currentFlights2 =
                mainWindow->network.searchFlights(departureCity2->text(), arrivalCity2->text(), mainWindow->selectedDate);

            for (const Flight &flight : currentFlights2) {
                if (flight.getFlightNumber() == flightNumber2 ) {
                    mainWindow->selectedFlight2 = flight;
                    qDebug() << mainWindow->selectedFlight2.toString();
                    break;
                }
            }
            if (mainWindow->selectedFlight.getFlightNumber().isEmpty() || mainWindow->selectedFlight2.getFlightNumber().isEmpty()) {
                QMessageBox::warning(this, "错误", "未找到对应的航班信息！");
                return;
            }
        }
    }
    mainWindow->showCheckoutPage();
}
void InfoPage::updateTableWidget(const QVector<Flight> &flights) {
    // 如果是直飞
    if (mainWindow->searchType == Flight_Ticket_Management_System::DIRECT) {
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
            info2search();

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
                    });
                    tableWidget->setCellWidget(row, 8, buy_button);
                } else {
                    QLabel *no_ticket = new QLabel("无票");
                    no_ticket->setAlignment(Qt::AlignCenter);
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
            info2search();

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
                    });
                    tableWidget->setCellWidget(row/2, 16, buy_button);
                } else {
                    QLabel *no_ticket = new QLabel("无票");
                    no_ticket->setAlignment(Qt::AlignCenter);
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

void InfoPage::updateSearch(int buttonId) {
    // 获取SearchPage的数据
    QString depCity = mainWindow->depCity;
    QString arrCity = mainWindow->arrCity;
    QDate selectedDate = mainWindow->selectedDate;

  if(mainWindow->searchType == Flight_Ticket_Management_System::DIRECT)
  {
    // 搜索航班
    QVector<Flight> flights =
        mainWindow->network.searchFlights(depCity, arrCity, selectedDate);

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
        flights = mainWindow->network.sortFlights(flights, sortType);
    }
    // 更新表格显示
    updateTableWidget(flights);
  }
  else if(mainWindow->searchType == Flight_Ticket_Management_System::TRANSFER) {
    QVector<QPair<Flight, Flight>> flights = mainWindow->network.findTransferFlight(depCity, arrCity, selectedDate);
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
        flights = mainWindow->network.sortFlights(flights, sortType);
    }
    // 这里将 pair 转换为两个对象进行存储
    QVector<Flight> directFlights;
    for (auto flight : flights) {
      directFlights.push_back(flight.first);
      directFlights.push_back(flight.second);
    }
    qDebug() << "Search results count:" << flights.size();
    mainWindow->searchType = Flight_Ticket_Management_System::TRANSFER;
    updateTableWidget(directFlights);
  }
}




