#include "network_server.h"

//-------------------------------------constructor and destructor -------------------------------------//
network_server::network_server(int port,QObject * parent){
    this->port = port;
    this->myNet = new network(); //used to send messages back and forth between itself and the client
    this->bankServer = new QNetworkRequest();
}

network_server::~network_server(){
    delete myNet;
    myNet = NULL;
}
//--------------------------------------------------------------------------------------------------------------

/*
 * @brief network_server::incoming connection
 * Checks if any client is trying to connect to the server
 * @param qintptr which is the client's socketfd
 */
void network_server::incomingConnection(qintptr socketfd){

    QTcpSocket * client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    qDebug()<<"::::::Connection Attempt:::::";
    connect(client,SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client,SIGNAL(disconnectPlayer()), this, SLOT(disconnectPlayer()));
}

/* SLOTS */
/*
 * @brief network_server::readyRead
 * Slot which will be called when a message is received from the client
 */
void network_server::readyRead(){
    QTcpSocket * client = (QTcpSocket*)sender();
    //QVERIFY(player !=NULL);
    QString message = client->readAll();
    qDebug()<<"MESSAGE RECEIVED: ";
    qDebug()<< message;

    //convert to JSON File
    QJsonDocument jDoc = QJsonDocument::fromJson(message.toUtf8());

    //connect to processJSON
   //return the JSON File
    this->jMes.append(message);//use take last
    emit receivedMessage(jDoc);
    processJSON(client, jDoc);

}

/*
 * @brief network_server::disconnected
 * Checks if client is still connected or not
 *
 */
void network_server::disconnected(){
    //check specific alias
    QTcpSocket * player = (QTcpSocket*)sender();
    int count = clients.count();
    clients.remove(player);
    //QVERIFY(count -1 == clients.count());
}
/*
 * @brief network_server::generateHashString
 * generates a hashed string given the parameters
 * converts to hex to allow itself to communicate to the existing
 * PHP infrastructure
*/
QString network_server::generateHashString(QString str){
    return QString(QCryptographicHash::hash((str.toUtf8()), QCryptographicHash::Md5).toHex());
}

/*
 * @brief network_server:: validateCard
 * Reads a card number submitted by a user
 * If the card has a valid bank number
 * A token is created and hashed
 * The card number is stored and a session is created on the server side
 * The server returns a session id that is used to transact the account in action
 *
*/
//--------------------------------------------------------
QString generateTransactionToken();
//--------------------------------------------------------
QString network_server::validateCard(QString crdnr){
    //first you need to identify the bank
    //but for the purpose of this application
    // we will stick to identifying Hcoin cards only

    //hash card number using md5
    QString hashed_card = generateHashString(crdnr);
    qDebug() << "Hashed Card Number " << hashed_card;

    //if bank is Hcoin






}
/*
 * @brief network_server::processJSON
 * Processes messages received from the client
 * @param QTcpSocket which is the client and their message
 */
void network_server::processJSON(QTcpSocket * sender, QJsonDocument jDoc){
    //call the specific function within server
    //QVERIFY(sender != NULL);

    QJsonObject jObj = jDoc.object();


    if (jObj.contains("Code") == false){
        qDebug() << "INVALID MESSAGE SENT";
    }else{
        QString code = jObj["Code"].toString();

        if (code == "010"){

        }else if (code =="001"){
            QString pin = jObj["Plain Text"].toString();
            qDebug() <<"PIN Check: "<< pin;
            //validate pin

            if (pin =="1234"){
                QJsonObject connectRequest{
                    {"Code", "111"},
                    {"Status", "Correct"}
                };
                QJsonDocument message(connectRequest);
                this->myNet->sendMessage(sender, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
            }else{
                QJsonObject connectRequest{
                    {"Code", "111"},
                    {"Status", "Incorrect"}
                };
                QJsonDocument message(connectRequest);
                this->myNet->sendMessage(sender, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
            }

        }else if (code == "000"){


            QString cardnr = jObj["Card Number"].toString();
            qDebug() <<"Card Check: "<< cardnr;

            //connect to bank
            QUrl url("http://localhost/Hcoin%20Financial/");
            bankServer->setUrl(url);
            bankServer->setHeader(bankServer->ContentTypeHeader, "application/x-www-form-urlencoded");
            QNetworkAccessManager * req = new QNetworkAccessManager();
            QNetworkReply * reply = req->post(*bankServer, cardnr.toUtf8());

            connect(reply, &QNetworkReply::readyRead, [=](){
                if (reply->error()==QNetworkReply::NoError){
                    QByteArray response = reply->readAll();
                    QString data = QString(response);
                    qDebug() << data;

                }else{
                    qDebug()<<reply->errorString();
                }
            });

            qDebug() << reply->readAll();

            QJsonObject connectRequest{
                {"Code", "101"},
                {"Bank", "Hcoin"}
            };
            QJsonDocument message(connectRequest);
            this->myNet->sendMessage(sender, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));




        }else if(code == "101"){
            qDebug() <<"Ping Attempt";
            myNet->pong(sender);

        }else if(code == "011"){
            qDebug() <<"Deposit";
            myNet->pong(sender);

        }else if(code == "100"){
            qDebug() <<"Receipt";
            myNet->pong(sender);

        }
    }

}
//---------------------------
void network_server::httpResponse(){
    qDebug()<<"Received HTTP response";
}
