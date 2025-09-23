#pragma once
#include <qdialog.h>
#include <QtCore>
#include <QtWidgets>
#include "lp_singleton_base.h"
// 报警对话框
// 展示内容包括：标题、报警内容、操作建议等
// vparam: des-标题 suggest-操作建议
//////////////////////////////////////////////////////////////////////////


class UiAlarm :	public QDialog, public lp_singleton_base<UiAlarm>
{
	Q_OBJECT
public:
	UiAlarm(QDialog* parent = nullptr);
	virtual ~UiAlarm();

	Q_SLOT void f_show(QString info, int type, QVariant vparam=QVariant());
	void f_reset();// 清除不再提醒的项
	
protected:
	void init();
	void pf_createItem(const QString& key, QGridLayout* lay);// 创建一项
	void pf_setItemValue(const QString& key, const QString& val);
	void pf_addIco(const QString& resource);

	Q_SLOT void on_btn_tips();// 不再提醒按钮
	Q_SLOT void on_btn_ok();// 确定按钮
	Q_SLOT void on_timer();

private:
	QStringList m_ignore_keys_;// 不再提醒的key
	QLabel* m_lb_ico{ nullptr };
	QLabel* m_lb_tm{ nullptr };
	QList<QPixmap> m_type_ico;
	QTimer* m_pTimer{ nullptr };//定时关闭窗口
	int m_close_cnt{ 0 };

	int m_cur_type{ 0 };// 当前的展示等级
	QString m_cur_des;  // 当前的标题(用于指定取消)
};

