/*
file:dataProcessThread.h
date:2024/7/8
brief:数据处理类  耗时操作丢入线程
author:wuchaoxi
copyright:杭州利珀科技有限公司
*/


#pragma once
#include <qobject.h>
#include <QJsonObject>
#include <QSharedPointer>
#include <QPoint>
#include <QThread>
#include <QMap>
class dataProcessThread :public QObject
{
	Q_OBJECT
public:
	dataProcessThread(QObject *parent=nullptr);
	virtual~dataProcessThread();

	void initThread();
	void closeThread();

signals:
	void sgResult(const QMap<int, QList<QPoint>>&channelDataMap);
public slots:
	void onRecvDoff(QSharedPointer<QJsonObject>json_sptr);
	void onClearData();
private:
	QThread* m_this_thread_ptr;
	QMap<int, QList<QPoint>> m_channelPoints;//全部片子累加的缺陷坐标
	int m_ok_box{-1};//box_id,1为OK片，2为NG片
	int m_count_type{1};//计数类型
};

