#include "CabinClassBookingDialog.h"
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>

CabinClassBookingDialog::CabinClassBookingDialog(const FlightRoute& flights, const double discount, QWidget *parent)
    : QDialog(parent), selectedFlight(flights) {
    setupUI();
    populateCabinClasses(discount);
}

void CabinClassBookingDialog::setupUI() {
    setWindowTitle("选择舱位");
    layout = new QVBoxLayout(this);

    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(300, 300);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(QPixmap(":/images/resources/images/economy_class.png"));
    layout->addWidget(imageLabel);

    cabinClassTable = new QTableWidget(this);
    cabinClassTable->setColumnCount(4);
    cabinClassTable->setHorizontalHeaderLabels(QStringList() << "舱位" << "价格" << "图片" << "预订");
    cabinClassTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(cabinClassTable);
    cabinClassTable->verticalHeader()->setVisible(false);
    cabinClassTable->setFixedHeight(116);
}

void CabinClassBookingDialog::populateCabinClasses(const double discount) {
    QStringList cabinClasses = {"经济舱", "商务舱", "头等舱"};
    QMap<QString, double> prices = {
        {"经济舱", selectedFlight.getTotalPrice() * discount},
        {"商务舱", selectedFlight.getTotalPrice() * 1.8 * discount},
        {"头等舱", selectedFlight.getTotalPrice() * 2.4 * discount}
    };
    QMap<QString, QString> imagePaths = {
        {"经济舱", ":/images/resources/images/economy_class.png"},
        {"商务舱", ":/images/resources/images/business_class.png"},
        {"头等舱", ":/images/resources/images/first_class.png"}
    };

    cabinClassTable->setRowCount(cabinClasses.size());
    cabinClassTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < cabinClasses.size(); ++i) {
        QTableWidgetItem *cabinClassItem = new QTableWidgetItem(cabinClasses[i]);
        cabinClassItem->setFlags(cabinClassItem->flags() & ~Qt::ItemIsSelectable);
        cabinClassTable->setItem(i, 0, cabinClassItem);

        QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(prices[cabinClasses[i]], 'f', 2));
        priceItem->setFlags(cabinClassItem->flags() & ~Qt::ItemIsSelectable);
        cabinClassTable->setItem(i, 1, priceItem);

        QPushButton *viewPhotosButton = new QPushButton("查看照片");
        cabinClassTable->setCellWidget(i, 2, viewPhotosButton);
        connect(viewPhotosButton, &QPushButton::clicked, [this, imagePath = imagePaths[cabinClasses[i]]]() {
            imageLabel->setPixmap(QPixmap(imagePath));
        });

        QPushButton *bookButton = new QPushButton("预订");
        bookButton->setStyleSheet("QPushButton { font-weight: bold; }");
        cabinClassTable->setCellWidget(i, 3, bookButton);
        connect(bookButton, &QPushButton::clicked, [this, cabinClass = cabinClasses[i]]() {
            emit cabinClassSelected(cabinClass);
            this->accept();
        });
    }
    cabinClassTable->setColumnWidth(0, 200);
    cabinClassTable->setColumnWidth(1, 100);
    cabinClassTable->setColumnWidth(2, 100);
    cabinClassTable->setColumnWidth(3, 100);
}

void CabinClassBookingDialog::handleBookButtonClick(const QString& cabinClass) {
    if (cabinClass == "经济舱") {
        onEconomyClassBookClicked();
    } else if (cabinClass == "商务舱") {
        onBusinessClassBookClicked();
    } else if (cabinClass == "头等舱") {
        onFirstClassBookClicked();
    }
}

void CabinClassBookingDialog::onEconomyClassBookClicked() {
    QMessageBox::information(this, "Booking", "You have booked an Economy Class ticket!");
    this->accept();
}

void CabinClassBookingDialog::onBusinessClassBookClicked() {
    QMessageBox::information(this, "Booking", "You have booked a Business Class ticket!");
    this->accept();
}

void CabinClassBookingDialog::onFirstClassBookClicked() {
    QMessageBox::information(this, "Booking", "You have booked a First Class ticket!");
    this->accept();
}
