#include "dataProcessThread.h"
#include <QVariantMap>
#include <QDebug>
dataProcessThread::dataProcessThread(QObject *parent/*=nullptr*/)
	:QObject(parent)
{
	qRegisterMetaType<QList<QPoint>>("QList<QPoint>");
	m_this_thread_ptr = new QThread(this);
	this->moveToThread(m_this_thread_ptr);
	initThread();
	connect(m_this_thread_ptr, &QThread::finished, this, &dataProcessThread::deleteLater);
}

dataProcessThread::~dataProcessThread()
{
	QMetaObject::invokeMethod(this, "closeThread", Qt::BlockingQueuedConnection);
}

void dataProcessThread::initThread()
{
	qDebug() << __FUNCTION__;
	if (m_this_thread_ptr)
	{
		m_this_thread_ptr->start();
	}
}

void dataProcessThread::closeThread()
{
	qDebug() << __FUNCTION__;
	if (m_this_thread_ptr)
	{
		m_this_thread_ptr->quit();
		m_this_thread_ptr->deleteLater();
		m_this_thread_ptr = nullptr;
	}
	// 清空数据
	m_channelPoints.clear();
}

void dataProcessThread::onRecvDoff(QSharedPointer<QJsonObject>json_sptr)
{
	qDebug() << __FUNCTION__;
	//获取一张片子的检测结果，拿到需要的信息（缺陷的坐标）
	if (json_sptr.isNull())
	{
		qDebug() << "__FUNCTION__" << "json is empty!!!" << json_sptr.isNull();
		return;
	}
	//拿到片检结果
	QJsonObject obj = json_sptr->value("doff").toObject();
	QVariantList flaws = obj.value("flaws").toVariant().toList();
	int currentChannel = obj.value("channel").toInt();
	for (const auto&flaw : flaws)
	{
		QVariantMap flawMap = flaw.toMap();
		//拿到缺陷分类 ok不算缺陷 排除
		int flaw_type = flawMap.value("flaw_class_type").toInt();
		if(flaw_type==8100)
			continue;
		//拿到缺陷的坐标
		int dCD = flawMap.value("d_cd").toInt();
		int dMD = flawMap.value("d_md").toInt();
		// 创建 QPoint 并设置坐标
		QPoint point(dCD, dMD);

		m_channelPoints[currentChannel].append(point);
	}
	emit sgResult(m_channelPoints);
}

void dataProcessThread::onClearData()
{
	m_channelPoints.clear();
}
