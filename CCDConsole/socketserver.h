#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();

public slots:
    void initServer();
    void onNewConnection();
    void closeServer();
    void onReadyRead(QTcpSocket *socket);
    void sendText(QString str);
    void sendData(QByteArray data);

signals:
    void getImageData();
    void getImageSize();

private:
    QTcpServer server;
    QList<QTcpSocket*> clientList;
};

#endif // SOCKETSERVER_H
