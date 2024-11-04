#include "Passenger.h"


Passenger::Passenger() {}

Passenger::Passenger(QString f, QString g, QString s, QString i, QString p)
    : familyName(f), givenName(g), sex(s), id(i), phone(p) {}

QString Passenger::getFamilyName() const {
    return familyName;
}

QString Passenger::getGivenName() const {
    return givenName;
}

QString Passenger::getSex() const {
    return sex;
}

QString Passenger::getId() const {
    return id;
}

QString Passenger::getPhone() const {
    return phone;
}

void Passenger::setFamilyName(const QString &f) {
    familyName = f;
}

void Passenger::setGivenName(const QString &g) {
    givenName = g;
}

void Passenger::setSex(const QString &s) {
    sex = s;
}

void Passenger::setId(const QString &i) {
    id = i;
}

void Passenger::setPhone(const QString &p) {
    phone = p;
}

QString Passenger::toString() const {
    return "Family Name: " + familyName + ", Given Name: " + givenName + ", Sex: " + sex + ", ID: " + id + ", Phone: " + phone;
}

bool Passenger::operator==(const Passenger &other) const {
    return this->getId() == other.getId();
}

