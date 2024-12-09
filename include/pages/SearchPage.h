#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qtreewidget.h"
#include "widgets/SearchableComboBox.h"

namespace Ui {
class SearchPage;
}

class SearchPage : public QWidget {
    Q_OBJECT

public:
    explicit SearchPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~SearchPage();
    void initSearchPage();

    void setDataForReschedule(const QString& dep, const QString& arr, const QDate& date);
    void loadCitiesIntoComboBox(QComboBox* comboBox, const QVector<QString>& cityNames);
    void loadRecommendation();
    void displayRecommendation();
    void addFlightPathToTree(QTreeWidget* treeView, const FlightRoute& flightPath, int& row);

private:
    Ui::SearchPage *ui;
    Flight_Ticket_Management_System *mainWindow;

    SearchableComboBox* depComboBox;
    SearchableComboBox* arrComboBox;


private slots:
    void showCalendar();
    void updateButtonWithDate(const QDate &date);
    void Exchange();
    void Back();
    void searchFlights();
    void updateCityWeather(QLabel* weatherLabel, const QString& cityName);
    void onBookButtonClicked(int row);
    void handleCabinClassSelected(const QString& cabinClass, int row);
    void showGraph();

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void pythonScriptOutputReceived(const QString &output);

};

#endif // SEARCHPAGE_H
