#pragma once

#include "Flight.h"

class FlightNode
{
public:
    Flight* flight;  // 指向航班对象的指针
    FlightNode* next;  // 指向下一个节点的指针

    FlightNode(Flight* f, FlightNode* n = nullptr) : flight(f), next(n) {}
};
