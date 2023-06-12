#include "gameserver.h"

/*
 * @brief gameServer::gameServer
 * Constructor for gameServer
 * creates an instance of the main window and the QTcpSocket. Also initialize the idCount
 * @param server - main window
 * @param QObject
 */
gameServer::gameServer(MainWindow * server, QObject * parent):QTcpServer(parent){
    serverWindow = server;
    idCount = 0;
}


/*PUBLIC FUNCTIONS*/

/*
 * @brief gameServer::generateID
 * Constructor for gameServer
 * Generates the ID of the JSON message
 */
int gameServer::generateID()
{
    int val = idCount;
    if (this->myIDs.isEmpty() == true)
    {
        this->myIDs.append(val);
    }
    else
    {
        while (this->myIDs.contains(val)) {
            val++;
        }
        this->myIDs.append(val);
    }
    //change this to an pass value
    qDebug() << "New message ID generated";
    idCount = val + 1;

    return val;
}
/* PROTECTED FUNCTIONS */
/*
 * @brief gameServer::incomingConnection
 * Called when a player wants to connect to the server, return a successful connection message
 * Generates the ID of the JSON message
 * @param socketfd
 */
void gameServer::incomingConnection(qintptr socketfd){

    QTcpSocket * user = new QTcpSocket(this);
    user->setSocketDescriptor(socketfd);
    //QVERIFY(clients.count() < 4);
    clients.insert(user);

    serverWindow->addMessage("New Player Inserted: " + user->peerAddress().toString());
    //show test passed connection
    connect(user,SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(user,SIGNAL(disconnected()), this, SLOT(disconnected()));

    int msgID = this->generateID();
    //QVERIFY(msgID == idCount);
    QJsonValue jID(static_cast<int>(msgID));
    QJsonObject connectRequest{
        {"Type", "CONNECT_SUCCESSFUL"},
        {"ID", jID}
    };
    QJsonDocument message(connectRequest);

    this->sendMessage(user, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}
/*
 * @brief gameServer::sendMessage
 * Writes the message over the network with the provided socket
 * @param *receiver - the socket to send the message over
 * @param mes - the message to be sent through
 */
void gameServer::sendMessage(QTcpSocket * receiver, QString mes){
    if (receiver == nullptr)
        return;
    //QVERIFY(receiver != nullptr);

    QJsonDocument jDoc = QJsonDocument::fromJson(mes.toUtf8());
    QJsonObject jObj = jDoc.object();

    receiver->write(mes.toUtf8());
    receiver->flush();
    receiver->waitForBytesWritten();
}

/* SLOTS */
/*
 * @brief gameServer::readyRead
 * Slot which will be called when a message is received from the client
 */
void gameServer::readyRead(){
    QTcpSocket * player = (QTcpSocket*)sender();
    QString message = player->readAll();

    QString feedbackMessage = "Server received: " + message + "\n";
    this->serverWindow->addMessage(feedbackMessage);

}


/*
 * @brief gameServer::disconnected
 * Slot which will be called when a player has left the game
 */
void gameServer::disconnected(){
    QTcpSocket * player = (QTcpSocket*)sender();
    this->serverWindow->addMessage("The following Player is disconnected: " + player->peerAddress().toString());
    int curr = clients.count();
    clients.remove(player);
    //QVERIFY(clients.count() < curr);

}
