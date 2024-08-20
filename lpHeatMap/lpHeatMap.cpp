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
	// 注册信号参数类型，以便Qt的元系统知道如何传递它
	qRegisterMetaType<QMap<int, QList<QPoint>>>("QMap<int, QList<QPoint>>");
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

	// 清空原有的 tabs 和 widgets
	m_tabWidget->clear();
	m_gridWidgets.clear();

	for (int i = 0; i < channelCount; ++i)
	{
		//创建默认大小的网格
		GridWidget *page = createDefaultGridWidget(this);
		m_tabWidget->addTab(page, "通道" + QString::number(i + 1));
		m_gridWidgets.append(page);
		// 设置每个通道的坐标点
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
	m_channelPoints.clear();
	for (const auto&widget : m_gridWidgets)
	{
		widget->clearPoints();
	}
}

void lpHeatMap::onRecvDoffHeatMap(QSharedPointer<QJsonObject>json_sptr)
{
	//发送信号的数据处理类 到线程中进行数据处理
	emit sgThreadData(json_sptr);
}

void lpHeatMap::onUpdateHeatMapData(const QMap<int, QList<QPoint>>& channelDataMap)
{
	// 根据传入的 QMap 更新每个流道的数据
	for (auto it = channelDataMap.begin(); it != channelDataMap.end(); ++it) 
	{
		int channel = it.key();
		const QList<QPoint>& points = it.value();
		// 确保标签页存在
		if (channel-1 < m_gridWidgets.size()) 
		{
			m_gridWidgets[channel-1]->updatePoints(points);
		}
	}
}

void lpHeatMap::onReset()
{
	qDebug() << __FUNCTION__ << "clicked Reset!";
	clearDataPoints();
	m_pThread->onClearData();
}

void lpHeatMap::initUI()
{
	this->setWindowTitle("热力图");
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	m_tabWidget = new QTabWidget(this);
	mainLayout->addWidget(m_tabWidget);
	// 下部清除按钮
	QHBoxLayout *bottomHLayout = new QHBoxLayout();
	bottomHLayout->setMargin(4);
	bottomHLayout->setSpacing(0);
	mainLayout->addLayout(bottomHLayout);

	QPushButton *clearBtn = new QPushButton("复位", this);
	clearBtn->setStyleSheet("background:red;color:white");
	clearBtn->setFixedSize(100, 30);
	bottomHLayout->addStretch();
	bottomHLayout->addWidget(clearBtn);

	connect(clearBtn, &QPushButton::clicked, this, &lpHeatMap::onReset);
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

void lpHeatMap::setChannelTabs(int channelCount)
{
	m_tabWidget->clear();
	m_gridWidgets.clear();
	//根据外部传入的通道数初始化 tab页显示
	initializeDefaultTabs(channelCount);
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
		GridWidget *page = createDefaultGridWidget(this);
		page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_tabWidget->addTab(page, "通道" + QString::number(i + 1));
		m_gridWidgets.append(page);
	}
}
