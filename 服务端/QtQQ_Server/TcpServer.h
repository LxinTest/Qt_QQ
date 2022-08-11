#pragma once

#include <QTcpServer>

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();

public:
	bool run();//����

signals:
	void signalTcpMsgComes(QByteArray&);

private slots:
	//��������
	void SocketDataProcessing(QByteArray& SendData, int descriptor);
	
	//�Ͽ����Ӵ���
	void SocketDisconnected(int descriptor);

protected:
	//�ͻ������µ�����ʱ
	void incomingConnection(qintptr socketDescriptor);

private:
	int m_port;//�˿ں�
	QList<QTcpSocket*> m_tcpSocketConnectList;
};
