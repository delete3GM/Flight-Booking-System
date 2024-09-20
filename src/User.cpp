#include "User.h"
#include "Flight.h"


User::User() : familyName(""), givenName(""), sex(""), id(""), phone("") {}

User::User(QString f, QString g, QString s, QString i, QString p)
    : familyName(f), givenName(g), sex(s), id(i), phone(p) {}

QString User::getFamilyName() const
{
    return familyName;
}

QString User::getGivenName() const
{
    return givenName;
}

QString User::getSex() const
{
    return sex;
}

QString User::getId() const
{
    return id;
}

QString User::getPhone() const
{
    return phone;
}

void User::setFamilyName(const QString &f)
{
    familyName = f;
}

void User::setGivenName(const QString &g)
{
    givenName = g;
}

void User::setSex(const QString &s)
{
    sex = s;
}

void User::setId(const QString &i)
{
    id = i;
}

void User::setPhone(const QString &p)
{
    phone = p;
}

QString User::toString() const
{
    return "Family Name: " + familyName + ", Given Name: " + givenName + ", Sex: " + sex + ", ID: " + id + ", Phone: " + phone;
}

