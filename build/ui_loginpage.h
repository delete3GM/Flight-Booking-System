/********************************************************************************
** Form generated from reading UI file 'loginpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGE_H
#define UI_LOGINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginPage
{
public:
    QLineEdit *typeAcnt;
    QLineEdit *typePsw;
    QLabel *loginTitleLbl;
    QPushButton *loginBtn;
    QPushButton *signupBtn;
    QLabel *label;
    QLabel *changePswBtn;

    void setupUi(QWidget *LoginPage)
    {
        if (LoginPage->objectName().isEmpty())
            LoginPage->setObjectName("LoginPage");
        LoginPage->resize(960, 540);
        LoginPage->setMinimumSize(QSize(960, 540));
        LoginPage->setMaximumSize(QSize(960, 540));
        typeAcnt = new QLineEdit(LoginPage);
        typeAcnt->setObjectName("typeAcnt");
        typeAcnt->setGeometry(QRect(370, 240, 221, 31));
        QFont font;
        font.setPointSize(11);
        typeAcnt->setFont(font);
        typeAcnt->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        typeAcnt->setClearButtonEnabled(true);
        typePsw = new QLineEdit(LoginPage);
        typePsw->setObjectName("typePsw");
        typePsw->setGeometry(QRect(370, 310, 221, 31));
        typePsw->setFont(font);
        typePsw->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        loginTitleLbl = new QLabel(LoginPage);
        loginTitleLbl->setObjectName("loginTitleLbl");
        loginTitleLbl->setGeometry(QRect(340, 100, 281, 51));
        QFont font1;
        font1.setPointSize(25);
        loginTitleLbl->setFont(font1);
        loginTitleLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginBtn = new QPushButton(LoginPage);
        loginBtn->setObjectName("loginBtn");
        loginBtn->setGeometry(QRect(370, 380, 91, 31));
        QFont font2;
        font2.setPointSize(14);
        loginBtn->setFont(font2);
        signupBtn = new QPushButton(LoginPage);
        signupBtn->setObjectName("signupBtn");
        signupBtn->setGeometry(QRect(500, 380, 91, 31));
        signupBtn->setFont(font2);
        label = new QLabel(LoginPage);
        label->setObjectName("label");
        label->setGeometry(QRect(240, 210, 481, 251));
        label->setStyleSheet(QString::fromUtf8("QLabel { background-color: lightgray; }"));
        changePswBtn = new QLabel(LoginPage);
        changePswBtn->setObjectName("changePswBtn");
        changePswBtn->setGeometry(QRect(550, 350, 71, 16));
        label->raise();
        typeAcnt->raise();
        typePsw->raise();
        loginTitleLbl->raise();
        loginBtn->raise();
        signupBtn->raise();
        changePswBtn->raise();

        retranslateUi(LoginPage);

        QMetaObject::connectSlotsByName(LoginPage);
    } // setupUi

    void retranslateUi(QWidget *LoginPage)
    {
        LoginPage->setWindowTitle(QCoreApplication::translate("LoginPage", "Form", nullptr));
        typePsw->setInputMask(QString());
        loginTitleLbl->setText(QCoreApplication::translate("LoginPage", "\346\234\272\347\245\250\351\242\204\350\256\242\347\263\273\347\273\237", nullptr));
        loginBtn->setText(QCoreApplication::translate("LoginPage", "\347\231\273\345\275\225", nullptr));
        signupBtn->setText(QCoreApplication::translate("LoginPage", "\346\263\250\345\206\214", nullptr));
        label->setText(QString());
        changePswBtn->setText(QCoreApplication::translate("LoginPage", "\345\277\230\350\256\260\345\257\206\347\240\201\357\274\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginPage: public Ui_LoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGE_H
