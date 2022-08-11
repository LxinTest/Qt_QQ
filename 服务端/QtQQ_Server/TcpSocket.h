#pragma once

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();
	void run();

private slots:
	void onReceiveData();		//����readyRead�źŶ�ȡ������
	void onClientDisconnect();	//����ͻ��˶Ͽ�����

signals:
	//�ӿͻ����յ����ݺ����źŸ���server������Ҫ����
	void signalGetDataFromClient(QByteArray&, int);
	//����server�пͻ��˶Ͽ�����
	void signalClientDisconnect(int);

private:
	int m_descriptor;//������,����Ψһ��ʶ
};
