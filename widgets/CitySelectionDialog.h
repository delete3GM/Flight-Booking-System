#ifndef CITYSELECTIONDIALOG_H
#define CITYSELECTIONDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>

class CitySelectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit CitySelectionDialog(const QVector<QString>& cities, QWidget* parent = nullptr);
    QString selectedCity() const { return m_selectedCity; }

signals:
    void citySelected(const QString& city);

private slots:
    void onCityButtonClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QString m_selectedCity;
    QGridLayout* m_layout;
    QVector<QString> m_cityNames;
};



#endif // CITYSELECTIONDIALOG_H
