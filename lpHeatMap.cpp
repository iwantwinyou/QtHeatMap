#include "lpHeatMap.h"
#include <QtWidgets>
#include "UiAlarm.h"
#include "include/lperror.h"
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
	qDebug() << "LP_VERSION_DLL ""V1.0.3.0 " __FUNCTION__;
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
		m_tabWidget->addTab(page, "Channel" + QString::number(i + 1));
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
		// 检查 channel 是否在有效范围 [1, maxChannelId]
		if (channel >= 1 && channel <= m_gridWidgets.size()) {
			m_gridWidgets[channel - 1]->updatePoints(points); // channel-1 转为索引
		}
		else {
			qWarning() << "Invalid channel number:" << channel;
		}
	}
}

void lpHeatMap::onReset()
{
	qDebug() << __FUNCTION__ << "clicked Reset!";
	clearDataPoints();
	m_pThread->onClearData();
}

void lpHeatMap::onGetWarnInfo()
{
	GridWidget* widget = qobject_cast<GridWidget*>(sender());
	if (nullptr == widget)
		return;
	QVector<WarningInfo> data;//获取报警信息，缺陷数及坐标点
	bool ok=widget->getWarnData(data);
	if (!ok || data.isEmpty())return;
	int consecutiveDoffCount = widget->getConsecutiveDoffs();
	int flawCountThreshold = widget->getFlawCountThreshold();
	QString warningMessage= QString("过去%1片产品中检测到超出阈值(%2)的缺陷: 位置:").arg(consecutiveDoffCount).arg(flawCountThreshold);
	for (const auto& warning : data) {
		warningMessage += QString("(%1,%2),")
			.arg(warning.row)
			.arg(warning.col);
	}
	emit sgAddLog(warningMessage, true);
	QVariantMap vm;
	vm["des"] = "缺陷报警";
	vm["suggest"] = "请查看缺陷位置！";
	UiAlarm::instance()->f_show(warningMessage, 2, vm);
}

void lpHeatMap::initUI()
{
	this->setWindowTitle("HeatMap");
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

	m_resetBtn = new QPushButton("Reset", this);
	m_resetBtn->setStyleSheet("background:red;color:white");
	m_resetBtn->setFixedSize(100, 30);
	bottomHLayout->addStretch();
	bottomHLayout->addWidget(m_resetBtn);

	connect(m_resetBtn, &QPushButton::clicked, this, &lpHeatMap::onReset);
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

void lpHeatMap::setReSetBtn(bool is_show)
{
	if (is_show) {
		m_resetBtn->show();
	}
	else {
		m_resetBtn->hide();
	}
}

GridWidget * lpHeatMap::createDefaultGridWidget(QWidget *parent /*= nullptr*/)
{
	GridWidget* widget = new GridWidget(m_rows, m_cols, parent);
	widget->setGridMargin(m_gridWidth, m_gridHeight);
	connect(widget, &GridWidget::sgSendWarn, this, &lpHeatMap::onGetWarnInfo);
	return widget;
}

void lpHeatMap::initializeDefaultTabs(int channel)
{
	for (int i = 0; i < channel; ++i) 
	{
		GridWidget *page = createDefaultGridWidget(this);
		page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_tabWidget->addTab(page, "Channel" + QString::number(i + 1));
		m_gridWidgets.append(page);
	}
}
