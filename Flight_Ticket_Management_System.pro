QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# 外部库
INCLUDEPATH += $$PWD/lib/

HEADERS += \
    $$PWD/lib/QRCodeGenerator/qrcodegen.h

SOURCES += \
    $$PWD/lib/QRCodeGenerator/qrcodegen.cpp


# 添加源文件路径
SOURCES += \
    src/FlightRoute.cpp \
    src/FlightRecommendation.cpp \
    src/UserPreferences.cpp \
    src/Flight.cpp \
    src/FlightNetwork.cpp \
    src/Order.cpp \
    src/OrderManager.cpp \
    src/Passenger.cpp \
    src/Utils.cpp \
    src/main.cpp \
    src/flight_ticket_management_system.cpp \
    src/pages/LoginPage.cpp \
    src/pages/MenuPage.cpp \
    src/pages/SearchPage.cpp \
    src/pages/MapPage.cpp \
    src/pages/UserPage.cpp \
    src/pages/CheckoutPage.cpp \
    src/pages/InfoPage.cpp

# 添加头文件路径
INCLUDEPATH += $$PWD/include

HEADERS += \
    include/FlightRoute.h \
    include/FlightRecommendation.h \
    include/UserPreferences.h \
    include/City.h \
    include/Flight.h \
    include/FlightNetwork.h \
    include/FlightNode.h \
    include/Order.h \
    include/OrderManager.h \
    include/Passenger.h \
    include/Utils.h \
    include/flight_ticket_management_system.h \
    include/pages/LoginPage.h \
    include/pages/MenuPage.h \
    include/pages/SearchPage.h \
    include/pages/MapPage.h \
    include/pages/UserPage.h \
    include/pages/CheckoutPage.h \
    include/pages/InfoPage.h

FORMS += \
    ui/mappage.ui \
    ui/checkoutpage.ui \
    ui/infopage.ui \
    ui/searchpage.ui \
    ui/flight_ticket_management_system.ui \
    ui/loginpage.ui \
    ui/menupage.ui \
    ui/userpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/flight/flight_data.txt \
    data/order/.txt \
    data/order/1.txt \
    data/order/1101.txt \
    data/order/3.txt \
    data/order/gh.txt \
    scripts/word_cloud.py \
    web/css/leaflet.css \
    web/html/domestic_routes.html \
    web/html/global_routes.html \
    web/html/map.html \
    web/js/leaflet.js \
    web/js/leaflet_geodesic.js \
    web/js/leaftlet_geodesic.js \
    web/js/qwebchannel.js

RESOURCES += \
    resources.qrc



