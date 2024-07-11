/********************************************************************************
** Form generated from reading UI file 'QtHeatMapTest.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTHEATMAPTEST_H
#define UI_QTHEATMAPTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtHeatMapTestClass
{
public:

    void setupUi(QWidget *QtHeatMapTestClass)
    {
        if (QtHeatMapTestClass->objectName().isEmpty())
            QtHeatMapTestClass->setObjectName(QStringLiteral("QtHeatMapTestClass"));
        QtHeatMapTestClass->resize(600, 400);

        retranslateUi(QtHeatMapTestClass);

        QMetaObject::connectSlotsByName(QtHeatMapTestClass);
    } // setupUi

    void retranslateUi(QWidget *QtHeatMapTestClass)
    {
        QtHeatMapTestClass->setWindowTitle(QApplication::translate("QtHeatMapTestClass", "QtHeatMapTest", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtHeatMapTestClass: public Ui_QtHeatMapTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTHEATMAPTEST_H
