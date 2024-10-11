/****************************************************************************
** Meta object code from reading C++ file 'LoginPage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/pages/LoginPage.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LoginPage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSLoginPageENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLoginPageENDCLASS = QtMocHelpers::stringData(
    "LoginPage",
    "Login",
    "",
    "signUp",
    "changePsw",
    "showChangePasswordDialog",
    "updatePassword",
    "newPassword",
    "confirmPassword"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLoginPageENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[10];
    char stringdata1[6];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[10];
    char stringdata5[25];
    char stringdata6[15];
    char stringdata7[12];
    char stringdata8[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLoginPageENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLoginPageENDCLASS_t qt_meta_stringdata_CLASSLoginPageENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "LoginPage"
        QT_MOC_LITERAL(10, 5),  // "Login"
        QT_MOC_LITERAL(16, 0),  // ""
        QT_MOC_LITERAL(17, 6),  // "signUp"
        QT_MOC_LITERAL(24, 9),  // "changePsw"
        QT_MOC_LITERAL(34, 24),  // "showChangePasswordDialog"
        QT_MOC_LITERAL(59, 14),  // "updatePassword"
        QT_MOC_LITERAL(74, 11),  // "newPassword"
        QT_MOC_LITERAL(86, 15)   // "confirmPassword"
    },
    "LoginPage",
    "Login",
    "",
    "signUp",
    "changePsw",
    "showChangePasswordDialog",
    "updatePassword",
    "newPassword",
    "confirmPassword"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLoginPageENDCLASS[] = {

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
       1,    0,   44,    2, 0x08,    1 /* Private */,
       3,    0,   45,    2, 0x08,    2 /* Private */,
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    2,   48,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    7,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject LoginPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSLoginPageENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLoginPageENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLoginPageENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LoginPage, std::true_type>,
        // method 'Login'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changePsw'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showChangePasswordDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updatePassword'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void LoginPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LoginPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Login(); break;
        case 1: _t->signUp(); break;
        case 2: _t->changePsw(); break;
        case 3: _t->showChangePasswordDialog(); break;
        case 4: { bool _r = _t->updatePassword((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *LoginPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLoginPageENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LoginPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
