#include "dataProcessThread.h"
#include <QVariantMap>
#include <QDebug>
dataProcessThread::dataProcessThread(QObject *parent/*=nullptr*/)
	:QObject(parent)
{
	m_this_thread_ptr = new QThread(this);
	this->moveToThread(m_this_thread_ptr);
	initThread();
	qRegisterMetaType<QList<QPoint>>("QList<QPoint>");
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
	m_points.clear();
}

void dataProcessThread::onRecvDoff(QSharedPointer<QJsonObject>json_sptr)
{
	//获取一张片子的检测结果，拿到需要的信息（缺陷的坐标）
	if (json_sptr.isNull())
	{
		qDebug() << "__FUNCTION__" << "json is empty!!!" << json_sptr.isNull();
		return;
	}
	//拿到工单统计信息的流道数
	QVariantMap map = json_sptr->toVariantMap();
	QJsonObject doffStatisObj = json_sptr->value("doff_statis").toObject();
	int channelNum = doffStatisObj.value("channel_num").toInt();
	//拿到片检结果
	QJsonObject obj = json_sptr->value("doff").toObject();
	QVariantList flaws = obj.value("flaws").toVariant().toList();
	for (const auto&flaw : flaws)
	{
		QVariantMap flawMap = flaw.toMap();
		//拿到缺陷的坐标
		int dCD = flawMap.value("d_cd").toInt();
		int dMD = flawMap.value("d_md").toInt();
		//拿到缺陷的流道号
		int channel = flawMap.value("channel").toInt();
		// 创建 QPoint 并设置坐标
		QPoint point(dCD, dMD);
		if (channel >= 0 && channel < channelNum)
		{
			m_points.append(point);
		}
	}

	emit sgResult(channelNum, m_points);
}

void dataProcessThread::onClearData()
{
	m_points.clear();
}
