#pragma once

#include <QObject>
#include <QString>
#include "FlightNode.h"

class City : public QObject {
    Q_OBJECT

public:
    QString name;
    int flightNum;
    FlightNode* flightsHead;

    City(QString n) : name(n), flightsHead(nullptr) {}
};

