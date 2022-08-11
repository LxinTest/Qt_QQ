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
	void onReceiveData();		//处理readyRead信号读取的数据
	void onClientDisconnect();	//处理客户端断开连接

signals:
	//从客户端收到数据后发射信号告诉server有数据要处理
	void signalGetDataFromClient(QByteArray&, int);
	//告诉server有客户端断开连接
	void signalClientDisconnect(int);

private:
	int m_descriptor;//描述符,用于唯一标识
};
