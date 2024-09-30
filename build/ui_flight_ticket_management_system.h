/********************************************************************************
** Form generated from reading UI file 'flight_ticket_management_system.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
#define UI_FLIGHT_TICKET_MANAGEMENT_SYSTEM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Flight_Ticket_Management_System
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QLabel *loginTitleLbl;
    QLineEdit *typeAcnt;
    QLineEdit *typePsw;
    QPushButton *loginBtn;
    QWidget *startPage;
    QPushButton *menuBtn;
    QPushButton *userBtn;
    QPushButton *exitBtn;
    QLabel *startTitleLbl;
    QWidget *userPage;
    QPushButton *user2startBtn;
    QLabel *myOrderLbl;
    QTabWidget *Orders;
    QWidget *avaiOrder;
    QWidget *allOrder;
    QPushButton *graphBtn;
    QWidget *menuPage;
    QPushButton *searchBtn;
    QComboBox *depBox;
    QComboBox *arrBox;
    QPushButton *showCalendarBtn;
    QCalendarWidget *calendarWidget;
    QPushButton *exchangeBtn;
    QPushButton *menu2startBtn;
    QPushButton *searchTransBtn;
    QComboBox *userhintcomb;
    QWidget *flightInfoPage;
    QLabel *infoTitleLbl;
    QTableWidget *flightTableWidget;
    QRadioButton *default_sort;
    QRadioButton *time_sort;
    QRadioButton *price_sort;
    QPushButton *backBtn;
    QRadioButton *early_sort;
    QWidget *buyPage;
    QLabel *passenger_title;
    QLabel *passenger_1;
    QLineEdit *familyname;
    QLineEdit *givenname;
    QRadioButton *isMale;
    QRadioButton *isFemale;
    QLineEdit *passenger_id;
    QLineEdit *passenger_phone;
    QPushButton *submitBtn;
    QLabel *flightInfo;
    QPushButton *cancelBtn;
    QWidget *mappage;
    QGridLayout *gridLayout_2;
    QComboBox *airlineComb;
    QSpacerItem *horizontalSpacer;
    QPushButton *backtoUser;

    void setupUi(QMainWindow *Flight_Ticket_Management_System)
    {
        if (Flight_Ticket_Management_System->objectName().isEmpty())
            Flight_Ticket_Management_System->setObjectName("Flight_Ticket_Management_System");
        Flight_Ticket_Management_System->resize(960, 540);
        Flight_Ticket_Management_System->setMinimumSize(QSize(960, 540));
        Flight_Ticket_Management_System->setMaximumSize(QSize(960, 540));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("QIcon::ThemeIcon::Phone")));
        Flight_Ticket_Management_System->setWindowIcon(icon);
        centralwidget = new QWidget(Flight_Ticket_Management_System);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setMinimumSize(QSize(960, 540));
        stackedWidget->setMaximumSize(QSize(960, 540));
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        loginTitleLbl = new QLabel(loginPage);
        loginTitleLbl->setObjectName("loginTitleLbl");
        loginTitleLbl->setGeometry(QRect(400, 30, 361, 51));
        QFont font;
        font.setPointSize(25);
        loginTitleLbl->setFont(font);
        typeAcnt = new QLineEdit(loginPage);
        typeAcnt->setObjectName("typeAcnt");
        typeAcnt->setGeometry(QRect(420, 170, 131, 31));
        typePsw = new QLineEdit(loginPage);
        typePsw->setObjectName("typePsw");
        typePsw->setGeometry(QRect(420, 240, 131, 31));
        loginBtn = new QPushButton(loginPage);
        loginBtn->setObjectName("loginBtn");
        loginBtn->setGeometry(QRect(430, 310, 91, 31));
        QFont font1;
        font1.setPointSize(14);
        loginBtn->setFont(font1);
        stackedWidget->addWidget(loginPage);
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        menuBtn = new QPushButton(startPage);
        menuBtn->setObjectName("menuBtn");
        menuBtn->setGeometry(QRect(410, 190, 151, 41));
        QFont font2;
        font2.setPointSize(16);
        menuBtn->setFont(font2);
        userBtn = new QPushButton(startPage);
        userBtn->setObjectName("userBtn");
        userBtn->setGeometry(QRect(410, 280, 151, 41));
        userBtn->setFont(font2);
        exitBtn = new QPushButton(startPage);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(410, 370, 151, 41));
        exitBtn->setFont(font2);
        startTitleLbl = new QLabel(startPage);
        startTitleLbl->setObjectName("startTitleLbl");
        startTitleLbl->setGeometry(QRect(400, 70, 361, 51));
        startTitleLbl->setFont(font);
        stackedWidget->addWidget(startPage);
        userPage = new QWidget();
        userPage->setObjectName("userPage");
        user2startBtn = new QPushButton(userPage);
        user2startBtn->setObjectName("user2startBtn");
        user2startBtn->setGeometry(QRect(40, 30, 61, 31));
        myOrderLbl = new QLabel(userPage);
        myOrderLbl->setObjectName("myOrderLbl");
        myOrderLbl->setGeometry(QRect(440, 20, 121, 51));
        QFont font3;
        font3.setPointSize(20);
        myOrderLbl->setFont(font3);
        Orders = new QTabWidget(userPage);
        Orders->setObjectName("Orders");
        Orders->setGeometry(QRect(30, 100, 891, 411));
        Orders->setMinimumSize(QSize(891, 401));
        avaiOrder = new QWidget();
        avaiOrder->setObjectName("avaiOrder");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(avaiOrder->sizePolicy().hasHeightForWidth());
        avaiOrder->setSizePolicy(sizePolicy);
        avaiOrder->setMinimumSize(QSize(891, 401));
        Orders->addTab(avaiOrder, QString());
        allOrder = new QWidget();
        allOrder->setObjectName("allOrder");
        allOrder->setEnabled(true);
        sizePolicy.setHeightForWidth(allOrder->sizePolicy().hasHeightForWidth());
        allOrder->setSizePolicy(sizePolicy);
        allOrder->setMinimumSize(QSize(891, 401));
        Orders->addTab(allOrder, QString());
        graphBtn = new QPushButton(userPage);
        graphBtn->setObjectName("graphBtn");
        graphBtn->setGeometry(QRect(770, 50, 91, 31));
        QFont font4;
        font4.setPointSize(13);
        graphBtn->setFont(font4);
        stackedWidget->addWidget(userPage);
        menuPage = new QWidget();
        menuPage->setObjectName("menuPage");
        searchBtn = new QPushButton(menuPage);
        searchBtn->setObjectName("searchBtn");
        searchBtn->setGeometry(QRect(550, 290, 161, 61));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font5.setPointSize(19);
        font5.setBold(false);
        font5.setItalic(false);
        searchBtn->setFont(font5);
        depBox = new QComboBox(menuPage);
        depBox->setObjectName("depBox");
        depBox->setGeometry(QRect(200, 120, 131, 51));
        depBox->setFont(font1);
        depBox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        depBox->setAutoFillBackground(false);
        depBox->setStyleSheet(QString::fromUtf8(""));
        arrBox = new QComboBox(menuPage);
        arrBox->setObjectName("arrBox");
        arrBox->setGeometry(QRect(580, 120, 131, 51));
        QFont font6;
        font6.setPointSize(15);
        arrBox->setFont(font6);
        showCalendarBtn = new QPushButton(menuPage);
        showCalendarBtn->setObjectName("showCalendarBtn");
        showCalendarBtn->setGeometry(QRect(200, 290, 261, 61));
        QFont font7;
        font7.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font7.setPointSize(14);
        showCalendarBtn->setFont(font7);
        calendarWidget = new QCalendarWidget(menuPage);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(210, 350, 251, 161));
        exchangeBtn = new QPushButton(menuPage);
        exchangeBtn->setObjectName("exchangeBtn");
        exchangeBtn->setGeometry(QRect(360, 130, 56, 31));
        menu2startBtn = new QPushButton(menuPage);
        menu2startBtn->setObjectName("menu2startBtn");
        menu2startBtn->setGeometry(QRect(40, 40, 61, 31));
        searchTransBtn = new QPushButton(menuPage);
        searchTransBtn->setObjectName("searchTransBtn");
        searchTransBtn->setGeometry(QRect(710, 290, 161, 61));
        searchTransBtn->setFont(font5);
        userhintcomb = new QComboBox(menuPage);
        userhintcomb->setObjectName("userhintcomb");
        userhintcomb->setGeometry(QRect(420, 130, 141, 31));
        stackedWidget->addWidget(menuPage);
        flightInfoPage = new QWidget();
        flightInfoPage->setObjectName("flightInfoPage");
        infoTitleLbl = new QLabel(flightInfoPage);
        infoTitleLbl->setObjectName("infoTitleLbl");
        infoTitleLbl->setGeometry(QRect(421, 30, 151, 23));
        QFont font8;
        font8.setPointSize(18);
        infoTitleLbl->setFont(font8);
        infoTitleLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        flightTableWidget = new QTableWidget(flightInfoPage);
        flightTableWidget->setObjectName("flightTableWidget");
        flightTableWidget->setGeometry(QRect(70, 100, 800, 400));
        flightTableWidget->setMinimumSize(QSize(800, 400));
        flightTableWidget->setMaximumSize(QSize(800, 400));
        default_sort = new QRadioButton(flightInfoPage);
        default_sort->setObjectName("default_sort");
        default_sort->setGeometry(QRect(600, 70, 81, 16));
        time_sort = new QRadioButton(flightInfoPage);
        time_sort->setObjectName("time_sort");
        time_sort->setGeometry(QRect(670, 70, 71, 16));
        price_sort = new QRadioButton(flightInfoPage);
        price_sort->setObjectName("price_sort");
        price_sort->setGeometry(QRect(810, 70, 91, 16));
        backBtn = new QPushButton(flightInfoPage);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(30, 30, 61, 31));
        early_sort = new QRadioButton(flightInfoPage);
        early_sort->setObjectName("early_sort");
        early_sort->setGeometry(QRect(740, 70, 71, 16));
        stackedWidget->addWidget(flightInfoPage);
        buyPage = new QWidget();
        buyPage->setObjectName("buyPage");
        passenger_title = new QLabel(buyPage);
        passenger_title->setObjectName("passenger_title");
        passenger_title->setGeometry(QRect(390, 50, 81, 41));
        passenger_title->setFont(font6);
        passenger_1 = new QLabel(buyPage);
        passenger_1->setObjectName("passenger_1");
        passenger_1->setGeometry(QRect(400, 100, 61, 21));
        passenger_1->setFont(font1);
        familyname = new QLineEdit(buyPage);
        familyname->setObjectName("familyname");
        familyname->setGeometry(QRect(400, 150, 171, 41));
        familyname->setFont(font1);
        givenname = new QLineEdit(buyPage);
        givenname->setObjectName("givenname");
        givenname->setGeometry(QRect(700, 150, 161, 41));
        givenname->setFont(font1);
        isMale = new QRadioButton(buyPage);
        isMale->setObjectName("isMale");
        isMale->setGeometry(QRect(430, 210, 51, 21));
        isMale->setFont(font4);
        isMale->setStyleSheet(QString::fromUtf8(""));
        isMale->setCheckable(true);
        isMale->setChecked(false);
        isMale->setAutoRepeat(false);
        isFemale = new QRadioButton(buyPage);
        isFemale->setObjectName("isFemale");
        isFemale->setGeometry(QRect(510, 210, 72, 21));
        isFemale->setFont(font4);
        passenger_id = new QLineEdit(buyPage);
        passenger_id->setObjectName("passenger_id");
        passenger_id->setGeometry(QRect(400, 250, 481, 41));
        passenger_id->setFont(font1);
        passenger_phone = new QLineEdit(buyPage);
        passenger_phone->setObjectName("passenger_phone");
        passenger_phone->setGeometry(QRect(400, 350, 481, 41));
        passenger_phone->setFont(font1);
        submitBtn = new QPushButton(buyPage);
        submitBtn->setObjectName("submitBtn");
        submitBtn->setGeometry(QRect(400, 440, 111, 41));
        submitBtn->setFont(font6);
        flightInfo = new QLabel(buyPage);
        flightInfo->setObjectName("flightInfo");
        flightInfo->setGeometry(QRect(50, 60, 251, 391));
        flightInfo->setFont(font2);
        flightInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        cancelBtn = new QPushButton(buyPage);
        cancelBtn->setObjectName("cancelBtn");
        cancelBtn->setGeometry(QRect(770, 440, 111, 41));
        cancelBtn->setFont(font6);
        stackedWidget->addWidget(buyPage);
        mappage = new QWidget();
        mappage->setObjectName("mappage");
        gridLayout_2 = new QGridLayout(mappage);
        gridLayout_2->setObjectName("gridLayout_2");
        airlineComb = new QComboBox(mappage);
        airlineComb->setObjectName("airlineComb");

        gridLayout_2->addWidget(airlineComb, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(775, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        backtoUser = new QPushButton(mappage);
        backtoUser->setObjectName("backtoUser");

        gridLayout_2->addWidget(backtoUser, 0, 1, 1, 1);

        stackedWidget->addWidget(mappage);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        Flight_Ticket_Management_System->setCentralWidget(centralwidget);

        retranslateUi(Flight_Ticket_Management_System);

        stackedWidget->setCurrentIndex(3);
        Orders->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Flight_Ticket_Management_System);
    } // setupUi

    void retranslateUi(QMainWindow *Flight_Ticket_Management_System)
    {
        Flight_Ticket_Management_System->setWindowTitle(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\234\272\347\245\250\351\242\204\345\256\232\347\263\273\347\273\237", nullptr));
        loginTitleLbl->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\234\272\347\245\250\351\242\204\350\256\242\347\263\273\347\273\237", nullptr));
        loginBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\347\231\273\345\275\225", nullptr));
        menuBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\256\242\347\245\250\344\270\255\345\277\203", nullptr));
        userBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\344\270\252\344\272\272\344\270\255\345\277\203", nullptr));
        exitBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
        startTitleLbl->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\234\272\347\245\250\351\242\204\350\256\242\347\263\273\347\273\237", nullptr));
        user2startBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\277\224\345\233\236", nullptr));
        myOrderLbl->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\210\221\347\232\204\350\256\242\345\215\225", nullptr));
        Orders->setTabText(Orders->indexOf(avaiOrder), QCoreApplication::translate("Flight_Ticket_Management_System", "\345\267\262\345\207\272\347\245\250", nullptr));
        Orders->setTabText(Orders->indexOf(allOrder), QCoreApplication::translate("Flight_Ticket_Management_System", "\345\205\250\351\203\250", nullptr));
        graphBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\210\221\347\232\204\350\210\252\347\272\277\345\233\276", nullptr));
        searchBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\237\245\350\257\242", nullptr));
        showCalendarBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\351\200\211\346\213\251\345\207\272\345\217\221\346\227\245\346\234\237", nullptr));
        exchangeBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\344\272\244\346\215\242", nullptr));
        menu2startBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\277\224\345\233\236", nullptr));
        searchTransBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\275\254\346\234\272\346\237\245\350\257\242", nullptr));
        infoTitleLbl->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\210\252\347\217\255\344\277\241\346\201\257", nullptr));
        default_sort->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\351\273\230\350\256\244\346\216\222\345\272\217", nullptr));
        time_sort->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\227\266\351\225\277\346\234\200\347\237\255", nullptr));
        price_sort->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\344\273\267\346\240\274\346\234\200\344\275\216", nullptr));
        backBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\350\277\224\345\233\236", nullptr));
        early_sort->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\227\266\351\227\264\346\234\200\346\227\251", nullptr));
        passenger_title->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\344\271\230\345\256\242\344\277\241\346\201\257", nullptr));
        passenger_1->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\344\271\230\345\256\2421", nullptr));
        familyname->setInputMask(QString());
        familyname->setText(QString());
        givenname->setText(QString());
        isMale->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\347\224\267", nullptr));
        isFemale->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\345\245\263", nullptr));
        submitBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\346\217\220\344\272\244", nullptr));
        flightInfo->setText(QString());
        cancelBtn->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "\345\217\226\346\266\210", nullptr));
        backtoUser->setText(QCoreApplication::translate("Flight_Ticket_Management_System", "back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Flight_Ticket_Management_System: public Ui_Flight_Ticket_Management_System {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLIGHT_TICKET_MANAGEMENT_SYSTEM_H
