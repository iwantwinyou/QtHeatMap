#pragma once

#include <QtCore/qglobal.h>
#include <qwidget.h>
#ifndef BUILD_STATIC
# if defined(LPHEATMAP_LIB)
#  define LPHEATMAP_EXPORT Q_DECL_EXPORT
# else
#  define LPHEATMAP_EXPORT Q_DECL_IMPORT
# endif
#else
# define LPHEATMAP_EXPORT
#endif

class LPHEATMAP_EXPORT lpHeatMapMgr :public QWidget
{
	Q_OBJECT
public:
	lpHeatMapMgr(QWidget *parent = nullptr):QWidget(parent) {}
	lpHeatMapMgr(int rows, int cols, int gridWidth, int gridHeight, QWidget *parent = nullptr) :QWidget(parent),m_rows(rows),m_cols(cols),m_gridWidth(gridWidth),m_gridHeight(gridHeight) {}
	virtual ~lpHeatMapMgr() {}

	//设置热力图坐标系行列
	virtual void setGridSize(int rows, int cols) = 0;
	//设置网格高度，宽度
	virtual void setGridMargin(int gridWidth, int gridHeight) = 0;
	//设置数据
	virtual void setTabData(int channelCount, const QList<QPoint>&pointData)=0;
public slots:
	virtual void onReset() = 0;
	virtual void onRecvDoffHeatMap(QSharedPointer<QJsonObject>json_sptr) = 0;
public:
	int m_rows;
	int m_cols;
	int m_gridWidth;
	int m_gridHeight;
};

typedef QSharedPointer<lpHeatMapMgr>LPHEATMAP_SPTR;
LPHEATMAP_EXPORT QSharedPointer<lpHeatMapMgr>lpCreateHeatMap(int rows,int cols,int gridWidth,int gridHeight,QWidget *parent = nullptr);