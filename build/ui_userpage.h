/********************************************************************************
** Form generated from reading UI file 'userpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERPAGE_H
#define UI_USERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserPage
{
public:
    QTabWidget *Orders;
    QWidget *avaiOrder;
    QWidget *allOrder;
    QPushButton *graphBtn;
    QPushButton *user2menuBtn;
    QLabel *myOrderLbl;

    void setupUi(QWidget *UserPage)
    {
        if (UserPage->objectName().isEmpty())
            UserPage->setObjectName("UserPage");
        UserPage->resize(960, 540);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UserPage->sizePolicy().hasHeightForWidth());
        UserPage->setSizePolicy(sizePolicy);
        UserPage->setMinimumSize(QSize(960, 540));
        Orders = new QTabWidget(UserPage);
        Orders->setObjectName("Orders");
        Orders->setGeometry(QRect(30, 90, 891, 411));
        Orders->setMinimumSize(QSize(891, 401));
        avaiOrder = new QWidget();
        avaiOrder->setObjectName("avaiOrder");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(avaiOrder->sizePolicy().hasHeightForWidth());
        avaiOrder->setSizePolicy(sizePolicy1);
        avaiOrder->setMinimumSize(QSize(891, 401));
        Orders->addTab(avaiOrder, QString());
        allOrder = new QWidget();
        allOrder->setObjectName("allOrder");
        allOrder->setEnabled(true);
        sizePolicy1.setHeightForWidth(allOrder->sizePolicy().hasHeightForWidth());
        allOrder->setSizePolicy(sizePolicy1);
        allOrder->setMinimumSize(QSize(891, 401));
        Orders->addTab(allOrder, QString());
        graphBtn = new QPushButton(UserPage);
        graphBtn->setObjectName("graphBtn");
        graphBtn->setGeometry(QRect(760, 40, 91, 31));
        QFont font;
        font.setPointSize(13);
        graphBtn->setFont(font);
        user2menuBtn = new QPushButton(UserPage);
        user2menuBtn->setObjectName("user2menuBtn");
        user2menuBtn->setGeometry(QRect(30, 20, 61, 31));
        myOrderLbl = new QLabel(UserPage);
        myOrderLbl->setObjectName("myOrderLbl");
        myOrderLbl->setGeometry(QRect(430, 10, 121, 51));
        QFont font1;
        font1.setPointSize(20);
        myOrderLbl->setFont(font1);

        retranslateUi(UserPage);

        Orders->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UserPage);
    } // setupUi

    void retranslateUi(QWidget *UserPage)
    {
        UserPage->setWindowTitle(QCoreApplication::translate("UserPage", "Form", nullptr));
        Orders->setTabText(Orders->indexOf(avaiOrder), QCoreApplication::translate("UserPage", "\345\267\262\345\207\272\347\245\250", nullptr));
        Orders->setTabText(Orders->indexOf(allOrder), QCoreApplication::translate("UserPage", "\345\205\250\351\203\250", nullptr));
        graphBtn->setText(QCoreApplication::translate("UserPage", "\346\210\221\347\232\204\350\210\252\347\272\277\345\233\276", nullptr));
        user2menuBtn->setText(QCoreApplication::translate("UserPage", "\350\277\224\345\233\236", nullptr));
        myOrderLbl->setText(QCoreApplication::translate("UserPage", "\346\210\221\347\232\204\350\256\242\345\215\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserPage: public Ui_UserPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERPAGE_H
