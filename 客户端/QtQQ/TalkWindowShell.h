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
	COMPANY = 0,	 //��˾Ⱥ
	PERSONELGROUP,	 //���²�
	DEVELOPMENTGROUP,//�з���
	MARKETGROUP,	 //�г���
	PTOP			 //ͬ��֮�������
};

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

public:
	//����µ����촰��
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString &uid/*, GroupType grouptype*/);
	
	//���õ�ǰ���촰��
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap()const;

private:
	void initControl();				//��ʼ���ؼ�
	void initTcpScoket();			//��ʼ��TCP
	void initUdpSocket();			//��ʼ��UDP
	void getEmployeesID(QStringList& employeeList);	//��ȡ����Ա��QQ��
;	bool createJSFile(QStringList& employeesList);
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);

public slots:
	void onEmotionBtnClicked(bool);//���鰴ť�����ִ�еĲۺ���
	
	//�ͻ��˷���Tcp����(����,��������,�ļ�)
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);	//���鱻ѡ��
	void processPendingData();//����UDP�㲥�յ�������

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//�򿪵����촰��
	EmotionWindow* m_emotionWindow;

private:
	QTcpSocket *m_tcpClientSocket;	//Tcp�ͻ���
	QUdpSocket *m_udpReceiver;		//udp���ն�
};
