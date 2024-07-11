/*
file:lpHeatMap.h
date:2024/6/24
brief:����ͼģ��
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
	//����ָ��ҳ���������
	void setPointsForPage(int pageIndex, const QList<QPoint>&points);
	// ��ʼ����ǩҳ�����ݴ���Ĳ�������ͨ������������
	void initializeTabs(int channelCount, const QVector<QList<QPoint>> &channelData);
	//��ʼ�������ݴ���Ĳ�������ͨ�����������ݣ����ݵ��ӣ�
	void setTabData(int channelCount, const QList<QPoint>&pointData)override;
	//��������ϵĵ�ͳ��
	void clearDataPoints();
private slots:
	//�յ�һ��Ƭ�ӵ���Ϣ
	void onRecvDoffHeatMap(QSharedPointer<QJsonObject>json_sptr)override;
	//�����յ�Ƭ�ӵ�������Ϣ��������ͼ����
	void onUpdateHeatMapData(int channel,QList<QPoint> points);
	//��λ��������ۼƸ���
	void onReset()override;
signals:
	void sgThreadData(QSharedPointer<QJsonObject>json_sptr);
private:
	void initUI();
	//��������
	void setGridSize(int rows, int cols)override;
	//��������߶ȣ����
	void setGridMargin(int gridWidth, int gridHeight) override;
	//��������
	GridWidget *createDefaultGridWidget(QWidget *parent = nullptr);
	//��ʼ��Ĭ�ϱ�ǩҳ Ĭ��Ϊ1
	void initializeDefaultTabs(int channel);
	void adjustWidgetSize();
private:
	QVector<GridWidget*>			m_gridWidgets; // �洢���е� GridWidget
	QTabWidget*						m_tabWidget; // �洢��ǩҳ�ؼ�	
	QList<QPoint>					m_points;	// ����һ�����ڱ���ȱ�ݵ�������б�	
	int m_rows;
	int m_cols;
	int m_gridWidth;
	int m_gridHeight;
	dataProcessThread*				m_pThread{ nullptr };
};
