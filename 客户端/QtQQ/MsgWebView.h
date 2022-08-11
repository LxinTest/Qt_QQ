#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject
{
	Q_OBJECT
	//���������ͣ��������ƣ����Ա�����Ե����Ա���źţ������źţ�

	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent,QString msgLPicPath = "");//����Ϣ�����˵�ͷ��·��

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmp();//��ʼ��������ҳ

	// ��ʼ����ҳ��Ϣ�������ǶԷ��ģ������ҵģ���Ҫ��ʼ��
	// ����һ�� code ����������Ϣ�����г�ʼ��
	QString getMsgTmpHtml(const QString& code);

private:
	QString m_msgLPicPath;
	QString m_msgLHtmlTmpl;//���˷�������Ϣ
	QString m_msgRHtmlTmpl;//��������Ϣ
};

// ��д��ҳ�����յ���ҳ��ֻ���� qrc��Դ���������
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr):QWebEnginePage(parent){}
	
protected:
	// ���ã�������Ľ������ƣ�ֻ����qrc��Դ���������
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);
};


//QWebEngineView��ͼ������ʾ��ҳ����
class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	// ׷����Ϣ���� �ַ���
	void appendMsg(const QString& html, QString strObj = "0");

private:
	//����html---html(html���ַ���)
	QList<QStringList> parseHtml(const QString& html);
	//�����ڵ�
	//Qt������DOM�ڵ�(����--˵��--�ı���)������ʹ��QDomNode���б�ʾ
	QList<QStringList> parseDocNode(const QDomNode& node);

signals:
	void signalSendMsg(QString& strData, int &msgType, QString sFile = "");

private:
	// ��ҳ��ʾ�Ļ�����Ҫһ������
	// QT��html����ͨ�ţ���Ҫ���� QWeb
	// ��ҳ����
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;
};
