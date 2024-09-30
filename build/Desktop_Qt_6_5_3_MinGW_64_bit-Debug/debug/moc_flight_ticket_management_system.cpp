/****************************************************************************
** Meta object code from reading C++ file 'flight_ticket_management_system.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/flight_ticket_management_system.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flight_ticket_management_system.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS = QtMocHelpers::stringData(
    "Flight_Ticket_Management_System",
    "Login",
    "",
    "Menu2Info",
    "Back2Menu",
    "Start2Menu",
    "Start2User",
    "toStart",
    "toInfo",
    "Exit",
    "initializeDepBox",
    "initializeArrBox",
    "getDep",
    "index",
    "getArr",
    "Exchange",
    "searchFlights",
    "updateSearch",
    "buttonId",
    "searchFlightsWithTransfers",
    "updateTableWidget",
    "QList<Flight>",
    "flights",
    "showCalendar",
    "updateButtonWithDate",
    "date",
    "handleTicketBooking",
    "rowIndex",
    "addPassenger",
    "displayOrders",
    "QTabWidget*",
    "tabWidget",
    "statusFilter",
    "onTabChanged",
    "handleRefund",
    "orderId",
    "handleReschedule",
    "showMap"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS_t {
    uint offsetsAndSizes[76];
    char stringdata0[32];
    char stringdata1[6];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[10];
    char stringdata5[11];
    char stringdata6[11];
    char stringdata7[8];
    char stringdata8[7];
    char stringdata9[5];
    char stringdata10[17];
    char stringdata11[17];
    char stringdata12[7];
    char stringdata13[6];
    char stringdata14[7];
    char stringdata15[9];
    char stringdata16[14];
    char stringdata17[13];
    char stringdata18[9];
    char stringdata19[27];
    char stringdata20[18];
    char stringdata21[14];
    char stringdata22[8];
    char stringdata23[13];
    char stringdata24[21];
    char stringdata25[5];
    char stringdata26[20];
    char stringdata27[9];
    char stringdata28[13];
    char stringdata29[14];
    char stringdata30[12];
    char stringdata31[10];
    char stringdata32[13];
    char stringdata33[13];
    char stringdata34[13];
    char stringdata35[8];
    char stringdata36[17];
    char stringdata37[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS_t qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS = {
    {
        QT_MOC_LITERAL(0, 31),  // "Flight_Ticket_Management_System"
        QT_MOC_LITERAL(32, 5),  // "Login"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 9),  // "Menu2Info"
        QT_MOC_LITERAL(49, 9),  // "Back2Menu"
        QT_MOC_LITERAL(59, 10),  // "Start2Menu"
        QT_MOC_LITERAL(70, 10),  // "Start2User"
        QT_MOC_LITERAL(81, 7),  // "toStart"
        QT_MOC_LITERAL(89, 6),  // "toInfo"
        QT_MOC_LITERAL(96, 4),  // "Exit"
        QT_MOC_LITERAL(101, 16),  // "initializeDepBox"
        QT_MOC_LITERAL(118, 16),  // "initializeArrBox"
        QT_MOC_LITERAL(135, 6),  // "getDep"
        QT_MOC_LITERAL(142, 5),  // "index"
        QT_MOC_LITERAL(148, 6),  // "getArr"
        QT_MOC_LITERAL(155, 8),  // "Exchange"
        QT_MOC_LITERAL(164, 13),  // "searchFlights"
        QT_MOC_LITERAL(178, 12),  // "updateSearch"
        QT_MOC_LITERAL(191, 8),  // "buttonId"
        QT_MOC_LITERAL(200, 26),  // "searchFlightsWithTransfers"
        QT_MOC_LITERAL(227, 17),  // "updateTableWidget"
        QT_MOC_LITERAL(245, 13),  // "QList<Flight>"
        QT_MOC_LITERAL(259, 7),  // "flights"
        QT_MOC_LITERAL(267, 12),  // "showCalendar"
        QT_MOC_LITERAL(280, 20),  // "updateButtonWithDate"
        QT_MOC_LITERAL(301, 4),  // "date"
        QT_MOC_LITERAL(306, 19),  // "handleTicketBooking"
        QT_MOC_LITERAL(326, 8),  // "rowIndex"
        QT_MOC_LITERAL(335, 12),  // "addPassenger"
        QT_MOC_LITERAL(348, 13),  // "displayOrders"
        QT_MOC_LITERAL(362, 11),  // "QTabWidget*"
        QT_MOC_LITERAL(374, 9),  // "tabWidget"
        QT_MOC_LITERAL(384, 12),  // "statusFilter"
        QT_MOC_LITERAL(397, 12),  // "onTabChanged"
        QT_MOC_LITERAL(410, 12),  // "handleRefund"
        QT_MOC_LITERAL(423, 7),  // "orderId"
        QT_MOC_LITERAL(431, 16),  // "handleReschedule"
        QT_MOC_LITERAL(448, 7)   // "showMap"
    },
    "Flight_Ticket_Management_System",
    "Login",
    "",
    "Menu2Info",
    "Back2Menu",
    "Start2Menu",
    "Start2User",
    "toStart",
    "toInfo",
    "Exit",
    "initializeDepBox",
    "initializeArrBox",
    "getDep",
    "index",
    "getArr",
    "Exchange",
    "searchFlights",
    "updateSearch",
    "buttonId",
    "searchFlightsWithTransfers",
    "updateTableWidget",
    "QList<Flight>",
    "flights",
    "showCalendar",
    "updateButtonWithDate",
    "date",
    "handleTicketBooking",
    "rowIndex",
    "addPassenger",
    "displayOrders",
    "QTabWidget*",
    "tabWidget",
    "statusFilter",
    "onTabChanged",
    "handleRefund",
    "orderId",
    "handleReschedule",
    "showMap"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSFlight_Ticket_Management_SystemENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  176,    2, 0x08,    1 /* Private */,
       3,    0,  177,    2, 0x08,    2 /* Private */,
       4,    0,  178,    2, 0x08,    3 /* Private */,
       5,    0,  179,    2, 0x08,    4 /* Private */,
       6,    0,  180,    2, 0x08,    5 /* Private */,
       7,    0,  181,    2, 0x08,    6 /* Private */,
       8,    0,  182,    2, 0x08,    7 /* Private */,
       9,    0,  183,    2, 0x08,    8 /* Private */,
      10,    0,  184,    2, 0x08,    9 /* Private */,
      11,    0,  185,    2, 0x08,   10 /* Private */,
      12,    1,  186,    2, 0x08,   11 /* Private */,
      14,    1,  189,    2, 0x08,   13 /* Private */,
      15,    0,  192,    2, 0x08,   15 /* Private */,
      16,    0,  193,    2, 0x08,   16 /* Private */,
      17,    1,  194,    2, 0x08,   17 /* Private */,
      19,    0,  197,    2, 0x08,   19 /* Private */,
      20,    1,  198,    2, 0x08,   20 /* Private */,
      23,    0,  201,    2, 0x08,   22 /* Private */,
      24,    1,  202,    2, 0x08,   23 /* Private */,
      26,    1,  205,    2, 0x08,   25 /* Private */,
      28,    0,  208,    2, 0x08,   27 /* Private */,
      29,    2,  209,    2, 0x08,   28 /* Private */,
      29,    1,  214,    2, 0x28,   31 /* Private | MethodCloned */,
      33,    1,  217,    2, 0x08,   33 /* Private */,
      34,    1,  220,    2, 0x08,   35 /* Private */,
      36,    1,  223,    2, 0x08,   37 /* Private */,
      37,    0,  226,    2, 0x08,   39 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   13,
    QMetaType::QString, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   25,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30, QMetaType::QString,   31,   32,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Flight_Ticket_Management_System::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSFlight_Ticket_Management_SystemENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Flight_Ticket_Management_System, std::true_type>,
        // method 'Login'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Menu2Info'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Back2Menu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Start2Menu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Start2User'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Exit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initializeDepBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initializeArrBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getDep'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getArr'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Exchange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchFlights'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'searchFlightsWithTransfers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateTableWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<Flight> &, std::false_type>,
        // method 'showCalendar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateButtonWithDate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'handleTicketBooking'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'addPassenger'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayOrders'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTabWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'displayOrders'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTabWidget *, std::false_type>,
        // method 'onTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleRefund'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleReschedule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showMap'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Flight_Ticket_Management_System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Flight_Ticket_Management_System *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Login(); break;
        case 1: _t->Menu2Info(); break;
        case 2: _t->Back2Menu(); break;
        case 3: _t->Start2Menu(); break;
        case 4: _t->Start2User(); break;
        case 5: _t->toStart(); break;
        case 6: _t->toInfo(); break;
        case 7: _t->Exit(); break;
        case 8: _t->initializeDepBox(); break;
        case 9: _t->initializeArrBox(); break;
        case 10: { QString _r = _t->getDep((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->getArr((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->Exchange(); break;
        case 13: _t->searchFlights(); break;
        case 14: _t->updateSearch((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->searchFlightsWithTransfers(); break;
        case 16: _t->updateTableWidget((*reinterpret_cast< std::add_pointer_t<QList<Flight>>>(_a[1]))); break;
        case 17: _t->showCalendar(); break;
        case 18: _t->updateButtonWithDate((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 19: _t->handleTicketBooking((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->addPassenger(); break;
        case 21: _t->displayOrders((*reinterpret_cast< std::add_pointer_t<QTabWidget*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 22: _t->displayOrders((*reinterpret_cast< std::add_pointer_t<QTabWidget*>>(_a[1]))); break;
        case 23: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->handleRefund((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->handleReschedule((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 26: _t->showMap(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTabWidget* >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTabWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *Flight_Ticket_Management_System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Flight_Ticket_Management_System::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSFlight_Ticket_Management_SystemENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Flight_Ticket_Management_System::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_WARNING_POP
