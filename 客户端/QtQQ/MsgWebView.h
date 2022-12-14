#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj :public QObject
{
	Q_OBJECT
	//（属性类型，属性名称，类成员，属性的类成员，信号，触发信号）

	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent,QString msgLPicPath = "");//发信息来的人的头像路径

signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmp();//初始化聊天网页

	// 初始化网页信息，不管是对方的，还是我的，都要初始化
	// 传入一个 code ，对左右信息，进行初始化
	QString getMsgTmpHtml(const QString& code);

private:
	QString m_msgLPicPath;
	QString m_msgLHtmlTmpl;//别人发来的信息
	QString m_msgRHtmlTmpl;//我来的信息
};

// 重写网页，接收的网页，只接受 qrc资源里面的类型
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr):QWebEnginePage(parent){}
	
protected:
	// 作用：对请求的接收限制，只接收qrc资源里面的类型
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);
};


//QWebEngineView视图用于显示网页内容
class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	// 追加信息，是 字符串
	void appendMsg(const QString& html, QString strObj = "0");

private:
	//解析html---html(html的字符串)
	QList<QStringList> parseHtml(const QString& html);
	//解析节点
	//Qt中所有DOM节点(属性--说明--文本等)都可以使用QDomNode进行表示
	QList<QStringList> parseDocNode(const QDomNode& node);

signals:
	void signalSendMsg(QString& strData, int &msgType, QString sFile = "");

private:
	// 网页显示的话，需要一个对象
	// QT与html进行通信，需要借助 QWeb
	// 网页对象
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;
};
