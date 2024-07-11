#include "GridWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#pragma execution_character_set("utf-8")
GridWidget::GridWidget(int rows, int cols, QWidget *parent /*= nullptr*/)
	:QWidget(parent),
	m_rows(rows),m_cols(cols)
	, m_gridWidth(0),m_gridHeight(0)
	,m_maxPointCount(0),m_minPointCount(0)
{
	// ��ʼ�� m_pointCounts
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
	for (const auto&point : points)
	{
		m_points.append(adjustPointToGrid(point));//����һ�²��Ϸ��ĵ���縺���򳬹������ֵ
	}
	//ͳ��ÿ�������е������
	countPointsInGrid(); 
}

void GridWidget::clearPoints()
{
	m_points.clear();
	countPointsInGrid();
	// ���»��Ʊ���ͼ��
	update();
}

void GridWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	//��QImage���Ƶ�������
	painter.drawImage(0, 0, m_image);
	//���Ʒ���painterEvent��ȽϿ�
	drawGrid(painter);
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
	// ���� QImage ��С�����»���
	m_image = QImage(size(), QImage::Format_ARGB32);
	m_image.fill(Qt::transparent);
	update();
	QWidget::resizeEvent(event);
}

void GridWidget::drawGrid(QPainter &painter)
{
	int leftMargin = 40;  // ���߾�
	int topMargin = 40;   // �����߾�

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);

	// ��������
	QFont font = painter.font();
	font.setPointSize(16);
	font.setBold(true);
	painter.setFont(font);

	// ��������
	drawGridBackground(painter, leftMargin, topMargin, m_gridWidth, m_gridHeight);
	drawGridLines(painter, leftMargin, topMargin, m_gridWidth, m_gridHeight);
	// �ָ�Ĭ������
	QFont defaultFont = painter.font();
	defaultFont.setPointSize(10);
	defaultFont.setBold(false);
	painter.setFont(defaultFont);
	//����λ
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
				color = QColor::fromRgb(255, 255, 255); // ��ɫ
			}
			else
			{
				// ʹ�����Բ�ֵ������ɫ
				float ratio = (float)(count - m_minPointCount) / (m_maxPointCount - m_minPointCount);
				int red = (int)(255 * ratio);
				int green = 255;
				int blue = 255;
				color = QColor::fromRgb(255, green - red, blue - red); // �Ӱ�ɫ����ɫ
			}

			// �������񱳾���ɫ
			painter.fillRect(leftMargin + j * gridWidth, topMargin + i * gridHeight, gridWidth, gridHeight, color);

			// ���Ƶ�����
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
	if (m_rows > 0)
	{
		for (int i = 0; i <= m_rows; ++i)
		{
			QString label = QString::number(i * gridHeight);
			if (i == m_rows) // ��������һ�У���� " mm"
			{
				label += " ( mm )";
			}
			painter.drawText(10, topMargin + i * gridHeight + 10, label);
		}
	}
	if (m_cols > 0)
	{
		for (int j = 0; j <= m_cols; ++j)
		{
			QString label = QString::number(j * gridWidth);
			if (j == m_cols) // ��������һ�У���� " mm"
			{
				label += " ( mm )";
			}
			painter.drawText(leftMargin + j * gridWidth - 10, 30, label);
		}
	}
}

void GridWidget::countPointsInGrid()
{
	// ���ͳ������
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)
		{
			m_pointCounts[i][j] = 0;
		}
	}
	// ͳ��ÿ�������еĵ�����
	for (const QPoint &point : m_points)
	{
		int row = point.y() / m_gridHeight; 
		int col = point.x() / m_gridWidth; 
		if (row >= 0 && row < m_rows && col >= 0 && col < m_cols)
		{
			m_pointCounts[row][col]++;
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
}

QPoint GridWidget::adjustPointToGrid(const QPoint&point) const
{
	int x = point.x();
	int y = point.y();
	// ���� x ����ķ�Χ
	if (x < 0) 
	{
		x = 0;
	}
	else if (x >= m_cols * m_gridWidth) 
	{
		x = m_cols * m_gridWidth-1;
	}
	// ���� y ����ķ�Χ
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
	//����m_pointsCounts��С
	m_pointCounts.resize(rows);
	for (int i = 0; i < rows; ++i)
	{
		m_pointCounts[i].resize(cols);
	}
	//����ͳ�Ƶ㲢����ͼ��
	countPointsInGrid();
	update();
}

void GridWidget::setGridMargin(int gridWidth, int gridHeight)
{
	m_gridWidth = gridWidth;
	m_gridHeight = gridHeight;
	countPointsInGrid();
	update();
}

void GridWidget::updatePoints(const QList<QPoint>& points)
{
	setPoints(points);
	update();
}

