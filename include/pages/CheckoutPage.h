#ifndef CHECKOUTPAGE_H
#define CHECKOUTPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class CheckoutPage;
}

class CheckoutPage : public QWidget {
    Q_OBJECT

public:
    explicit CheckoutPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~CheckoutPage();

    void initCheckout();

    Passenger createPassenger();
    Order createOrder(const Passenger& passenger);
    void handleRescheduleOrder();
    void handleNewOrder(const Order& order);
    void updateFlightSeats(const Order& order);
    void increaseSeatsForRescheduledFlight();
    void increaseSeatsForFlight(const std::shared_ptr<Flight> flight);
    bool saveOrderToFile(const Order& order);
    void resetGenderRadioButtons();
    void loadPassengerHistory();
    void fillPassengerInfo(const Passenger &passenger);
    void initPassengerArea();
    QString detailInfo();

private:
    Ui::CheckoutPage *ui;
    Flight_Ticket_Management_System *mainWindow;
    QList<Passenger> passengerList;
    QButtonGroup *passengerButtonGroup;

    QString meal = "无餐食";

 private slots:
    void addPassenger();
    void checkout2info();
    void changeNoFood();
    void changeNormalFood();
    void changePlusFood();



};

#endif // CHECKOUTPAGE_H
