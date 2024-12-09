#include "pages/InfoPage.h"
#include <QMessageBox>
#include "Utils.h"
#include "qboxlayout.h"
#include "ui_infopage.h"
#include "widgets/CabinClassBookingDialog.h"

InfoPage::InfoPage(Flight_Ticket_Management_System* mainWindow, QWidget* parent)
    : QWidget(parent), ui(new Ui::InfoPage), mainWindow(mainWindow) {
    ui->setupUi(this);

    initInfoPage();
    connect(ui->backBtn, SIGNAL(released()), this, SLOT(info2search()));
    setupTreeWidget();
}

InfoPage::~InfoPage() { delete ui; }

void InfoPage::setupTreeWidget() {
    ui->flightTreeWidget->setSelectionMode(
	QAbstractItemView::SelectionMode::SingleSelection);
    ui->flightTreeWidget->setColumnCount(5);
    ui->flightTreeWidget->setHeaderLabels({"  航班信息", "", "", "", ""});
    ui->flightTreeWidget->setRootIsDecorated(false);
}

void InfoPage::initInfoPage() {
    originalFlights = mainWindow->searchedFlights;
    filteredFlights = originalFlights;
    ui->showCityLbl->setText(mainWindow->depCity + "    \t\t\t\t      " + mainWindow->arrCity);
    ui->backBtn->setIcon(QIcon(":/images/resources/images/back.png"));
    ui->backBtn->setIconSize(QSize(32, 32));
    ui->backBtn->setStyleSheet("QToolButton { border: none; background: none; }");
    ui->backBtn->setAutoRaise(true);

    QLabel* imageLabel = new QLabel(this);
    QPixmap pixmap(":/images/resources/images/take_off.png");
    imageLabel->setGeometry(490, 30, 60, 40);
    imageLabel->setPixmap(pixmap.scaled(60, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QButtonGroup* buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(ui->intel_sort, 1);
    buttonGroup1->addButton(ui->time_sort, 2);
    buttonGroup1->addButton(ui->early_sort, 3);
    buttonGroup1->addButton(ui->price_sort, 4);
    buttonGroup1->setExclusive(true);
    connect(buttonGroup1, &QButtonGroup::idClicked, this,
	    &InfoPage::handleSortButtonClick);

    ui->checkBox1->setChecked(true);
    ui->checkBox2->setChecked(true);
    ui->checkBox3->setChecked(true);
    connect(ui->checkBox1, &QCheckBox::stateChanged, this,
	    &InfoPage::applyFilters);
    connect(ui->checkBox2, &QCheckBox::stateChanged, this,
	    &InfoPage::applyFilters);
    connect(ui->checkBox3, &QCheckBox::stateChanged, this,
	    &InfoPage::applyFilters);

    initAirlineArea();
}

void InfoPage::info2search() {
    currentSortType = SORT_NORMAL;
    mainWindow->showSearchPage();
}

void InfoPage::initAirlineArea() {
    for(int i = 0; i < mainWindow->searchedFlights.size(); ++i) {
        for(int j = 0; j < mainWindow->searchedFlights[i].getFlightCount(); ++j) {
            if(mainWindow->searchedFlights[i][j] != nullptr) {
                Airlines.insert(
                    mainWindow->searchedFlights[i][j]->getAirline());
            }
        }
    }
    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->airlineArea->setWidgetResizable(true);
    foreach(const QString& text, Airlines) {
	QCheckBox* checkBox = new QCheckBox(text);
	checkBox->setChecked(true);
	layout->addWidget(checkBox);
	connect(checkBox, &QCheckBox::stateChanged, this,
		&InfoPage::applyFilters);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void InfoPage::displayFlights(QVector<FlightRoute>& flights) {
    ui->flightTreeWidget->setUpdatesEnabled(false);
    ui->flightTreeWidget->clear();
    int row = 0;
    for(auto& flightPath : flights) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->flightTreeWidget);
        addFlightPathToTree(item, flightPath, row);
        row++;
    }

    ui->flightTreeWidget->resizeColumnToContents(0);
    ui->flightTreeWidget->setUpdatesEnabled(true);
    ui->flightTreeWidget->header()->setStyleSheet("QHeaderView::section { border: none; }");
}

void InfoPage::addFlightPathToTree(QTreeWidgetItem* item,
				   FlightRoute& flightPath, const int row) {
    QString cities;
    cities.append(flightPath.first()->getDepartureCity());
    for(int i = 0; i < flightPath.getTransferCount(); ++i) {
        cities.append(" -> " + flightPath[i]->getArrivalCity());
    }
    cities.append(" -> " + flightPath.last()->getArrivalCity());

    QString priceText;
    double originalPrice = flightPath.getTotalPrice();
    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    if (vipLevel > 0) {
        double discountedPrice = originalPrice * discount;
        priceText = QString("<s>￥%1</s> <br> <b> ￥%2 <b> 起")
                        .arg(originalPrice, 0, 'f', 2)
                        .arg(discountedPrice, 0, 'f', 2);
    } else {
        priceText = QString("￥%1 起").arg(originalPrice, 0, 'f', 2);
    }

    item->setText(0, cities);
    item->setText(1, "\t\t");
    int hours = flightPath.getTotalTransferTime() / 3600;
    int minutes = flightPath.getTotalTransferTime() % 3600 / 60;

    QString transferTime = "";
    if(flightPath.getTransferCount() > 0)
        transferTime = " (中转" + QString::number(hours) + "h " + QString::number(minutes) + "m)";
    item->setText(2, "总时长：" + formatMinutes(
                         Duration(flightPath.first()->getDepartureTime(), flightPath.last()->getArrivalTime())) +
                         "\t\n" + transferTime );
    ui->flightTreeWidget->setItemWidget(item, 3, createPriceLabel(priceText));

    QFont font("Microsoft YaHei UI", 10, QFont::Bold);
    item->setFont(0, font);

    QWidget* buttonWidget = new QWidget();
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
    QPushButton* bookButton = new QPushButton("订票");
    bookButton->setStyleSheet(
        "QPushButton {"
        "background-color: #5CACEE;"
        "color: #FFFFFF;"
        "border: 2px solid #5CACEE;"
        "border-radius: 10px;"
        "font: bold 14px;"
        "}"
        "QPushButton:hover {"
        "background-color: #2E8DEF;"
        "border-color: #2E8DEF;"
        "}"
        "QPushButton:pressed {"
        "background-color: #1D68B3;"
        "border-color: #1D68B3;"
        "}"
        );

    bookButton->setFixedSize(50, 30);
    buttonLayout->addStretch();
    buttonLayout->addWidget(bookButton);
    bookButton->setProperty("rowIndex", row);
    ui->flightTreeWidget->setItemWidget(item, 4, buttonWidget);
    connect(bookButton, &QPushButton::clicked, this,
	    [this, row]() { handleBookButtonClick(row); });

    for(auto& flight : flightPath) {
        QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
        for(int i = 0; i < ui->flightTreeWidget->columnCount(); ++i) {
            childItem->setSizeHint(i, QSize(0, 30));
        }

        childItem->setText(0, flight->getAirline() + " " + flight->getFlightNumber());
        childItem->setText(1, "出发：" + flight->getDepartureTime());
        childItem->setText(2, "到达：" + flight->getArrivalTime());

        double originalFlightPrice = flight->getPrice();
        double discountedFlightPrice = originalFlightPrice * discount;
        QString flightPriceText;
        if (vipLevel > 0) {
            flightPriceText = QString("<s>￥%1</s> <br> <b>￥%2 <b>起")
                                      .arg(originalFlightPrice, 0, 'f', 2)
                                      .arg(discountedFlightPrice, 0, 'f', 2);
        } else {
            flightPriceText = QString("￥%1 起").arg(originalFlightPrice, 0, 'f', 2);
        }
        ui->flightTreeWidget->setItemWidget(childItem, 3, createPriceLabel(flightPriceText));
        QFont font("Microsoft YaHei UI", 8, QFont::Medium);
        childItem->setFont(0, font);
    }
    for (int i = 0; i < ui->flightTreeWidget->columnCount(); ++i) {
        ui->flightTreeWidget->resizeColumnToContents(i);
    }
    ui->flightTreeWidget->header()->setStretchLastSection(true);
}

void InfoPage::handleBookButtonClick(int row) {
    int vipLevel = mainWindow->currentUser.getVIPLevel();
    double discount = std::max(0.1, 1.0 - vipLevel * 0.05);
    CabinClassBookingDialog dialog(filteredFlights[row], discount, this);
    connect(&dialog, &CabinClassBookingDialog::cabinClassSelected, this, [this, row](const QString& cabinClass) {
        handleCabinClassSelected(cabinClass, row);
    });
    if (dialog.exec() == QDialog::Accepted) {
        mainWindow->selectedFlight = filteredFlights[row];
        mainWindow->showCheckoutPage();
    }
}

void InfoPage::handleCabinClassSelected(const QString& cabinClass, int row) {
    mainWindow->selectedClass = cabinClass;
    mainWindow->selectedFlight = filteredFlights[row];
    mainWindow->showCheckoutPage();
}

void InfoPage::handleSortButtonClick(int buttonId) {
    switch(buttonId) {
    case 1:
        currentSortType = SORT_BY_PERSON;
        break;
    case 2:
	    currentSortType = SORT_BY_DURA;
	    break;
    case 3:
	    currentSortType = SORT_BY_TIME;
	    break;
    case 4:
	    currentSortType = SORT_BY_PRICE;
	    break;
	default:
	    return;
    }
    if(currentSortType != SORT_NORMAL) {
        filteredFlights = mainWindow->network.sortFlights(mainWindow->currentUser, filteredFlights, currentSortType);
    }
    displayFlights(filteredFlights);
}

void InfoPage::applyFilters() {
    filteredFlights.clear();
    for(auto& flightPath : originalFlights) {
        bool isValid = true;
        int stopNum = flightPath.getTransferCount();
        if((stopNum == 0 && !ui->checkBox1->isChecked()) ||
           (stopNum == 1 && !ui->checkBox2->isChecked()) ||
           (stopNum >= 2 && !ui->checkBox3->isChecked())) {
            isValid = false;
        }
        if(isValid) {
            for(auto& flight : flightPath) {
                bool airlineChecked = false;
                QLayout* layout = ui->scrollAreaWidgetContents->layout();
                for(int i = 0; i < layout->count(); ++i) {
                    QCheckBox* checkbox = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
                    if(checkbox && checkbox->text() == flight->getAirline()) {
                        airlineChecked = checkbox->isChecked();
                        break;
                    }
                }
                if(!airlineChecked) {
                    isValid = false;
                    break;
                }
            }
        }
        if(isValid) {
            filteredFlights.append(flightPath);
        }
    }
    if(currentSortType != SORT_NORMAL) {
        filteredFlights = mainWindow->network.sortFlights(mainWindow->currentUser, filteredFlights, currentSortType);
    }
    displayFlights(filteredFlights);
}

QLabel* InfoPage::createPriceLabel(const QString& text) {
    QLabel* label = new QLabel(text);
    label->setTextFormat(Qt::RichText);
    return label;
}
