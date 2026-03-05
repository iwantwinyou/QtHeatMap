#include "GridWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include "lpqtConfig/include/lpServerSheetCfg.h"
#pragma execution_character_set("utf-8")
GridWidget::GridWidget(int rows, int cols, QWidget *parent /*= nullptr*/)
	:QWidget(parent),
	m_rows(rows),m_cols(cols)
	, m_gridWidth(0),m_gridHeight(0)
	,m_maxPointCount(0),m_minPointCount(0)
{
	QVariantMap extend_vmp = lpServerSheetCfg::instance()->getCfg()->t_srv_cfg_.extend_funtion_.toMap();
	QVariantMap info_mp = extend_vmp.value("heat_map_info").toMap();
	m_flawCountThreshold = info_mp.value("flaw_count_threshold",10).toInt();
	m_consecutive_doffs = info_mp.value("consecutive_doffs", 20).toInt();
	m_count_type = info_mp.value("count_type", 1).toInt();
	// 初始化 m_pointCounts
	m_pointCounts.resize(rows);
	for (int i = 0; i < rows; ++i)
	{
		m_pointCounts[i].resize(cols);
	}
	m_image = QImage(size(), QImage::Format_ARGB32);
	m_image.fill(Qt::transparent);
}

GridWidget::~GridWidget()
{

}

void GridWidget::setPoints(const QList<QPoint>&points)
{
	m_points.clear();
	if (m_count_type == 1) {
		for (const auto&point : points)
		{
			m_points.append(adjustPointToGrid(point));//处理一下不合法的点比如负数或超过了最大值
		}
	}
	else if (m_count_type == 2) {
		m_doffInfo.enqueue(points);
		while (m_doffInfo.size() > m_consecutive_doffs) {
			m_doffInfo.dequeue();
		}
	}
	//统计每个网格中点的数量
	countPointsInGrid(true); 
}

void GridWidget::clearPoints()
{
	m_points.clear();
	m_doffInfo.clear();
	countPointsInGrid(false);
}

void GridWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	//将QImage绘制到窗口上
	painter.drawImage(0, 0, m_image);
	//绘制放入painterEvent会比较快
	drawGrid(painter);
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	// 调整 QImage 大小并重新绘制
	m_image = QImage(size(), QImage::Format_ARGB32);
	m_image.fill(Qt::transparent);
	update();
}

void GridWidget::drawGrid(QPainter &painter)
{
	int leftMargin = 50;  // 左侧边距
	int topMargin = 40;   // 顶部边距

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);

	// 设置字体
	QFont font = painter.font();
	font.setPointSize(16);
	font.setBold(true);
	painter.setFont(font);
	int tmGridWidth, tmGgridHeight;
	if (m_cols == 0 || m_rows == 0)
	{
		//初始化时，网格行列为0，此时处理一下
		drawGridBackground(painter, leftMargin, topMargin, m_gridWidth, m_gridHeight);
		drawGridLines(painter, leftMargin, topMargin, m_gridWidth, m_gridHeight);
	}
	else
	{
		//实际的间距  要间距左右间距 上下间距
		tmGridWidth = (size().width() - 100) / m_cols;
		tmGgridHeight = (size().height() - 100) / m_rows;
		// 绘制网格
		drawGridBackground(painter, leftMargin, topMargin, tmGridWidth, tmGgridHeight);
		drawGridLines(painter, leftMargin, topMargin, tmGridWidth, tmGgridHeight);
	}

	// 恢复默认字体
	QFont defaultFont = painter.font();
	defaultFont.setPointSize(10);
	defaultFont.setBold(false);
	painter.setFont(defaultFont);
	//画单位
	drawGridLabels(painter, leftMargin, topMargin, m_gridWidth, m_gridHeight);
}

void GridWidget::drawGridBackground(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight)
{
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)
		{
			int count = m_pointCounts[i][j];
			QColor color;
			if (m_maxPointCount == m_minPointCount)
			{
				color = QColor::fromRgb(255, 255, 255); // 白色
			}
			else
			{
				// 使用线性插值计算颜色
				float ratio = (float)(count - m_minPointCount) / (m_maxPointCount - m_minPointCount);
				int red = (int)(255 * ratio);
				int green = 255;
				int blue = 255;
				color = QColor::fromRgb(255, green - red, blue - red); // 从白色到红色
			}

			// 绘制网格背景颜色
			painter.fillRect(leftMargin + j * gridWidth, topMargin + i * gridHeight, gridWidth, gridHeight, color);

			// 绘制点数量
			int x = leftMargin + j * gridWidth + gridWidth / 2;
			int y = topMargin + i * gridHeight + gridHeight / 2;
			painter.drawText(x, y, QString::number(count));
		}
	}
}

void GridWidget::drawGridLines(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight)
{
	painter.setPen(QPen(Qt::black, 2));

	for (int i = 0; i <= m_rows; ++i)
	{
		painter.drawLine(leftMargin, topMargin + i * gridHeight, leftMargin + m_cols * gridWidth, topMargin + i * gridHeight);
	}

	for (int j = 0; j <= m_cols; ++j)
	{
		painter.drawLine(leftMargin + j * gridWidth, topMargin, leftMargin + j * gridWidth, topMargin + m_rows * gridHeight);
	}
}

void GridWidget::drawGridLabels(QPainter &painter, int leftMargin, int topMargin, int gridWidth, int gridHeight)
{
	painter.setPen(Qt::white);
	//实际的尺寸
	int real_width = size().width() - 100;
	int real_height = size().height() - 100;
	if (m_rows > 0)
	{
		for (int i = 0; i <= m_rows; ++i)
		{
			QString label = QString::number(i * gridHeight);
			if (i == m_rows) // 如果是最后一行，添加 " mm"
			{
				label += " ( mm )";
			}//跟随实际的高度画 传入数据的间隔距离
			if (m_cols == 0 || m_rows == 0)
			{//初始化 热力图时 做一下处理
				painter.drawText(10, topMargin + i * gridHeight + 10, label);
			}
			else
			{
				painter.drawText(10, topMargin + i * real_height / m_rows + 10, label);
			}
			
		}
	}
	if (m_cols > 0)
	{
		for (int j = 0; j <= m_cols; ++j)
		{
			QString label = QString::number(j * gridWidth);
			if (j == m_cols) // 如果是最后一列，添加 " mm"
			{
				label += " ( mm )";
			}//跟随实际的宽度画 传入数据的间隔距离
			if (m_cols == 0 || m_rows == 0)
			{//初始化 热力图时 做一下处理
				painter.drawText(leftMargin + j * gridWidth - 25, 30, label);
			}
			else
			{
				painter.drawText(leftMargin + j * real_width / m_cols - 25, 30, label);
			}
			
		}
	}
}

void GridWidget::countPointsInGrid(bool isSend)
{
	// 清空统计数据
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)
		{
			m_pointCounts[i][j] = 0;
		}
	}
	bool hasWarn= false;
	if (m_count_type == 1) {
		// 统计每个网格中的点数量
		for (const QPoint &point : m_points)
		{
			int row = point.y() / m_gridHeight;
			int col = point.x() / m_gridWidth;
			if (point==QPoint(0,0)) {
				continue; // 跳过这个点，不计数
			}
			if (row >= 0 && row < m_rows && col >= 0 && col < m_cols)
			{
				m_pointCounts[row][col]++;
				//if (m_pointCounts[row][col] > m_flawCountThreshold)//预留报警，该网格缺陷超过设定阈值
				//	hasWarn = true;
			}
		}
	}
	else if (m_count_type == 2){
		// 重新统计每个网格有多少片出现过缺陷
		for (const  QList<QPoint>& pts : m_doffInfo) {
			QVector<QVector<bool>> gridHit(m_rows, QVector<bool>(m_cols, false));//网格是否有缺陷，默认false
			for (const QPoint& pt : pts) {
				if (pt == QPoint(0, 0)) continue;//ok片设置坐标为（0，0）,当作空，不参与计数
				int row = pt.y() / m_gridHeight;
				int col = pt.x() / m_gridWidth;
				if (row >= 0 && row < m_rows && col >= 0 && col < m_cols)
					gridHit[row][col] = true;// 标记该网格被当前片子命中,即该片子在该网格有缺陷
			}
			for (int r = 0; r < m_rows; ++r) {
				for (int c = 0; c < m_cols; ++c) {
					if (gridHit[r][c]) {
						m_pointCounts[r][c] += 1;
					}
					if (m_pointCounts[r][c] > m_flawCountThreshold) {
						hasWarn = true;
					}
				}
			}			
		}
	}

	m_maxPointCount = 0;
	m_minPointCount = INT_MAX;
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)
		{
			if (m_pointCounts[i][j] > m_maxPointCount)
				m_maxPointCount = m_pointCounts[i][j];
			if (m_pointCounts[i][j] < m_minPointCount)
				m_minPointCount = m_pointCounts[i][j];
		}
	}

	if (m_minPointCount == INT_MAX) m_minPointCount = 0;
	update();//先刷新，再发出报警
	if (isSend&&hasWarn){
		emit sgSendWarn();
	}
}

QPoint GridWidget::adjustPointToGrid(const QPoint&point) const
{
	int x = point.x();
	int y = point.y();
	// 限制 x 坐标的范围
	if (x < 0) 
	{
		x = 0;
	}
	else if (x >= m_cols * m_gridWidth) 
	{
		x = m_cols * m_gridWidth-1;
	}
	// 限制 y 坐标的范围
	if (y < 0) 
	{
		y = 0;
	}
	else if (y >= m_rows * m_gridHeight) 
	{
		y = m_rows * m_gridHeight-1;
	}

	return QPoint(x, y);
}

void GridWidget::setGridSize(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	//调整m_pointsCounts大小
	m_pointCounts.resize(rows);
	for (int i = 0; i < rows; ++i)
	{
		m_pointCounts[i].resize(cols);
	}
	//重新统计点并更新图像
	countPointsInGrid(true);
}

void GridWidget::setGridMargin(int gridWidth, int gridHeight)
{
	m_gridWidth = gridWidth;
	m_gridHeight = gridHeight;
	countPointsInGrid(true);
}

void GridWidget::updatePoints(const QList<QPoint>& points)
{
	setPoints(points);
}

bool GridWidget::getWarnData(QVector<WarningInfo> &data)
{
	for (int i = 0; i < m_rows; ++i){
		for (int j = 0; j < m_cols; ++j){
			int count = m_pointCounts[i][j];
			if (count != 0&&count>m_flawCountThreshold) {//如果缺陷数不为0才把数据塞到数组里，让报警的地方自己来取一些数据，比如缺陷数跟缺陷大致坐标（0，1）...
				// 计算该网格单元的坐标范围
				int x1 = j * m_gridWidth;
				int y1 = i * m_gridHeight;
				int x2 = x1 + m_gridWidth;
				int y2 = y1 + m_gridHeight;
				data.push_back({ count,i,j,x1,x2,y1,y2});
			}
		}
	}
	return !data.isEmpty();
}

int GridWidget::getConsecutiveDoffs() const
{
	return m_doffInfo.size();
}

int GridWidget::getFlawCountThreshold() const
{
	return m_flawCountThreshold;
}
