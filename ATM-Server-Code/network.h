#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>


struct Message{
    QTcpSocket * recipient;
    QString message;
};

class network: public QObject
{
    Q_OBJECT
public:
    explicit network(QObject * parent = nullptr);//pure virtual
    void ping(QTcpSocket * );
    void pong(QTcpSocket * );
    //~network();

public:
    //member functionns
    int calMessageID();
    void sendMessage(QTcpSocket*, QString);

 private:
    //member variables
    int idCount;
    QList<Message> messages;
    QList<int> generatedIDs;


};

#endif // NETWORK_H
