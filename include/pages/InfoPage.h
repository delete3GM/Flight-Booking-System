#ifndef INFOPAGE_H
#define INFOPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class InfoPage;
}

class InfoPage : public QWidget {
    Q_OBJECT

public:
    explicit InfoPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~InfoPage();
    void updateTableWidget(const QVector<Flight> &flights);


private:
    Ui::InfoPage *ui;
    Flight_Ticket_Management_System *mainWindow; // 指向主窗口的指针


private slots:
    void updateSearch(int buttonId);
    void handleTicketBooking(int rowIndex);
    void info2search();

};

#endif // INFOPAGE_H
