#pragma once

#include <QWidget>
#include <QMap>
#include "ui_TalkWindowShell.h"
#include "basicwindow.h"
#include "EmotionWindow.h"
#include <qlistwidget.h>
#include <QTcpSocket>
#include <QUdpSocket>

extern QString gLoginEmployeeID;

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

const int gtcpPort = 8888;
const int gudpPort = 6666;

enum GroupType
{
	COMPANY = 0,	 //公司群
	PERSONELGROUP,	 //人事部
	DEVELOPMENTGROUP,//研发部
	MARKETGROUP,	 //市场部
	PTOP			 //同事之间的聊天
};

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

public:
	//添加新的聊天窗口
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString &uid/*, GroupType grouptype*/);
	
	//设置当前聊天窗口
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap()const;

private:
	void initControl();				//初始化控件
	void initTcpScoket();			//初始化TCP
	void initUdpSocket();			//初始化UDP
	void getEmployeesID(QStringList& employeeList);	//获取所有员工QQ号
;	bool createJSFile(QStringList& employeesList);
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);

public slots:
	void onEmotionBtnClicked(bool);//表情按钮点击后执行的槽函数
	
	//客户端发送Tcp数据(数据,数据类型,文件)
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);	//表情被选中
	void processPendingData();//处理UDP广播收到的数据

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//打开的聊天窗口
	EmotionWindow* m_emotionWindow;

private:
	QTcpSocket *m_tcpClientSocket;	//Tcp客户端
	QUdpSocket *m_udpReceiver;		//udp接收端
};
