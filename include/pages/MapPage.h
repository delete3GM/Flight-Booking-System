#ifndef MAPPAGE_H
#define MAPPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class MapPage;
}

class MapPage : public QWidget
{
    Q_OBJECT

public:
    explicit MapPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~MapPage();
    void showMap();

private:
    Ui::MapPage *ui;
    Flight_Ticket_Management_System *mainWindow;


private slots:
    void displayMap(int index);
    void map2user();


protected:
    QMap<QString, QPair<float, float>> cityCoordinates = {
                                                          QPair<QString, QPair<float, float>>(
                                                              "北京", QPair<float, float>(39.9042f, 116.4074f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "上海", QPair<float, float>(31.2304f, 121.4737f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "重庆", QPair<float, float>(29.5630f, 106.5516f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "南宁", QPair<float, float>(22.8195f, 108.3150f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "南京", QPair<float, float>(32.0603f, 118.7969f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "杭州", QPair<float, float>(30.2741f, 120.1551f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "广州", QPair<float, float>(23.1291f, 113.2644f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "昆明", QPair<float, float>(25.0389f, 102.7183f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "西安", QPair<float, float>(34.3416f, 108.9398f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "深圳", QPair<float, float>(22.5431f, 114.0579f)),
                                                          QPair<QString, QPair<float, float>>(
                                                              "成都", QPair<float, float>(30.5728f, 104.0668f))};
private:
    float departureCity_x = 0;
    float departureCity_y = 0;
    float arrivalCity_x = 0;
    float arrivalCity_y = 0;


};

#endif // MAPPAGE_H
