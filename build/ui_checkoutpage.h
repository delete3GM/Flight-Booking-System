/********************************************************************************
** Form generated from reading UI file 'checkoutpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKOUTPAGE_H
#define UI_CHECKOUTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckoutPage
{
public:
    QRadioButton *isMale;
    QLineEdit *givenname;
    QPushButton *cancelBtn;
    QPushButton *submitBtn;
    QLineEdit *familyname;
    QLineEdit *passenger_phone;
    QLabel *flightInfo;
    QLineEdit *passenger_id;
    QRadioButton *isFemale;
    QLabel *passenger_1;
    QLabel *passenger_title;

    void setupUi(QWidget *CheckoutPage)
    {
        if (CheckoutPage->objectName().isEmpty())
            CheckoutPage->setObjectName("CheckoutPage");
        CheckoutPage->resize(960, 540);
        CheckoutPage->setMinimumSize(QSize(960, 540));
        isMale = new QRadioButton(CheckoutPage);
        isMale->setObjectName("isMale");
        isMale->setGeometry(QRect(410, 220, 51, 21));
        QFont font;
        font.setPointSize(13);
        isMale->setFont(font);
        isMale->setCheckable(true);
        isMale->setChecked(false);
        isMale->setAutoRepeat(false);
        givenname = new QLineEdit(CheckoutPage);
        givenname->setObjectName("givenname");
        givenname->setGeometry(QRect(680, 160, 161, 41));
        QFont font1;
        font1.setPointSize(14);
        givenname->setFont(font1);
        cancelBtn = new QPushButton(CheckoutPage);
        cancelBtn->setObjectName("cancelBtn");
        cancelBtn->setGeometry(QRect(750, 450, 111, 41));
        QFont font2;
        font2.setPointSize(15);
        cancelBtn->setFont(font2);
        submitBtn = new QPushButton(CheckoutPage);
        submitBtn->setObjectName("submitBtn");
        submitBtn->setGeometry(QRect(380, 450, 111, 41));
        submitBtn->setFont(font2);
        familyname = new QLineEdit(CheckoutPage);
        familyname->setObjectName("familyname");
        familyname->setGeometry(QRect(380, 160, 171, 41));
        familyname->setFont(font1);
        passenger_phone = new QLineEdit(CheckoutPage);
        passenger_phone->setObjectName("passenger_phone");
        passenger_phone->setGeometry(QRect(380, 360, 481, 41));
        passenger_phone->setFont(font1);
        flightInfo = new QLabel(CheckoutPage);
        flightInfo->setObjectName("flightInfo");
        flightInfo->setGeometry(QRect(30, 30, 301, 431));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223")});
        font3.setPointSize(12);
        font3.setBold(true);
        flightInfo->setFont(font3);
        flightInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        passenger_id = new QLineEdit(CheckoutPage);
        passenger_id->setObjectName("passenger_id");
        passenger_id->setGeometry(QRect(380, 260, 481, 41));
        passenger_id->setFont(font1);
        isFemale = new QRadioButton(CheckoutPage);
        isFemale->setObjectName("isFemale");
        isFemale->setGeometry(QRect(490, 220, 72, 21));
        isFemale->setFont(font);
        passenger_1 = new QLabel(CheckoutPage);
        passenger_1->setObjectName("passenger_1");
        passenger_1->setGeometry(QRect(380, 110, 61, 21));
        passenger_1->setFont(font1);
        passenger_title = new QLabel(CheckoutPage);
        passenger_title->setObjectName("passenger_title");
        passenger_title->setGeometry(QRect(370, 60, 81, 41));
        passenger_title->setFont(font2);

        retranslateUi(CheckoutPage);

        QMetaObject::connectSlotsByName(CheckoutPage);
    } // setupUi

    void retranslateUi(QWidget *CheckoutPage)
    {
        CheckoutPage->setWindowTitle(QCoreApplication::translate("CheckoutPage", "Form", nullptr));
        isMale->setText(QCoreApplication::translate("CheckoutPage", "\347\224\267", nullptr));
        givenname->setText(QString());
        cancelBtn->setText(QCoreApplication::translate("CheckoutPage", "\345\217\226\346\266\210", nullptr));
        submitBtn->setText(QCoreApplication::translate("CheckoutPage", "\346\217\220\344\272\244", nullptr));
        familyname->setInputMask(QString());
        familyname->setText(QString());
        flightInfo->setText(QString());
        isFemale->setText(QCoreApplication::translate("CheckoutPage", "\345\245\263", nullptr));
        passenger_1->setText(QCoreApplication::translate("CheckoutPage", "\344\271\230\345\256\2421", nullptr));
        passenger_title->setText(QCoreApplication::translate("CheckoutPage", "\344\271\230\345\256\242\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CheckoutPage: public Ui_CheckoutPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKOUTPAGE_H
