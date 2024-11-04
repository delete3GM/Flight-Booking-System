#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"
#include "qcombobox.h"

namespace Ui {
class SearchPage;
}

class SearchPage : public QWidget {
    Q_OBJECT

public:
    explicit SearchPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~SearchPage();
    void initSearchPage();
    void initializeDepBox();
    void initializeArrBox();

    void setDataForReschedule(const QString& dep, const QString& arr, const QDate& date);
    void loadCitiesIntoComboBox(QComboBox* comboBox, const QVector<QString>& cityNames);

private:
    Ui::SearchPage *ui;
    Flight_Ticket_Management_System *mainWindow; // 指向主窗口的指针


private slots:
    void getDep(int index);
    void getArr(int index);
    void showCalendar();
    void updateButtonWithDate(const QDate &date);
    void Exchange();
    void Back();
    void callPythonScript(const QString& city);
    void searchFlights(); 
    void updateDepWeather(const QString &output);
    void updateArrWeather(const QString &output);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void pythonScriptOutputReceived(const QString &output);

};

#endif // SEARCHPAGE_H
