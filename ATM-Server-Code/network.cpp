#include "network.h"

network::network(QObject *parent) : QObject(parent)
{
    idCount = 0;
}

/*
 * @brief Network::generateID
 * Generates a new message ID for the respective client
 */
int network::calMessageID()
{
    int val = idCount;
    if (this->generatedIDs.isEmpty() == true)
    {
        this->generatedIDs.append(val);
    }
    else
    {
        while (this->generatedIDs.contains(val)) {
            val++;
        }
        this->generatedIDs.append(val);
    }

    idCount = val + 1;
    qDebug() << "ID Generated: " << idCount;
    return val;
}

/*
 * @brief Network::sendMessage
 * Writes the message over the network with the provided socket
 * @param *socket - the socket to send the message over
 * @param message - the message to be sent through
 */
void network::sendMessage(QTcpSocket *socket, QString message)
{
    if (socket == nullptr)
        return;

    QJsonDocument jDoc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jObj = jDoc.object();

    Message newMessage;
    newMessage.message = message;
    newMessage.recipient = socket;

    socket->write(message.toUtf8());
    socket->flush();
    socket->waitForBytesWritten();

//    qDebug() << "Client sent: " << message;
    this->messages.append(newMessage);
}

void network:: ping(QTcpSocket * receiver){

    QJsonValue jID(static_cast<int>(calMessageID()));
    QJsonObject connectRequest{
        {"Type", "PING"},
        {"ID", jID}
    };
    QJsonDocument message(connectRequest);
    this->sendMessage(receiver, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}

void network::pong(QTcpSocket * receiver){

    QJsonValue jID(static_cast<int>(calMessageID()));
    QJsonObject connectRequest{
        {"Type", "PONG"},
        {"ID", jID}
    };
    QJsonDocument message(connectRequest);
    this->sendMessage(receiver, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}
