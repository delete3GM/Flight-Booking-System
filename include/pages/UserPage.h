#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class UserPage;
}

class UserPage : public QWidget {
    Q_OBJECT

public:
    explicit UserPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~UserPage();
    void handleRefund(const QString& orderId);
    void handleReschedule(const QString& orderId);


private:
    Ui::UserPage *ui;
    Flight_Ticket_Management_System *mainWindow;

private slots:
    void displayOrders(QTabWidget* tabWidget, const QString& statusFilter);
    void onTabChanged(int index);
    void initOrderTab();
    void user2menu();
    void user2map();
};

#endif // USERPAGE_H
