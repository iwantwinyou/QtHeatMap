#include "UiAlarm.h"
#include <QDebug>

#pragma execution_character_set("utf-8")
//////////////////////////////////////////////////////////////////////////
namespace LpAlarm {
	const QString T_TITLE = "title";	// 标题
	const QString T_TYPE = "type";	// 类型
	const QString T_INFO = "info";	// 内容
	const QString T_CODE = "code";  // 代码
	const QString T_SUGGEST = "suggest";// 操作建议

	inline QMap<QString, QString> mpObjs() {
		static QMap<QString, QString> mp = {
			{T_TITLE,		QObject::tr("标题")},
			{T_TYPE,		QObject::tr("类型")},
			{T_INFO,		QObject::tr("内容")},
			{T_CODE,		QObject::tr("代码")},
			{T_SUGGEST,		QObject::tr("操作建议")}
		};
		return mp;
	};
	inline QStringList type_name() {
		static QStringList ls = { QObject::tr("记录"),QObject::tr("提醒"),QObject::tr("警告"),QObject::tr("错误") };
		return ls;
	};

	

	const int n_close = 180;// 默认180秒(3分钟)关闭对话框
}

//////////////////////////////////////////////////////////////////////////
UiAlarm::UiAlarm(QDialog* parent /*= nullptr*/)
	:QDialog(parent)
{
	init();
}

UiAlarm::~UiAlarm()
{
	if (m_pTimer) {
		m_pTimer->stop();
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void UiAlarm::f_show(QString info, int type, QVariant vparam/*=QVariant()*/)
{
	if (type < 0) {
		if (vparam.toMap().value("des").toString() == m_cur_des) {
			if (m_cur_type > 2)
				accept();
			else
				m_close_cnt = 0;
		}
		return;
	}
	if (type < 2)
		return;//0,1,2,3 只有报警等级2以上的才弹窗
	if (type > 3)
		type = 3;
	auto& vmp = vparam.toMap();
	QString des = vmp.value("des").toString();
	if (m_ignore_keys_.contains(des))
		return;
	if (type < m_cur_type)
		return;// 小于当前报警等级的报警忽略
	m_cur_type = type;
	m_cur_des = des;

	QString suggest = vmp.value(LpAlarm::T_SUGGEST).toString();
	pf_setItemValue(LpAlarm::T_TITLE, des);
	pf_setItemValue(LpAlarm::T_SUGGEST, suggest);
	pf_setItemValue(LpAlarm::T_INFO, info);
	pf_setItemValue(LpAlarm::T_CODE, "0x"+QString("%1").arg(vmp.value(LpAlarm::T_CODE).toInt(), 8, 16, QChar('0')).toUpper());
	pf_setItemValue(LpAlarm::T_TYPE, LpAlarm::type_name()[type]);

	if (m_lb_ico)
		m_lb_ico->setPixmap(m_type_ico.at(type));

	m_lb_tm->setText("");
	if (type == 2) {
		m_close_cnt = LpAlarm::n_close;
		m_pTimer->start(1000);
	}
	else 
		m_pTimer->stop();

	if(this->isHidden())
		exec();
}

void UiAlarm::f_reset()
{
	m_ignore_keys_.clear();
}

void UiAlarm::init()
{
	this->setWindowFlags(Qt::WindowTitleHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
	this->setStyleSheet("UiAlarm{border: 4px solid red;}  ");//QTextEdit{color: red}
	this->resize(500, 250);
	this->setWindowTitle(tr("利珀科技温馨提醒"));

	//  界面
	QHBoxLayout* hl = new QHBoxLayout;
	QPushButton *pOK = new QPushButton(tr("确定"));
	QPushButton *pTip = new QPushButton(tr("不再提醒"));
	connect(pOK, SIGNAL(clicked()), this, SLOT(on_btn_ok()));
	connect(pTip, SIGNAL(clicked()), this, SLOT(on_btn_tips()));
	pOK->setDefault(true);
	hl->addWidget(m_lb_tm = new QLabel());
	hl->addStretch();
	hl->addWidget(pTip);
	hl->addWidget(pOK);
	m_lb_tm->setStyleSheet("color: #707070");

	QGridLayout* lay = new QGridLayout();
	pf_createItem(LpAlarm::T_TITLE, lay);
	pf_createItem(LpAlarm::T_TYPE, lay);
	pf_createItem(LpAlarm::T_INFO, lay);
	pf_createItem(LpAlarm::T_CODE, lay);
	pf_createItem(LpAlarm::T_SUGGEST, lay);

	QVBoxLayout* vl = new QVBoxLayout(this);
	vl->addLayout(lay);
	vl->addLayout(hl);

	// 图标
	m_type_ico << QPixmap();
	pf_addIco(":/SInspect/Resources/t_info.ico");
	pf_addIco(":/SInspect/Resources/t_warn.ico");
	pf_addIco(":/SInspect/Resources/t_error.ico");

	// 定时器
	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_timer()));
}

void UiAlarm::pf_createItem(const QString& key, QGridLayout* lay)
{
	QLabel *lb = new QLabel(LpAlarm::mpObjs().value(key));
	lb->setFixedHeight(22);
	QTextEdit *te = new QTextEdit();
	te->setLineWrapMode(QTextEdit::NoWrap);
	te->setReadOnly(true);
	te->setObjectName(key);
	
	int row = lay->rowCount();
	// 特殊处理1
	if (key == LpAlarm::T_TYPE) {
		m_lb_ico = new QLabel();
		QHBoxLayout* hl = new QHBoxLayout;
		hl->addWidget(lb);
		hl->addWidget(m_lb_ico);
		lay->addLayout(hl, row, 0);
	}
	else
		lay->addWidget(lb, row, 0);
	
	// 特殊处理2
	if (key == LpAlarm::T_SUGGEST) {
		lay->setRowStretch(row + 1, 0);
		lay->addWidget(te, row, 1, 2, 1);
	}
	else {
		te->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		te->setFixedHeight(22);
		lay->addWidget(te, row, 1);
	}
}

void UiAlarm::pf_setItemValue(const QString& key, const QString& val)
{
	auto* pbtn = this->findChild<QTextEdit*>(key);
	if (!pbtn) return;

	pbtn->setText(val);
}

void UiAlarm::pf_addIco(const QString& resource)
{
	QIcon ico(resource);
	QPixmap pic = ico.pixmap(ico.actualSize(QSize(16, 16)));
	m_type_ico << pic;
}

void UiAlarm::on_btn_tips()
{
	auto* pbtn = this->findChild<QTextEdit*>(LpAlarm::T_TITLE);
	if (!pbtn) return;
	QString des = pbtn->toPlainText();
	if (!m_ignore_keys_.contains(des))
		m_ignore_keys_ << des;
	m_cur_type = 0;
	m_pTimer->stop();
	accept();
}

void UiAlarm::on_btn_ok()
{
	m_cur_type = 0;
	m_pTimer->stop();
	accept();
}

void UiAlarm::on_timer()
{
	m_close_cnt--;
	if (m_close_cnt <= 0) {
		m_cur_type = 0;
		m_pTimer->stop();
		accept();
		return;
	}
	QString str = QString(tr("         %1 秒后自动关闭对话框.")).arg(m_close_cnt);
	m_lb_tm->setText(str);
}
