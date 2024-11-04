#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <QString>

class UserPreferences {
private:
    double priceWeight;
    double timeWeight;
    double layoverWeight;

public:
    UserPreferences(double priceWeight = 1.0, double timeWeight = 1.0, double layoverWeight = 1.0);

    double getPriceWeight() const;
    double getTimeWeight() const;
    double getLayoverWeight() const;

    void setPriceWeight(double weight);
    void setTimeWeight(double weight);
    void setLayoverWeight(double weight);

private:
    UserPreferences getUserPreferencesFromOrder();

};

#endif // USERPREFERENCES_H
