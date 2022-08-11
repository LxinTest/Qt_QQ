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

// ���ã�������Ľ������ƣ�ֻ����qrc��Դ���������
bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//������qrc:/*,html
	if (url.scheme() == QString("qrc"))//�ж�url����
	{
		return true;
	}
	else
	{
		return false;
	}
}

//QWebEngineView��ͼ������ʾ��ҳ����
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
	,m_channel(new QWebChannel(this))
{
	// ����һ����ҳ���� ����Ϊ ��ǰ����ҳ
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	// ͨ��
	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);

	// ע��
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);

	//��ǰ�����������촰�ڵ�ID(QQ��)
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;

	//��ȡ��˾ȺID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE DEPARTMENT_NAME = '%1'")
	.arg(QStringLiteral("��˾Ⱥ")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyID = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID)//��˾Ⱥ
	{
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1");
	}
	else
	{ 
		if (strTalkId.length() == 4)//����Ⱥ��
		{
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));
		}
		else//��������
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

	// ���õ�ǰ��ҳ������ͨ��
	this->page()->setWebChannel(m_channel);

	//��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{}

void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);//����html

	int imageNum = 0;
	int msgType = 1;//��Ϣ����:0�Ǳ��� 1�ı� 2�ļ�
	bool isImageMsg = false;
	QString strData;//���͵�����055  008  155

	for (int i = 0; i < msgLst.size(); i++)
	{
		// ��� msgList��ǰѭ���ģ������ ��һ��Ԫ��
		// �� img �ļ�
		if (msgLst.at(i).at(0) == "img")
		{
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			//��ȡ�������Ƶ�λ��
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;

			//��ȡ��������
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			//���ݱ������Ƶĳ��Ƚ������ñ�������
			//����3λ����3λ,��23������λ023
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

			msgType = 0;//������Ϣ
			imageNum++;

			// �ж�����ߵ� 3���ַ����Ƿ�Ϊ qrc
			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));//ȥ������·����qrc
			}
			else
			{
				pixmap.load(imagePath);
			}

			//����ͼƬhtml��ʽ�ı����  �ı���ϣ�·������ȣ��߶ȣ�
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

	//JSON�����Ǽ�ֵ�Ե��б����м���Ψһ���ַ���,ֵ��QJsonValue��ʾ��
	msgObj.insert("MSG", qsMsg);

	// һ�е�Ŀ�ģ�����Ϊ�� ת��QString �ַ�������
	// ��ת���� Json �ĵ������� Ҫת�� UTF-8 ���ĵ�
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj == "0")//����Ϣ
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));
		
		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images" + strData;
		}

		emit signalSendMsg(strData, msgType);
	}
	else			//����Ϣ
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}

	// Java�ű���������
	/*this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));

	emit signalSendMsg(strData, msgType);*/
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();//�ڵ�Ԫ��
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//���������ӽڵ�

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);


		if (node.isElement())
		{
			//ת��Ԫ��
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
