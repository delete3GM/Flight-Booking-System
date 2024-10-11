#pragma once

#include <QList>
#include <QString>
#include <QTextStream>
#include <QFile>
#include "Flight.h"

class User {
private:
    QString familyName;
    QString givenName;
    QString sex;
    QString id;
    QString phone;

public:
    User();
    User(QString f, QString g, QString s, QString i, QString p);

    QString getFamilyName() const;
    QString getGivenName() const;
    QString getSex() const;
    QString getId() const;
    QString getPhone() const;
    void setFamilyName(const QString &f) ;
    void setGivenName(const QString &g) ;
    void setSex(const QString &s) ;
    void setId(const QString &i) ;
    void setPhone(const QString &p) ;
    QString toString()const;

    void addFlightRecord(const Flight& flight);
    static void saveUser(const QList<User>& userList);
};
extern QList<User> userList;

