/****************************************************************************
** Meta object code from reading C++ file 'lpHeatMap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lpHeatMap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lpHeatMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lpHeatMap_t {
    QByteArrayData data[11];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lpHeatMap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lpHeatMap_t qt_meta_stringdata_lpHeatMap = {
    {
QT_MOC_LITERAL(0, 0, 9), // "lpHeatMap"
QT_MOC_LITERAL(1, 10, 12), // "sgThreadData"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 27), // "QSharedPointer<QJsonObject>"
QT_MOC_LITERAL(4, 52, 9), // "json_sptr"
QT_MOC_LITERAL(5, 62, 17), // "onRecvDoffHeatMap"
QT_MOC_LITERAL(6, 80, 19), // "onUpdateHeatMapData"
QT_MOC_LITERAL(7, 100, 7), // "channel"
QT_MOC_LITERAL(8, 108, 13), // "QList<QPoint>"
QT_MOC_LITERAL(9, 122, 6), // "points"
QT_MOC_LITERAL(10, 129, 7) // "onReset"

    },
    "lpHeatMap\0sgThreadData\0\0"
    "QSharedPointer<QJsonObject>\0json_sptr\0"
    "onRecvDoffHeatMap\0onUpdateHeatMapData\0"
    "channel\0QList<QPoint>\0points\0onReset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lpHeatMap[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   37,    2, 0x08 /* Private */,
       6,    2,   40,    2, 0x08 /* Private */,
      10,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    7,    9,
    QMetaType::Void,

       0        // eod
};

void lpHeatMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        lpHeatMap *_t = static_cast<lpHeatMap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sgThreadData((*reinterpret_cast< QSharedPointer<QJsonObject>(*)>(_a[1]))); break;
        case 1: _t->onRecvDoffHeatMap((*reinterpret_cast< QSharedPointer<QJsonObject>(*)>(_a[1]))); break;
        case 2: _t->onUpdateHeatMapData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QList<QPoint>(*)>(_a[2]))); break;
        case 3: _t->onReset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (lpHeatMap::*_t)(QSharedPointer<QJsonObject> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&lpHeatMap::sgThreadData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject lpHeatMap::staticMetaObject = {
    { &lpHeatMapMgr::staticMetaObject, qt_meta_stringdata_lpHeatMap.data,
      qt_meta_data_lpHeatMap,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *lpHeatMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lpHeatMap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lpHeatMap.stringdata0))
        return static_cast<void*>(this);
    return lpHeatMapMgr::qt_metacast(_clname);
}

int lpHeatMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = lpHeatMapMgr::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void lpHeatMap::sgThreadData(QSharedPointer<QJsonObject> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
