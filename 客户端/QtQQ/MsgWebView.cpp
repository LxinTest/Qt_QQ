#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include "TalkWindowShell.h"
#include "WindowManager.h"
#include <QSqlQueryModel>

extern QString gstrLoginHeadPath;

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString msgLPicPath) :QObject(parent)
{
	m_msgLPicPath = msgLPicPath;
	initHtmlTmp();
}

void MsgHtmlObj::initHtmlTmp()
{
	m_msgLHtmlTmpl = getMsgTmpHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);
	m_msgRHtmlTmpl = getMsgTmpHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);
}

QString MsgHtmlObj::getMsgTmpHtml(const QString & code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strDate;
	if (file.isOpen())
	{
		strDate = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");
	}
	file.close();
	return strDate;
}

// 作用：对请求的接收限制，只接收qrc资源里面的类型
bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接收qrc:/*,html
	if (url.scheme() == QString("qrc"))//判断url类型
	{
		return true;
	}
	else
	{
		return false;
	}
}

//QWebEngineView视图用于显示网页内容
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	,m_channel(new QWebChannel(this))
{
	// 构造一个网页，并 设置为 当前的网页
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	// 通道
	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);

	// 注册
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);

	//当前正构建的聊天窗口的ID(QQ号)
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;

	//获取公司群ID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE DEPARTMENT_NAME = '%1'")
	.arg(QStringLiteral("公司群")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyID = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID)//公司群
	{
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1");
	}
	else
	{ 
		if (strTalkId.length() == 4)//其他群聊
		{
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));
		}
		else//单独聊天
		{
			queryEmployeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(strTalkId));
			QModelIndex index = queryEmployeeModel.index(0, 0);
			strPicturePath = queryEmployeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj *msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)
	{
		int rows = queryEmployeeModel.rowCount();
		QModelIndex employeeModelIndex, pictureModelIndex;
		for (int i = 0; i < rows; i++)
		{
			employeeModelIndex = queryEmployeeModel.index(i, 0);
			pictureModelIndex = queryEmployeeModel.index(i, 1);

			strEmployeeID = queryEmployeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmployeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj * msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	// 设置当前网页，网络通道
	this->page()->setWebChannel(m_channel);

	//初始化收信息页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{}

void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);//解析html

	int imageNum = 0;
	int msgType = 1;//信息类型:0是表情 1文本 2文件
	bool isImageMsg = false;
	QString strData;//发送的数据055  008  155

	for (int i = 0; i < msgLst.size(); i++)
	{
		// 如果 msgList当前循环的，链表的 第一个元素
		// 是 img 文件
		if (msgLst.at(i).at(0) == "img")
		{
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			//获取表情名称的位置
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;

			//获取表情名称
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			//根据表情名称的长度进行设置表情数据
			//不足3位则补足3位,如23则数据位023
			int emotionNameL = strEmotionName.length();
			
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if(emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			msgType = 0;//表情信息
			imageNum++;

			// 判断最左边的 3个字符，是否为 qrc
			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));//去掉表情路径中qrc
			}
			else
			{
				pixmap.load(imagePath);
			}

			//表情图片html格式文本组合  文本组合（路径，宽度，高度）
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;
		}
		else if (msgLst.at(i).at(0) == "text")
		{
			qsMsg += msgLst.at(i).at(1);
			strData = qsMsg;
		}
	}

	//JSON对象是键值对的列表，其中键是唯一的字符串,值由QJsonValue表示。
	msgObj.insert("MSG", qsMsg);

	// 一切的目的，都是为了 转成QString 字符串类型
	// 再转换成 Json 文档，并且 要转成 UTF-8 的文档
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj == "0")//发信息
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));
		
		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images" + strData;
		}

		emit signalSendMsg(strData, msgType);
	}
	else			//来信息
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}

	// Java脚本语言运行
	/*this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));

	emit signalSendMsg(strData, msgType);*/
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();//节点元素
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//返回所有子节点

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);


		if (node.isElement())
		{
			//转换元素
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList atrributeList;
				atrributeList << "img" << element.attribute("src");
				attribute << atrributeList;
			}

			if (element.tagName() == "span")
			{
				QStringList atrributeList;
				atrributeList << "text" << element.text();
				attribute << atrributeList;
			}

			if (node.hasChildNodes())
			{
				attribute << parseDocNode(node);

			}
		}
	}

	return attribute;
}
