#ifndef SEARCHABLECOMBOBOX_H
#define SEARCHABLECOMBOBOX_H

#include <QComboBox>
#include <QCompleter>
#include <QMap>
#include "Utils.h"

class SearchableComboBox : public QComboBox {
    Q_OBJECT

public:
    explicit SearchableComboBox(QWidget *parent = nullptr);
    void initializeBox(const QVector<QString>& cities, const QMap<QString, CityInfo>& cityInfoMap);
    QString getCurrentCity() const;

private:
    QCompleter* completer;
    QMap<QString, CityInfo> cityInfoMap;
    void setupUI();
    void setupCompleter();

signals:
    void citySelected(const QString& city);

private slots:
    void onTextChanged(const QString& text);
};

#endif // SEARCHABLECOMBOBOX_H
