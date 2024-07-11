#include "QtHeatMapTest.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <random>
#include <ctime>
#include <chrono>
#include <QDebug>
#include <QTimer>
#pragma execution_character_set("utf-8")

QtHeatMapTest::QtHeatMapTest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("����ͼ����");

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);
	this->setLayout(mainLayout);

	QPushButton *pushButton = new QPushButton("��ʼ����", this);
	mainLayout->addWidget(pushButton);
	QPushButton *cancelBtn = new QPushButton("ֹͣ����", this); 
	mainLayout->addWidget(cancelBtn);

	connect(pushButton, &QPushButton::clicked, this, &QtHeatMapTest::startPressTest);
	connect(cancelBtn, &QPushButton::clicked, this, &QtHeatMapTest::stopPressTest);

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &QtHeatMapTest::onGetData2Map);
}

void QtHeatMapTest::onGetData2Map()
{
	// ���������������
	std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // ʹ��ʱ����Ϊ����
	std::uniform_int_distribution<int> distX(0, 1000); // ����x�ķ�Χ��0��1000
	std::uniform_int_distribution<int> distY(0, 1000); // ����y�ķ�Χ��0��1000


	QList<QPoint>pointList;
	for (int i = 0; i < 100000; ++i)
	{
		int x = distX(rng);
		int y = distY(rng);
		pointList.append(QPoint(x,y));
	}

	m_heatMapPtr->setTabData(1, pointList);
}

void QtHeatMapTest::startPressTest()
{
	if (m_timer->isActive())
		return;
	m_heatMapPtr = lpCreateHeatMap(5, 5, 140, 80, this);
	m_heatMapPtr->setParent(this, Qt::Dialog);
	m_heatMapPtr->show();
	m_timer->start(1000);
}

void QtHeatMapTest::stopPressTest()
{
	m_timer->stop();
	//delete m_heatMapPtr;
	//m_heatMapPtr = nullptr;
}
