#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class MenuPage;
}

class MenuPage : public QWidget {
    Q_OBJECT

public:
    explicit MenuPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~MenuPage();


private:
    Ui::MenuPage *ui;
    Flight_Ticket_Management_System *mainWindow; // 指向主窗口的指针


private slots:
    void menu2search();
    void menu2user();
    void Exit();


};

#endif // MENUPAGE_H
