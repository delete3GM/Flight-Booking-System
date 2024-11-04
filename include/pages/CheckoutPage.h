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
    void handleRescheduleOrder(const Order& order);
    void handleNewOrder(const Order& order);
    void updateFlightSeats(const Order& order);
    void increaseSeatsForRescheduledFlight();
    void increaseSeatsForFlight(const Flight* flight);
    bool saveOrderToFile(const Order& order);
    void resetGenderRadioButtons();
    void loadPassengerHistory();
    void fillPassengerInfo(const Passenger &passenger);
    void initPassengerArea();

private:
    Ui::CheckoutPage *ui;
    Flight_Ticket_Management_System *mainWindow;

    QList<Passenger> passengerList;
    QButtonGroup *passengerButtonGroup;

 private slots:
    void addPassenger();
    void checkout2info();



};

#endif // CHECKOUTPAGE_H
