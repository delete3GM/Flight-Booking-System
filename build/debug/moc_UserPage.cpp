/****************************************************************************
** Meta object code from reading C++ file 'UserPage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/pages/UserPage.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UserPage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSUserPageENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSUserPageENDCLASS = QtMocHelpers::stringData(
    "UserPage",
    "displayOrders",
    "",
    "QTabWidget*",
    "tabWidget",
    "statusFilter",
    "onTabChanged",
    "index",
    "initOrderTab",
    "user2menu",
    "user2map"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSUserPageENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[9];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[10];
    char stringdata5[13];
    char stringdata6[13];
    char stringdata7[6];
    char stringdata8[13];
    char stringdata9[10];
    char stringdata10[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSUserPageENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSUserPageENDCLASS_t qt_meta_stringdata_CLASSUserPageENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "UserPage"
        QT_MOC_LITERAL(9, 13),  // "displayOrders"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 11),  // "QTabWidget*"
        QT_MOC_LITERAL(36, 9),  // "tabWidget"
        QT_MOC_LITERAL(46, 12),  // "statusFilter"
        QT_MOC_LITERAL(59, 12),  // "onTabChanged"
        QT_MOC_LITERAL(72, 5),  // "index"
        QT_MOC_LITERAL(78, 12),  // "initOrderTab"
        QT_MOC_LITERAL(91, 9),  // "user2menu"
        QT_MOC_LITERAL(101, 8)   // "user2map"
    },
    "UserPage",
    "displayOrders",
    "",
    "QTabWidget*",
    "tabWidget",
    "statusFilter",
    "onTabChanged",
    "index",
    "initOrderTab",
    "user2menu",
    "user2map"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSUserPageENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x08,    1 /* Private */,
       6,    1,   49,    2, 0x08,    4 /* Private */,
       8,    0,   52,    2, 0x08,    6 /* Private */,
       9,    0,   53,    2, 0x08,    7 /* Private */,
      10,    0,   54,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject UserPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSUserPageENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSUserPageENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSUserPageENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UserPage, std::true_type>,
        // method 'displayOrders'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTabWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'initOrderTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'user2menu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'user2map'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void UserPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->displayOrders((*reinterpret_cast< std::add_pointer_t<QTabWidget*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->initOrderTab(); break;
        case 3: _t->user2menu(); break;
        case 4: _t->user2map(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTabWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *UserPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSUserPageENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UserPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
