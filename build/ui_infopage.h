/********************************************************************************
** Form generated from reading UI file 'infopage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOPAGE_H
#define UI_INFOPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoPage
{
public:
    QTableWidget *flightTableWidget;
    QRadioButton *early_sort;
    QRadioButton *time_sort;
    QPushButton *backBtn;
    QLabel *infoTitleLbl;
    QRadioButton *price_sort;
    QRadioButton *default_sort;

    void setupUi(QWidget *InfoPage)
    {
        if (InfoPage->objectName().isEmpty())
            InfoPage->setObjectName("InfoPage");
        InfoPage->resize(960, 540);
        InfoPage->setMinimumSize(QSize(960, 540));
        flightTableWidget = new QTableWidget(InfoPage);
        flightTableWidget->setObjectName("flightTableWidget");
        flightTableWidget->setGeometry(QRect(80, 100, 800, 400));
        flightTableWidget->setMinimumSize(QSize(800, 400));
        flightTableWidget->setMaximumSize(QSize(800, 400));
        early_sort = new QRadioButton(InfoPage);
        early_sort->setObjectName("early_sort");
        early_sort->setGeometry(QRect(750, 70, 71, 16));
        time_sort = new QRadioButton(InfoPage);
        time_sort->setObjectName("time_sort");
        time_sort->setGeometry(QRect(680, 70, 71, 16));
        backBtn = new QPushButton(InfoPage);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(40, 30, 61, 31));
        infoTitleLbl = new QLabel(InfoPage);
        infoTitleLbl->setObjectName("infoTitleLbl");
        infoTitleLbl->setGeometry(QRect(431, 30, 151, 23));
        QFont font;
        font.setPointSize(18);
        infoTitleLbl->setFont(font);
        infoTitleLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        price_sort = new QRadioButton(InfoPage);
        price_sort->setObjectName("price_sort");
        price_sort->setGeometry(QRect(820, 70, 91, 16));
        default_sort = new QRadioButton(InfoPage);
        default_sort->setObjectName("default_sort");
        default_sort->setGeometry(QRect(610, 70, 71, 16));

        retranslateUi(InfoPage);

        QMetaObject::connectSlotsByName(InfoPage);
    } // setupUi

    void retranslateUi(QWidget *InfoPage)
    {
        InfoPage->setWindowTitle(QCoreApplication::translate("InfoPage", "Form", nullptr));
        early_sort->setText(QCoreApplication::translate("InfoPage", "\346\227\266\351\227\264\346\234\200\346\227\251", nullptr));
        time_sort->setText(QCoreApplication::translate("InfoPage", "\346\227\266\351\225\277\346\234\200\347\237\255", nullptr));
        backBtn->setText(QCoreApplication::translate("InfoPage", "\350\277\224\345\233\236", nullptr));
        infoTitleLbl->setText(QCoreApplication::translate("InfoPage", "\350\210\252\347\217\255\344\277\241\346\201\257", nullptr));
        price_sort->setText(QCoreApplication::translate("InfoPage", "\344\273\267\346\240\274\346\234\200\344\275\216", nullptr));
        default_sort->setText(QCoreApplication::translate("InfoPage", "\351\273\230\350\256\244\346\216\222\345\272\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InfoPage: public Ui_InfoPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOPAGE_H
