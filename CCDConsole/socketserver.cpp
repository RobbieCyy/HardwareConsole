#include "socketserver.h"

SocketServer::SocketServer(QObject *parent) : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection, this, &SocketServer::onNewConnection);
    connect(&server, &QTcpServer::acceptError, this, [=]{
        qDebug() << tr("Server error: %1").arg(server.errorString());
    });
    initServer();
}

SocketServer::~SocketServer()
{
    closeServer();
}

void SocketServer::initServer()
{
    const unsigned short port = 3340;
    server.listen(QHostAddress::Any, port);
    qDebug() << "Server start.";
}

void SocketServer::closeServer()
{
    server.close();
    for (int i=0; i<clientList.length(); i++) {
        clientList[i]->disconnectFromHost();
        if (clientList[i]->state()!=QAbstractSocket::UnconnectedState) {
            clientList[i]->abort();
        }
    }
}

void SocketServer::onNewConnection()
{
    while(server.hasPendingConnections()) {
        QTcpSocket *socket = server.nextPendingConnection();
        clientList.append(socket);
        qDebug() << tr("[%1:%2] Socket Connected")
                    .arg(socket->peerAddress().toString())
                    .arg(socket->peerPort());

        connect(socket, &QIODevice::readyRead, this, [=](){
            this->onReadyRead(socket);
        });

        connect(socket, &QAbstractSocket::errorOccurred, [=](){
            qDebug() << tr("[%1:%2] Socket Error: %3")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort())
                        .arg(socket->errorString());
        });

        connect(socket, &QAbstractSocket::disconnected, this, [=](){
            socket->deleteLater();
            clientList.removeOne(socket);
            qDebug() << tr("[%1:%2] Socket Error: %3")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort())
                        .arg(socket->errorString());
        });
    }
}

void SocketServer::onReadyRead(QTcpSocket *socket)
{
    const QString recv_text = QString::fromUtf8(socket->readAll());
    if (recv_text == "GET /image/data\r\n") {
        emit getImageData();
    }
    if (recv_text == "GET /image/size\r\n") {
        emit getImageSize();
    }
    //qDebug() << tr("[%1:%2]")
    //            .arg(socket->peerAddress().toString())
    //            .arg(socket->peerPort());
    //qDebug() << recv_text;
}

void SocketServer::sendText(QString str)
{
    if (!server.isListening()) {
        return;
    }
    const QByteArray send_data = str.toUtf8();
    if (send_data.isEmpty()) {
        return;
    }
    for (int i=0; i<clientList.length(); i++) {
        clientList[i]->write(send_data);
    }
}

void SocketServer::sendData(QByteArray data)
{
    if (!server.isListening() || data.isEmpty()) {
        return;
    }
    for (int i=0; i<clientList.length(); i++) {
        clientList[i]->write(data);
    }
}
