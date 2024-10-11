/****************************************************************************
** Meta object code from reading C++ file 'InfoPage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/pages/InfoPage.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InfoPage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSInfoPageENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSInfoPageENDCLASS = QtMocHelpers::stringData(
    "InfoPage",
    "updateSearch",
    "",
    "buttonId",
    "handleTicketBooking",
    "rowIndex",
    "info2search"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSInfoPageENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[9];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[20];
    char stringdata5[9];
    char stringdata6[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSInfoPageENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSInfoPageENDCLASS_t qt_meta_stringdata_CLASSInfoPageENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "InfoPage"
        QT_MOC_LITERAL(9, 12),  // "updateSearch"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 8),  // "buttonId"
        QT_MOC_LITERAL(32, 19),  // "handleTicketBooking"
        QT_MOC_LITERAL(52, 8),  // "rowIndex"
        QT_MOC_LITERAL(61, 11)   // "info2search"
    },
    "InfoPage",
    "updateSearch",
    "",
    "buttonId",
    "handleTicketBooking",
    "rowIndex",
    "info2search"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSInfoPageENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x08,    1 /* Private */,
       4,    1,   35,    2, 0x08,    3 /* Private */,
       6,    0,   38,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject InfoPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSInfoPageENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSInfoPageENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSInfoPageENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<InfoPage, std::true_type>,
        // method 'updateSearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleTicketBooking'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'info2search'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void InfoPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InfoPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateSearch((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->handleTicketBooking((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->info2search(); break;
        default: ;
        }
    }
}

const QMetaObject *InfoPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InfoPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSInfoPageENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int InfoPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
