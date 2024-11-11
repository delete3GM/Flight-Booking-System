#ifndef MAPPAGE_H
#define MAPPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class MapPage;
}

class MapPage : public QWidget {
    Q_OBJECT

public:
    explicit MapPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~MapPage();
    void initWebEngine();
    void configureWebEngine();
    void showMap();
    void initMapPage();
    void processLeafletMap(int index, QList<Order> orders);
    void processCesiumMap(int index, QList<Order> orders);
    void drawFlightRoutesLeaflet(const FlightRoute& flightRoute);
    void drawFlightRoutesCesium(const FlightRoute& flightRoute);

private:
    Ui::MapPage *ui;
    Flight_Ticket_Management_System *mainWindow;
    QList<Order> addedOrders;

    float departureCity_x = 0;
    float departureCity_y = 0;
    float arrivalCity_x = 0;
    float arrivalCity_y = 0;

private slots:
    void displayMap(int index);
    void map2user();
    void filterFlights();


};

#endif // MAPPAGE_H
