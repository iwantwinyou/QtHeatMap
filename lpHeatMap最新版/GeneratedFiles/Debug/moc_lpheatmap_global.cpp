/****************************************************************************
** Meta object code from reading C++ file 'lpheatmap_global.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lpheatmap_global.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lpheatmap_global.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lpHeatMapMgr_t {
    QByteArrayData data[13];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lpHeatMapMgr_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lpHeatMapMgr_t qt_meta_stringdata_lpHeatMapMgr = {
    {
QT_MOC_LITERAL(0, 0, 12), // "lpHeatMapMgr"
QT_MOC_LITERAL(1, 13, 10), // "sgAddUiLog"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "info"
QT_MOC_LITERAL(4, 30, 4), // "type"
QT_MOC_LITERAL(5, 35, 6), // "vparam"
QT_MOC_LITERAL(6, 42, 7), // "onReset"
QT_MOC_LITERAL(7, 50, 17), // "onRecvDoffHeatMap"
QT_MOC_LITERAL(8, 68, 27), // "QSharedPointer<QJsonObject>"
QT_MOC_LITERAL(9, 96, 9), // "json_sptr"
QT_MOC_LITERAL(10, 106, 19), // "onUpdateHeatMapData"
QT_MOC_LITERAL(11, 126, 24), // "QMap<int,QList<QPoint> >"
QT_MOC_LITERAL(12, 151, 14) // "channelDataMap"

    },
    "lpHeatMapMgr\0sgAddUiLog\0\0info\0type\0"
    "vparam\0onReset\0onRecvDoffHeatMap\0"
    "QSharedPointer<QJsonObject>\0json_sptr\0"
    "onUpdateHeatMapData\0QMap<int,QList<QPoint> >\0"
    "channelDataMap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lpHeatMapMgr[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       1,    2,   46,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   51,    2, 0x0a /* Public */,
       7,    1,   52,    2, 0x0a /* Public */,
      10,    1,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QVariant,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void lpHeatMapMgr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        lpHeatMapMgr *_t = static_cast<lpHeatMapMgr *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sgAddUiLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 1: _t->sgAddUiLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onReset(); break;
        case 3: _t->onRecvDoffHeatMap((*reinterpret_cast< QSharedPointer<QJsonObject>(*)>(_a[1]))); break;
        case 4: _t->onUpdateHeatMapData((*reinterpret_cast< const QMap<int,QList<QPoint> >(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (lpHeatMapMgr::*_t)(QString , int , QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lpHeatMapMgr::sgAddUiLog)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject lpHeatMapMgr::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_lpHeatMapMgr.data,
      qt_meta_data_lpHeatMapMgr,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *lpHeatMapMgr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lpHeatMapMgr::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lpHeatMapMgr.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int lpHeatMapMgr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void lpHeatMapMgr::sgAddUiLog(QString _t1, int _t2, QVariant _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
