#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "flight_ticket_management_system.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(Flight_Ticket_Management_System *mainWindow, QWidget *parent = nullptr);
    ~LoginPage();
    void initLoginPage();
    QString getID() const;
    QString getPassword() const;
    void writeUserToFile(const QString& filename);
    bool isUserRegistered(const QString& id, const QString& psw);

private:
    Ui::LoginPage *ui;
    Flight_Ticket_Management_System *mainWindow;
    int loginAttempts = 0;
    static const int MAX_LOGIN_ATTEMPTS = 3;

private slots:
    void Login();
    void signUp();
    void changePsw();
    void showChangePasswordDialog();
    bool updatePassword(const QString &newPassword, const QString &confirmPassword);



};

#endif // LOGINPAGE_H
