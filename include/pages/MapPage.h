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

protected:
    QMap<QString, QPair<float, float>> cityCoordinates = {
        {"北京", {39.9042f, 116.4074f}},
        {"哈尔滨", {45.8038f, 126.5347f}},
        {"呼和浩特", {40.8424f, 111.7512f}},
        {"乌鲁木齐", {43.7930f, 87.6168f}},
        {"广州", {23.1291f, 113.2644f}},
        {"拉萨", {29.6420f, 91.1000f}},
        {"昆明", {25.0389f, 102.7183f}},
        {"成都", {30.5728f, 104.0668f}},
        {"上海", {31.2304f, 121.4737f}},
        {"南宁", {22.8195f, 108.3150f}}
    };

};

#endif // MAPPAGE_H
