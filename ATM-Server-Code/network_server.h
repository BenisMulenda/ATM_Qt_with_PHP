#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "network.h"
#include <QTest>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>

class network_server: public QTcpServer
{
    Q_OBJECT
public:

    network_server(int port = 5002,QObject * parent = nullptr);
    ~network_server();
    void processJSON(QTcpSocket * sender, QJsonDocument jDoc);
    bool validatePassword(QString psw);//checks the password matches the account
    QString validateCard(QString crdnr);//checks which bank and stores the card number
    QString generateHashString(QString str);
    QString generateTransactionToken();


    network * myNet;
    QList<QString> jMes;


private slots:
    void readyRead();

    void disconnected();
signals:
    void receivedMessage(QJsonDocument jDoc );
private:
    QSet<QTcpSocket*> clients;
    QSet<QString> clientNames;
     void httpResponse();
protected:
    void incomingConnection(qintptr socketfd) Q_DECL_OVERRIDE;
private:
    int port = 5002;
    QNetworkRequest * bankServer;
    QString bankUrls [2] = {"http://localhost/Hcoin%20Financial/", "123bank.com"};

};

/*struct bankReq{
    QString token;
    QString bankUrl;
    QString pin;
};*/

#endif // NETWORK_SERVER_H
