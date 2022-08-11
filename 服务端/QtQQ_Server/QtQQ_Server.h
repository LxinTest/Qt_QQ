#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"
#include "TcpServer.h"
#include <QSqlQueryModel>
#include <QTimer>
#include <QUdpSocket>

class QtQQ_Server : public QDialog
{
	Q_OBJECT

public:
	QtQQ_Server(QWidget *parent = Q_NULLPTR);

private:
	void initComboBoxData();	//��ʼ����Ͽ�����
	void initTcpSocket();
	void initUdpSocket();
	bool connectMySql();
	void setdepNameMap();
	void setstatusMap();
	void setonlineMap();
	int getComDepID();//��ȡ��˾ȺQQ��
	void updataTableData(int depID = 0, int employeeID = 0);

private slots:
	void onUDPbroadMsg(QByteArray& btData);
	void onRefresh();
	void on_queryDepartmentBtn_clicked();	//����ȺQQ�Ų���Ա��,����ź���ۺ����Զ�����
	void on_queryIDBtn_clicked();			//����Ա��QQ��ɸѡ
	void on_logoutBtn_clicked();			//ע��Ա��QQ��
	void on_selectPictureBtn_clicked();		//ѡ��ͼƬ(Ա���Ĵ���)
	void on_addBtn_clicked();				//����Ա��

private:
	Ui::QtQQ_ServerClass ui;

	int m_depID;						//����QQ��
	int m_employeeID;					//Ա��QQ��
	int m_compDepID;					//��˾ȺQQ��
	QTimer* m_timer;					//��ʱˢ������
	QString m_pixPath;					//ͷ��·��
	QMap<QString, QString> m_statusMap;	//״̬
	QMap<QString, QString> m_depNameMap;//��������
	QMap<QString, QString>m_onlineMap;	//����
	QSqlQueryModel m_queryInfoModel;	//��ѯʹ��Ա������Ϣģ��
	TcpServer *m_tcpServer;
	QUdpSocket* m_udpSender;
};
