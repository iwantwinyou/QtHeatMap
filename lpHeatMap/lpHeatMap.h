/*
file:lpHeatMap.h
date:2024/6/24
brief:热力图模块
author:wuchaoxi

*/
#pragma once

#include "lpheatmap_global.h"
#include "GridWidget.h"
#include <QTabWidget>
#include "dataProcessThread.h"
class GriWidget;
class LPHEATMAP_EXPORT lpHeatMap:public lpHeatMapMgr
{
	Q_OBJECT
public:
	lpHeatMap(int rows, int cols, int gridWidth, int gridHeight, QWidget *parent = nullptr);
	virtual ~lpHeatMap();
	//设置指定页坐标点数据
	void setPointsForPage(int pageIndex, const QList<QPoint>&points);
	// 初始化标签页，根据传入的参数设置通道数量和数据
	void initializeTabs(int channelCount, const QVector<QList<QPoint>> &channelData);
	//初始化，根据传入的参数设置通道数量和数据（数据叠加）
	void setTabData(int channelCount, const QList<QPoint>&pointData)override;
	//清除界面上的点统计
	void clearDataPoints();
private slots:
	//收到一张片子的信息
	void onRecvDoffHeatMap(QSharedPointer<QJsonObject>json_sptr)override;
	//根据收到片子的坐标信息更新热力图数据
	void onUpdateHeatMapData(int channel,QList<QPoint> points);
	//复位，清除点累计个数
	void onReset()override;
signals:
	void sgThreadData(QSharedPointer<QJsonObject>json_sptr);
private:
	void initUI();
	//设置行列
	void setGridSize(int rows, int cols)override;
	//设置网格高度，宽度
	void setGridMargin(int gridWidth, int gridHeight) override;
	//创建网格
	GridWidget *createDefaultGridWidget(QWidget *parent = nullptr);
	//初始化默认标签页 默认为1
	void initializeDefaultTabs(int channel);
	void adjustWidgetSize();
private:
	QVector<GridWidget*>			m_gridWidgets; // 存储所有的 GridWidget
	QTabWidget*						m_tabWidget; // 存储标签页控件	
	QList<QPoint>					m_points;	// 创建一个用于保存缺陷点坐标的列表	
	int m_rows;
	int m_cols;
	int m_gridWidth;
	int m_gridHeight;
	dataProcessThread*				m_pThread{ nullptr };
};
