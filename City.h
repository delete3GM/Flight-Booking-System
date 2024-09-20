// city.h
#pragma once
#include <QObject>
#include <QString>
#include "FlightNode.h"  // 引入FlightNode

class City : public QObject
{
    Q_OBJECT
public:
    QString name;
    //City* next;  // 指向下一个城市的指针
    FlightNode* flights;  // 链表头节点

    City(QString n) : name(n), flights(nullptr) {}
};

