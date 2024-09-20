QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common.cpp \
    Flight.cpp \
    FlightNetwork.cpp \
    Order.cpp \
    OrderManager.cpp \
    User.cpp \
    main.cpp \
    flight_ticket_management_system.cpp

HEADERS += \
    City.h \
    Common.h \
    Flight.h \
    FlightNetwork.h \
    FlightNode.h \
    Order.h \
    OrderManager.h \
    User.h \
    flight_ticket_management_system.h

FORMS += \
    flight_ticket_management_system.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
