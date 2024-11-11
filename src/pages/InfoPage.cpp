#include "pages/InfoPage.h"

#include <QMessageBox>

#include "qboxlayout.h"
#include "ui_infopage.h"

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
    ui->flightTreeWidget->setHeaderLabels({"航班信息", "", "", "", ""});
    ui->flightTreeWidget->setRootIsDecorated(false);
    ui->flightTreeWidget->setStyleSheet(
    "QTreeWidget::item { height: 30px; border-bottom: 1px solid lightgray;}"
	"QTreeWidget::item:selected { background-color: lightblue; }"  // 选中时高亮
	"QTreeWidget::item:hover { background-color: lightblue; }");
}

void InfoPage::initInfoPage() {
    originalFlights = mainWindow->searchedFlights;
    filteredFlights = originalFlights;
    ui->showCityLbl->setText(mainWindow->depCity + " -----------------> " +
                 mainWindow->arrCity);

    QButtonGroup* buttonGroup1 = new QButtonGroup(this);
    buttonGroup1->addButton(ui->time_sort, 1);
    buttonGroup1->addButton(ui->early_sort, 2);
    buttonGroup1->addButton(ui->price_sort, 3);
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
    // ui->airlineArea->setFrameShape(QFrame::NoFrame);
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
}

// void InfoPage::addFlightPathToTree(QTreeWidgetItem* item, FlightRoute&
// flightPath, const int row) {
//     QString cities;
//     cities.append(flightPath.first()->getDepartureCity());
//     for (int i = 0; i < flightPath.getTransferCount(); ++i) {
//         cities.append(" -> " + flightPath[i]->getArrivalCity());
//     }
//     cities.append(" -> " + flightPath.last()->getArrivalCity());
//     QString sumPrice = "￥ " + QString::number(flightPath.getTotalPrice()) +
//     " 起";

//     item->setText(0, cities);
//     item->setText(4, sumPrice);

//     QWidget *buttonWidget = new QWidget();
//     QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
//     buttonLayout->setContentsMargins(0, 0, 0, 0);  // 移除边距
//     QPushButton* bookButton = new QPushButton("订票");
//     bookButton->setFixedSize(60, 20);
//     buttonLayout->addStretch();
//     buttonLayout->addWidget(bookButton);
//     bookButton->setProperty("rowIndex", row);
//     ui->flightTreeWidget->setItemWidget(item, 4, buttonWidget);
//     connect(bookButton, &QPushButton::clicked, this, [this, row]() {
//         handleBookButtonClick(row);
//     });

//     for (auto& flight : flightPath) {
//         QTreeWidgetItem *childItem = new QTreeWidgetItem(item);
//         childItem->setText(0, flight->getAirline() + " " +
//         flight->getFlightNumber()); childItem->setText(1,
//         flight->getDepartureTime()); childItem->setText(2,
//         flight->getArrivalTime()); childItem->setText(4, "￥ " +
//         QString::number(flight->getPrice()));
//     }
// }

void InfoPage::addFlightPathToTree(QTreeWidgetItem* item,
				   FlightRoute& flightPath, const int row) {
    QString cities;
    cities.append(flightPath.first()->getDepartureCity());
    for(int i = 0; i < flightPath.getTransferCount(); ++i) {
	cities.append(" -> " + flightPath[i]->getArrivalCity());
    }
    cities.append(" -> " + flightPath.last()->getArrivalCity());
    QString sumPrice =
	"￥ " + QString::number(flightPath.getTotalPrice()) + " 起";

    item->setText(0, cities);
    item->setText(1, "\t\t");
    item->setText(2, "\t\t");
    item->setText(3, sumPrice);

    QWidget* buttonWidget = new QWidget();
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);  // 移除边距
    QPushButton* bookButton = new QPushButton("订票");
    bookButton->setFixedSize(60, 20);
    buttonLayout->addStretch();
    buttonLayout->addWidget(bookButton);
    bookButton->setProperty("rowIndex", row);
    ui->flightTreeWidget->setItemWidget(item, 4, buttonWidget);
    connect(bookButton, &QPushButton::clicked, this,
	    [this, row]() { handleBookButtonClick(row); });

    for(auto& flight : flightPath) {
        QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
        childItem->setText(0, flight->getAirline() + " " + flight->getFlightNumber());
        childItem->setText(1, flight->getDepartureTime());
        childItem->setText(2, flight->getArrivalTime());
        childItem->setText(3, "￥ " + QString::number(flight->getPrice()));
    }
    for (int i = 0; i < ui->flightTreeWidget->columnCount(); ++i) {
        ui->flightTreeWidget->resizeColumnToContents(i);
    }

    ui->flightTreeWidget->header()->setStretchLastSection(true);
}

void InfoPage::handleBookButtonClick(int row) {
    mainWindow->selectedFlight = filteredFlights[row];
    mainWindow->showCheckoutPage();
}

void InfoPage::handleSortButtonClick(int buttonId) {
    switch(buttonId) {
	case 1:
	    currentSortType = SORT_BY_DURA;
	    break;
	case 2:
	    currentSortType = SORT_BY_TIME;
	    break;
	case 3:
	    currentSortType = SORT_BY_PRICE;
	    break;
	default:
	    return;
    }
    if(currentSortType != SORT_NORMAL) {
	filteredFlights =
	    mainWindow->network.sortFlights(filteredFlights, currentSortType);
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
		    QCheckBox* checkbox =
			qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
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
	filteredFlights =
	    mainWindow->network.sortFlights(filteredFlights, currentSortType);
    }
    displayFlights(filteredFlights);
}
