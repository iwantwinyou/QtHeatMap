/*
file:GridWidget.h
date:2024/6/25
brief:����ؼ���װ  ��������ͼչʾȱ�ݸ��������Ҹ���ȱ�ݸ��� ��ʾ��ͬ��ɫ(����Ȩ�����ý���ɫ)
author:wuchaoxi

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

	//���������
	void setPoints(const QList<QPoint>&points);
	//���
	void clearPoints();
	//������������
	void setGridSize(int rows, int cols);
	//�����������и��Ӽ�ļ��
	void setGridMargin(int gridWidth, int gridHeight);
	//���µ�����ͳ��
	void updatePoints(const QList<QPoint>& points);
protected:
	void paintEvent(QPaintEvent *event)override;
	void resizeEvent(QResizeEvent *event)override;
private:
	void drawGrid(QPainter &painter);
	//������
	void drawGridBackground(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//�� ��
	void drawGridLines(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//����λ(mm)
	void drawGridLabels(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight);
	//ͳ����������
	void countPointsInGrid();
	//����������ĺ��������Ǹ���������Ϊ0�������������ֵ������Ϊ���ֵ��
	QPoint adjustPointToGrid(const QPoint&point)const;
private:
	int			m_rows;
	int			m_cols;
	int			m_gridWidth;
	int			m_gridHeight;
	int			m_maxPointCount;
	int			m_minPointCount;
	QList<QPoint>				m_points; // �洢���е������
	QVector<QVector<int>>		m_pointCounts; // �洢ÿ�������еĵ�����
	QImage						m_image;
};



