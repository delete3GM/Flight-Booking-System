#include "flight_ticket_management_system.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Flight_Ticket_Management_System w;
    w.show();
    return a.exec();
}
