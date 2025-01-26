/****************************************************************************
** Meta object code from reading C++ file 'audiowidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../audiowidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
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
struct qt_meta_stringdata_CLASSAudioWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSAudioWidgetENDCLASS = QtMocHelpers::stringData(
    "AudioWidget",
    "playAudio",
    "",
    "updatePosition",
    "newOcclusion",
    "modeChanged",
    "fileChanged",
    "file",
    "openFileDialog",
    "updateRoom",
    "animateChanged",
    "checked",
    "handlePositionChanged",
    "id",
    "pos",
    "handleFileDropped",
    "filePath",
    "position",
    "updateKnobs",
    "onSoundSourceSelected",
    "index"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAudioWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    0,   88,    2, 0x08,    3 /* Private */,
       5,    0,   89,    2, 0x08,    4 /* Private */,
       6,    1,   90,    2, 0x08,    5 /* Private */,
       8,    0,   93,    2, 0x08,    7 /* Private */,
       9,    0,   94,    2, 0x08,    8 /* Private */,
      10,    1,   95,    2, 0x08,    9 /* Private */,
      12,    2,   98,    2, 0x08,   11 /* Private */,
      15,    2,  103,    2, 0x08,   14 /* Private */,
      18,    2,  108,    2, 0x08,   17 /* Private */,
      19,    1,  113,    2, 0x08,   20 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QVector3D,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QVector3D,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QVector3D,   13,   17,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

Q_CONSTINIT const QMetaObject AudioWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSAudioWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAudioWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAudioWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AudioWidget, std::true_type>,
        // method 'playAudio'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updatePosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newOcclusion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'modeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fileChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'openFileDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateRoom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'animateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handlePositionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector3D, std::false_type>,
        // method 'handleFileDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector3D, std::false_type>,
        // method 'updateKnobs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector3D, std::false_type>,
        // method 'onSoundSourceSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void AudioWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudioWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->playAudio(); break;
        case 1: _t->updatePosition(); break;
        case 2: _t->newOcclusion(); break;
        case 3: _t->modeChanged(); break;
        case 4: _t->fileChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->openFileDialog(); break;
        case 6: _t->updateRoom(); break;
        case 7: _t->animateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->handlePositionChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVector3D>>(_a[2]))); break;
        case 9: _t->handleFileDropped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVector3D>>(_a[2]))); break;
        case 10: _t->updateKnobs((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVector3D>>(_a[2]))); break;
        case 11: _t->onSoundSourceSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *AudioWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAudioWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AudioWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
