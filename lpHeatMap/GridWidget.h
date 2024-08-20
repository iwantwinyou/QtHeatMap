/*
file:GridWidget.h
date:2024/6/25
brief:网格控件封装  用于热力图展示缺陷个数，并且根据缺陷个数 显示不同颜色(根据权重设置渐变色)
author:wuchaoxi
copyright:杭州利珀科技有限公司
*/

#pragma once
#include <qwidget.h>
#include <QPoint>
#include <QImage>

class GridWidget :public QWidget
{
public:
	GridWidget(int rows,int cols,QWidget *parent = nullptr);
	virtual ~GridWidget();

	//设置坐标点
	void setPoints(const QList<QPoint>&points);
	//清空
	void clearPoints();
	//设置网格行列
	void setGridSize(int rows, int cols);
	//设置网格行列格子间的间距
	void setGridMargin(int gridWidth, int gridHeight);
	//更新点坐标统计
	void updatePoints(const QList<QPoint>& points);
protected:
	void paintEvent(QPaintEvent *event)override;
	void resizeEvent(QResizeEvent *event)override;
private:
	void drawGrid(QPainter &painter);
	//画背景
	void drawGridBackground(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//画 线
	void drawGridLines(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//画单位(mm)
	void drawGridLabels(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//统计坐标点个数
	void countPointsInGrid();
	//调整点坐标的函数（如是负数则设置为0，超过坐标最大值则设置为最大值）
	QPoint adjustPointToGrid(const QPoint&point)const;
private:
	int			m_rows;
	int			m_cols;
	int			m_gridWidth;
	int			m_gridHeight;
	int			m_maxPointCount;
	int			m_minPointCount;
	QList<QPoint>				m_points; // 存储所有的坐标点
	QVector<QVector<int>>		m_pointCounts; // 存储每个网格中的点数量
	QImage						m_image;
};



