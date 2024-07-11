#include "lpHeatMap.h"
#include <QtWidgets>
#pragma execution_character_set("utf-8")

QSharedPointer<lpHeatMapMgr>lpCreateHeatMap(int rows,int cols,int gridWidth,int gridHeight,QWidget *parent /* = nullptr */)
{
	return QSharedPointer<lpHeatMapMgr>(new lpHeatMap(rows,cols,gridWidth,gridHeight,parent));
}

lpHeatMap::lpHeatMap(int rows, int cols, int gridWidth, int gridHeight, QWidget *parent /*= nullptr*/)
	: lpHeatMapMgr(parent),
	m_tabWidget(nullptr),
	m_rows(rows),
	m_cols(cols),
	m_gridWidth(gridWidth),
	m_gridHeight(gridHeight)
{
	initUI();
	m_pThread = new dataProcessThread;
	qRegisterMetaType<QList<QPoint>>("QList<QPoint>");
	connect(this, &lpHeatMap::sgThreadData, m_pThread, &dataProcessThread::onRecvDoff);
	connect(m_pThread, &dataProcessThread::sgResult, this, &lpHeatMap::onUpdateHeatMapData);
}

lpHeatMap::~lpHeatMap()
{

}

void lpHeatMap::setPointsForPage(int pageIndex, const QList<QPoint>&points)
{
	if (pageIndex < 0 || pageIndex >= m_gridWidgets.size())
	{
		qDebug() << "Invalid page index:" << pageIndex << __FUNCTION__;
		return;
	}
	m_gridWidgets[pageIndex]->setPoints(points);
}

void lpHeatMap::initializeTabs(int channelCount, const QVector<QList<QPoint>> &channelData)
{
	if (channelData.size() != channelCount)
	{
		qDebug() << "Channel count and data size mismatch!" << __FUNCTION__;
		return;
	}

	// ���ԭ�е� tabs �� widgets
	m_tabWidget->clear();
	m_gridWidgets.clear();

	for (int i = 0; i < channelCount; ++i)
	{
		QScrollArea *scrollArea = new QScrollArea(this);
		scrollArea->setWidgetResizable(true);
		//����Ĭ�ϴ�С������
		GridWidget *page = createDefaultGridWidget(this);
		m_tabWidget->addTab(scrollArea, "ͨ��" + QString::number(i + 1));
		scrollArea->setWidget(page);
		m_gridWidgets.append(page);
		// ����ÿ��ͨ���������
		setPointsForPage(i, channelData[i]);
	}
}

void lpHeatMap::setTabData(int channelCount, const QList<QPoint>&pointData)
{
	for (int i = 0; i < channelCount; ++i)
	{
		m_gridWidgets[i]->updatePoints(pointData);
	}
}

void lpHeatMap::clearDataPoints()
{
	m_points.clear();
	for (const auto&widget : m_gridWidgets)
	{
		widget->clearPoints();
	}
}

void lpHeatMap::onRecvDoffHeatMap(QSharedPointer<QJsonObject>json_sptr)
{
	//�����źŵ����ݴ����� ���߳��н������ݴ���
	emit sgThreadData(json_sptr);
}
void lpHeatMap::onUpdateHeatMapData(int channel, QList<QPoint> points)
{
	setTabData(channel, points);
}

void lpHeatMap::onReset()
{
	qDebug() << __FUNCTION__ << "clicked Reset!";
	clearDataPoints();
	m_pThread->onClearData();
}

void lpHeatMap::initUI()
{
	this->setWindowTitle("����ͼ");
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	m_tabWidget = new QTabWidget(this);
	mainLayout->addWidget(m_tabWidget);
	//Ĭ��ͨ����
	initializeDefaultTabs(1);
	// �²������ť
	QHBoxLayout *bottomHLayout = new QHBoxLayout();
	bottomHLayout->setMargin(4);
	bottomHLayout->setSpacing(0);
	mainLayout->addLayout(bottomHLayout);

	QPushButton *clearBtn = new QPushButton("��λ", this);
	clearBtn->setStyleSheet("background:red;color:white");
	clearBtn->setFixedSize(100, 30);
	bottomHLayout->addStretch();
	bottomHLayout->addWidget(clearBtn);

	connect(clearBtn, &QPushButton::clicked, this, &lpHeatMap::onReset);
	adjustWidgetSize();
}

void lpHeatMap::setGridSize(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	for (const auto &gridWidget : m_gridWidgets)
	{
		gridWidget->setGridSize(rows, cols);
		gridWidget->update();
	}
}

void lpHeatMap::setGridMargin(int gridWidth, int gridHeight)
{
	for (const auto &gridWidget : m_gridWidgets)
	{
		gridWidget->setGridMargin(gridWidth, gridHeight);
		gridWidget->update();
	}
}

GridWidget * lpHeatMap::createDefaultGridWidget(QWidget *parent /*= nullptr*/)
{
	GridWidget* widget = new GridWidget(m_rows, m_cols, parent);
	widget->setGridMargin(m_gridWidth, m_gridHeight);
	return widget;
}

void lpHeatMap::initializeDefaultTabs(int channel)
{
	for (int i = 0; i < channel; ++i) 
	{
		QScrollArea *scrollArea = new QScrollArea(this);
		scrollArea->setWidgetResizable(true);
		GridWidget *page = createDefaultGridWidget(this);
		m_tabWidget->addTab(scrollArea, "ͨ��" + QString::number(i + 1));
		scrollArea->setWidget(page);
		m_gridWidgets.append(page);
	}
}

void lpHeatMap::adjustWidgetSize()
{
	int totalWidth = m_cols * m_gridWidth + 2 * 60; // �������ұ߾�
	int totalHeight = m_rows * m_gridHeight + 2 * 65; // ���Ƕ����͵ײ��߾�

	if (m_rows == 0 || m_cols == 0 || m_gridHeight == 0 || m_gridWidth == 0)
	{
		//û��������һ��Ĭ�ϴ�С�Ľ���
		this->resize(830, 830);
	}
	else
	{
		// ����������Ĵ�С�������������񲼾�
		this->resize(totalWidth, totalHeight);
	}
}