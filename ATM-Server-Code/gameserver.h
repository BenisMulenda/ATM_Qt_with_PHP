#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QTest>


class MainWindow;
class gameServer : public QTcpServer
{


    Q_OBJECT
public:

    gameServer(MainWindow * server, QObject * parent = 0);
    MainWindow *serverWindow;
    int generateID();

private slots:
    void readyRead();
    void disconnected();

protected:
    void incomingConnection(qintptr socketfd) Q_DECL_OVERRIDE;
    QList<int> myIDs;
    int idCount;
    void sendMessage(QTcpSocket *, QString);
private:
    QSet<QTcpSocket*> clients;


};

#endif // GAMESERVER_H
