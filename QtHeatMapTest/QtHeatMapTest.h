#pragma once

#include "../lpHeatMap/lpheatmap_global.h"
#include <QtWidgets/QWidget>
#include "ui_QtHeatMapTest.h"

#if _DEBUG
#pragma comment (lib, "lpHeatMapd.lib")

#else
#pragma comment (lib, "lpHeatMap.lib")
#endif

class QtHeatMapTest : public QWidget
{
	Q_OBJECT

public:
	QtHeatMapTest(QWidget *parent = Q_NULLPTR);
private slots:
	void onGetData2Map();
	void startPressTest();
	void stopPressTest();

private:
	Ui::QtHeatMapTestClass ui;

	LPHEATMAP_SPTR   m_heatMapPtr{nullptr};
	QTimer*   m_timer;
};
