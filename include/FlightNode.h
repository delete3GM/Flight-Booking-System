#pragma once

#include "Flight.h"

class FlightNode {
public:
    std::shared_ptr<Flight> flight;
    FlightNode* next;

    FlightNode() : flight(nullptr), next(nullptr) {}
    FlightNode(std::shared_ptr<Flight> f, FlightNode* n = nullptr) : flight(f), next(n) {}

};
