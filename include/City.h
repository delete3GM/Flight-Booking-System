#pragma once

#include <QObject>
#include <QString>
#include "FlightNode.h"

class City : public QObject {
    Q_OBJECT

public:
    QString name;
    FlightNode* flightsHead;  // 链表头节点

    City(QString n) : name(n), flightsHead(nullptr) {}
    // ~City() {
    //     FlightNode* current = flightsHead;
    //     while (current != nullptr) {
    //         FlightNode* next = current->next;
    //         delete current;
    //         current = next;
    //     }
    //     flightsHead = nullptr;
    // }
};

