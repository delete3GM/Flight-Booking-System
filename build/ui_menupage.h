/********************************************************************************
** Form generated from reading UI file 'menupage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENUPAGE_H
#define UI_MENUPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuPage
{
public:
    QPushButton *exitBtn;
    QPushButton *userBtn;
    QPushButton *searchBtn;
    QLabel *titleLbl;

    void setupUi(QWidget *MenuPage)
    {
        if (MenuPage->objectName().isEmpty())
            MenuPage->setObjectName("MenuPage");
        MenuPage->resize(960, 540);
        MenuPage->setMinimumSize(QSize(960, 540));
        exitBtn = new QPushButton(MenuPage);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(410, 370, 151, 41));
        QFont font;
        font.setPointSize(16);
        exitBtn->setFont(font);
        userBtn = new QPushButton(MenuPage);
        userBtn->setObjectName("userBtn");
        userBtn->setGeometry(QRect(410, 280, 151, 41));
        userBtn->setFont(font);
        searchBtn = new QPushButton(MenuPage);
        searchBtn->setObjectName("searchBtn");
        searchBtn->setGeometry(QRect(410, 190, 151, 41));
        searchBtn->setFont(font);
        titleLbl = new QLabel(MenuPage);
        titleLbl->setObjectName("titleLbl");
        titleLbl->setGeometry(QRect(400, 70, 361, 51));
        QFont font1;
        font1.setPointSize(25);
        titleLbl->setFont(font1);

        retranslateUi(MenuPage);

        QMetaObject::connectSlotsByName(MenuPage);
    } // setupUi

    void retranslateUi(QWidget *MenuPage)
    {
        MenuPage->setWindowTitle(QCoreApplication::translate("MenuPage", "Form", nullptr));
        exitBtn->setText(QCoreApplication::translate("MenuPage", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
        userBtn->setText(QCoreApplication::translate("MenuPage", "\344\270\252\344\272\272\344\270\255\345\277\203", nullptr));
        searchBtn->setText(QCoreApplication::translate("MenuPage", "\350\256\242\347\245\250\344\270\255\345\277\203", nullptr));
        titleLbl->setText(QCoreApplication::translate("MenuPage", "\346\234\272\347\245\250\351\242\204\350\256\242\347\263\273\347\273\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MenuPage: public Ui_MenuPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENUPAGE_H
