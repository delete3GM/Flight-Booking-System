#ifndef INFOPAGE_H
#define INFOPAGE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "flight_ticket_management_system.h"

namespace Ui {
class InfoPage;
}

class InfoPage : public QWidget {
    Q_OBJECT

public:
    explicit InfoPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~InfoPage();
    void initInfoPage();
    void setupTreeWidget();
    void addFlightPathToTree(QTreeWidgetItem* item, FlightRoute& flightPath, const int row);
    void displayFlights(QVector<FlightRoute>& flights);
    void initAirlineArea();

private:
    Ui::InfoPage *ui;
    Flight_Ticket_Management_System *mainWindow;
    QSet<QString> Airlines;
    QVector<FlightRoute> originalFlights;  // 原始搜索结果的备份
    QVector<FlightRoute> filteredFlights;  // 当前筛选后的结果
    SORT_TYPE currentSortType = SORT_NORMAL;


private slots:
    void handleBookButtonClick(int row);
    void handleSortButtonClick(int buttonId);
    void info2search();
    void applyFilters();

};

#endif // INFOPAGE_H
