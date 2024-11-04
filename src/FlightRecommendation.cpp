#include "FlightRecommendation.h"
#include "Flight.h"
#include "Utils.h"

FlightRecommendation::FlightRecommendation() {}

int FlightRecommendation::calculateTotalTime(const Flight& flight) {
    //if(mainWindow->)
    return Duration(flight.getArrivalTime(), flight.getDepartureTime());
}

void FlightRecommendation::addFlight(const Flight& flight) {
    flights.append(flight);
}

void FlightRecommendation::setScoreWeight(const QString& factor, double weight) {
    scoreWeights[factor] = weight;
}

int FlightRecommendation::calculateScore(const Flight& flight) {
    int score = 0;

    if (scoreWeights.contains("price") && scoreWeights.contains("time")) {
        int timeScore = calculateTotalTime(flight) / 60;
        score += static_cast<int>(-scoreWeights["price"] * flight.getPrice());
        score += static_cast<int>(scoreWeights["time"] * timeScore);
    }

    return score;
}

Flight FlightRecommendation::recommendBestFlight() {
    int maxScore = -1;
    Flight bestFlight;

    for (const auto& flight : flights) {
        int score = calculateScore(flight);
        if (score > maxScore) {
            maxScore = score;
            bestFlight = flight;
        }
    }

    return bestFlight;
}
