#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"
#include "qtreewidget.h"

namespace Ui {
class UserPage;
}

class UserPage : public QWidget {
    Q_OBJECT

public:
    explicit UserPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~UserPage();


private:
    Ui::UserPage *ui;
    Flight_Ticket_Management_System *mainWindow;
    QTreeWidget *allOrdersTreeWidget;
    QTreeWidget *paidOrdersTreeWidget;

private slots:
    void initOrderTab();
    void displayOrders(QTabWidget* tabWidget, const QString& statusFilter);
    void onTabChanged(int index);
    void user2menu();
    void showDomesticMap();
    void showGlobalMap();
    void handleRefund(const QString& orderId);
    void handleReschedule(const QString& orderId);
};

#endif // USERPAGE_H
