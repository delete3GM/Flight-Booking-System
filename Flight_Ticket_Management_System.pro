QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# 添加源文件路径
SOURCES += \
    src/Common.cpp \
    src/Flight.cpp \
    src/FlightNetwork.cpp \
    src/Order.cpp \
    src/OrderManager.cpp \
    src/User.cpp \
    src/main.cpp \
    src/flight_ticket_management_system.cpp

# 添加头文件路径
INCLUDEPATH += $$PWD/include

HEADERS += \
    include/City.h \
    include/Common.h \
    include/Flight.h \
    include/FlightNetwork.h \
    include/FlightNode.h \
    include/Order.h \
    include/OrderManager.h \
    include/User.h \
    include/flight_ticket_management_system.h

FORMS += \
    flight_ticket_management_system.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
