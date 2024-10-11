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

private:
    Ui::CheckoutPage *ui;
    Flight_Ticket_Management_System *mainWindow;

 private slots:
    void addPassenger();
    void checkout2info();



};

#endif // CHECKOUTPAGE_H
