#ifndef CABINCLASSBOOKINGDIALOG_H
#define CABINCLASSBOOKINGDIALOG_H

#include "FlightRoute.h"
#include "qlabel.h"
#include <QDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>

class CabinClassBookingDialog : public QDialog {
    Q_OBJECT

public:
    explicit CabinClassBookingDialog(const FlightRoute& flight, const double discount, QWidget *parent = nullptr);

private slots:
    void onEconomyClassBookClicked();
    void onBusinessClassBookClicked();
    void onFirstClassBookClicked();
    void handleBookButtonClick(const QString& cabinClass);

private:
    FlightRoute selectedFlight;
    QLabel *flightInfoLabel;
    QLabel *imageLabel;
    QTableWidget *cabinClassTable;
    QVBoxLayout *layout;
    QPushButton *economyClassBookButton;
    QPushButton *superEconomyClassBookButton;
    QPushButton *businessClassBookButton;

    void setupUI();
    void populateCabinClasses(const double discount);

signals:
    void cabinClassSelected(const QString& cabinClass);
};

#endif // CABINCLASSBOOKINGDIALOG_H
