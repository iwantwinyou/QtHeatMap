#include "dataProcessThread.h"
#include <QVariantMap>
#include <QDebug>
#include "lpqtConfig/include/lpServerSheetCfg.h"
dataProcessThread::dataProcessThread(QObject *parent/*=nullptr*/)
	:QObject(parent)
{
	qRegisterMetaType<QList<QPoint>>("QList<QPoint>");
	m_this_thread_ptr = new QThread;
	this->moveToThread(m_this_thread_ptr);
	m_ok_box = lpServerSheetCfg::instance()->getCfg()->t_srv_cfg_.r_ss_box.ok_box;
	QVariantMap extend_vmp = lpServerSheetCfg::instance()->getCfg()->t_srv_cfg_.extend_funtion_.toMap();
	QVariantMap info_mp = extend_vmp.value("heat_map_info").toMap();
	m_count_type = info_mp.value("count_type", 1).toInt();
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
	//获取一张片子的检测结果，拿到需要的信息（缺陷的坐标）
	if (json_sptr.isNull())
		return;
	//拿到片检结果
	QJsonObject obj = json_sptr->value("doff").toObject();
	if (obj.isEmpty())
		return;// 加doff为空的防呆
	QVariantList flaws = obj.value("flaws").toVariant().toList();
	int currentChannel = obj.value("channel").toInt();
	int box_id = obj.value("box_id").toInt();
	if (m_count_type == 2){
		m_channelPoints.clear();
	}
	if (box_id == m_ok_box) {//如果是OK片，下面都不需要进行处理，下面流程的目的时为了拿到缺陷坐标
		m_channelPoints[currentChannel].append(QPoint(0, 0));
		emit sgResult(m_channelPoints);
		return;
	}	
	for (const auto&flaw : flaws)
	{
		QVariantMap flawMap = flaw.toMap();
		//拿到缺陷分类 ok不算缺陷 排除
		bool is_show = flawMap.value("is_show").toBool();//不勾选的缺陷不算入缺陷
		int flaw_type = flawMap.value("flaw_class_type").toInt()/100*100;
		if(flaw_type==8100||!is_show)
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
