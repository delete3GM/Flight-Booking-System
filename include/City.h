#pragma once

#include <QObject>
#include <QString>
#include "FlightNode.h"

class City : public QObject {
    Q_OBJECT

public:
    QString name;
    FlightNode* flights;  // 链表头节点

    City(QString n) : name(n), flights(nullptr) {}
};

