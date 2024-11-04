#ifndef FLIGHTRECOMMENDATION_H
#define FLIGHTRECOMMENDATION_H

#include "Flight.h"
#include <QList>
#include <QMap>

class FlightRecommendation {
private:
    QList<Flight> flights;
    QMap<QString, double> scoreWeights; // Key: "price" or "time", Value: weight


    int calculateTotalTime(const Flight& flight);

public:
    FlightRecommendation();
    void addFlight(const Flight& flight);
    void setScoreWeight(const QString& factor, double weight);
    int calculateScore(const Flight& flight);
    Flight recommendBestFlight();

};

#endif // FLIGHTRECOMMENDATION_H
