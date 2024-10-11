/********************************************************************************
** Form generated from reading UI file 'searchpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHPAGE_H
#define UI_SEARCHPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchPage
{
public:
    QPushButton *backBtn;
    QPushButton *showCalendarBtn;
    QComboBox *arrBox;
    QPushButton *searchTransBtn;
    QComboBox *userhintcomb;
    QPushButton *exchangeBtn;
    QPushButton *searchBtn;
    QCalendarWidget *calendarWidget;
    QComboBox *depBox;

    void setupUi(QWidget *SearchPage)
    {
        if (SearchPage->objectName().isEmpty())
            SearchPage->setObjectName("SearchPage");
        SearchPage->resize(960, 540);
        SearchPage->setMinimumSize(QSize(960, 540));
        backBtn = new QPushButton(SearchPage);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(70, 30, 61, 31));
        showCalendarBtn = new QPushButton(SearchPage);
        showCalendarBtn->setObjectName("showCalendarBtn");
        showCalendarBtn->setGeometry(QRect(230, 280, 261, 61));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(14);
        showCalendarBtn->setFont(font);
        arrBox = new QComboBox(SearchPage);
        arrBox->setObjectName("arrBox");
        arrBox->setGeometry(QRect(610, 110, 131, 51));
        QFont font1;
        font1.setPointSize(15);
        arrBox->setFont(font1);
        searchTransBtn = new QPushButton(SearchPage);
        searchTransBtn->setObjectName("searchTransBtn");
        searchTransBtn->setGeometry(QRect(740, 280, 161, 61));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font2.setPointSize(19);
        font2.setBold(false);
        font2.setItalic(false);
        searchTransBtn->setFont(font2);
        userhintcomb = new QComboBox(SearchPage);
        userhintcomb->setObjectName("userhintcomb");
        userhintcomb->setGeometry(QRect(450, 120, 141, 31));
        exchangeBtn = new QPushButton(SearchPage);
        exchangeBtn->setObjectName("exchangeBtn");
        exchangeBtn->setGeometry(QRect(390, 120, 56, 31));
        searchBtn = new QPushButton(SearchPage);
        searchBtn->setObjectName("searchBtn");
        searchBtn->setGeometry(QRect(580, 280, 161, 61));
        searchBtn->setFont(font2);
        calendarWidget = new QCalendarWidget(SearchPage);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(240, 340, 251, 161));
        depBox = new QComboBox(SearchPage);
        depBox->setObjectName("depBox");
        depBox->setGeometry(QRect(230, 110, 131, 51));
        QFont font3;
        font3.setPointSize(14);
        depBox->setFont(font3);
        depBox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        depBox->setAutoFillBackground(false);
        depBox->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(SearchPage);

        QMetaObject::connectSlotsByName(SearchPage);
    } // setupUi

    void retranslateUi(QWidget *SearchPage)
    {
        SearchPage->setWindowTitle(QCoreApplication::translate("SearchPage", "Form", nullptr));
        backBtn->setText(QCoreApplication::translate("SearchPage", "\350\277\224\345\233\236", nullptr));
        showCalendarBtn->setText(QCoreApplication::translate("SearchPage", "\351\200\211\346\213\251\345\207\272\345\217\221\346\227\245\346\234\237", nullptr));
        searchTransBtn->setText(QCoreApplication::translate("SearchPage", "\350\275\254\346\234\272\346\237\245\350\257\242", nullptr));
        exchangeBtn->setText(QCoreApplication::translate("SearchPage", "\344\272\244\346\215\242", nullptr));
        searchBtn->setText(QCoreApplication::translate("SearchPage", "\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchPage: public Ui_SearchPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHPAGE_H
