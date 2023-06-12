#include "atm_client.h"
#include "ui_atm_client.h"
#include <QThread>

atm_client::atm_client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::atm_client)
{
    ui->setupUi(this);

    //Create ATM Socket
    atmSocket = new QTcpSocket(this);

    cToggleVisible(false);
    pToggleVisible(false);
    aToggleVisible(false);

    //connection to host
    displayFeedback("Attempting to connect...");

    QThread::sleep(2);
    port ="5002";
    qDebug()<< "PORT: " <<port;
    connectToHost();


}

atm_client::~atm_client()
{
    delete ui;
}

//-----------------------------------------Network Communication Functions ---------------------------------
void atm_client::connectToHost(){
    atmSocket->connectToHost("localhost", quint16(port.toInt()));

    if(atmSocket->waitForConnected()){

        displayFeedback("Connected");

        cToggleVisible(true);

        connect(atmSocket, SIGNAL(readyRead()), this, SLOT(hostResponse()), Qt::UniqueConnection);

    }else{
        displayFeedback("ERROR: COULDN'T CONNECT TO PORT " + port);

        //TEST functionality
        cToggleVisible(true);

    }
}

QString atm_client::pingHost(QTcpSocket * target){

    QJsonObject connectRequest{
        {"Code", "101"},
        {"Location", "Sunnyridge"}
    };
    QJsonDocument message(connectRequest);
    this->sendRequest(target, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}

void atm_client::sendRequest(QTcpSocket* target, QString mes){
    if (target == nullptr)
        return;

    QJsonDocument jDoc = QJsonDocument::fromJson(mes.toUtf8());
    QJsonObject jObj = jDoc.object();

    target->write(mes.toUtf8());
    target->flush();
    target->waitForBytesWritten();
}

void atm_client::hostResponse(){
 qDebug() << "RECEIVED A NEW MESSAGE";
 QString mes = atmSocket->readAll();

 //processMessage
 QJsonDocument jDoc = QJsonDocument::fromJson(mes.toUtf8());

 QJsonObject jObj = jDoc.object();

 QString code = jObj["Code"].toString();

if (code == "101"){//

    displayFeedback(jObj["Bank"].toString());
    ui->btnSubmitCard->setEnabled(false);
    ui->ledtCard->setEnabled(false);
    pToggleVisible(true);
}else if(code =="111"){
    if (jObj["Status"].toString()=="Incorrect"){
        displayFeedback("Reenter the password");
    }else{
        ui->ledtPin->setEnabled(false);
        ui->btnEnter->setEnabled(false);
        displayFeedback("Remote Access Established");
        aToggleVisible(true);
    }


}


}
//--------------------------------- SECURITY ----------------------------------------
/*Security*/
QString atm_client::md5Hash(QString dt){

}


//-----------------------------------VISUAL FEEDBACK --------------------------------------
/* Feedback */
void atm_client::displayFeedback(QString out){
    ui->lblConnect->setText(out);
}
/*Card Details */
void atm_client::cToggleVisible(bool v){
    ui->lblCard->setVisible(v);
    ui->ledtCard->setVisible(v);
    ui->btnCancel->setVisible(v);
    ui->btnSubmitCard->setVisible(v);
}
/*Pin Details */
void atm_client::pToggleVisible(bool v){
    ui->lblPin->setVisible(v);
    ui->ledtPin->setVisible(v);
    ui->btnEnter->setVisible(v);
}

/*Show Account details*/
void atm_client::aToggleVisible(bool v){
    /* ACCOUNT */
    ui->edtFeedback->setVisible(v);
    ui->btnDeposit->setVisible(v);
    ui->btnWithdraw->setVisible(v);
    ui->btnInquiry->setVisible(v);
    ui->spedtAmount->setVisible(v);
}
//------------------------------------ATM functionality----------------------------------

void atm_client::makeDeposit(){

}

void atm_client::withdraw(){

}

void atm_client::inquiry(){

}

/*Clear all edits*/
void atm_client::clearAllEdits(){

}

//----------------------------------Interactions with the UI-----------------------------------------------

void atm_client::on_btnSubmitCard_clicked()
{
    qDebug()<<"Inside Check Card Number";
    QString crdNr = ui->ledtCard->text();
    qDebug()<<"Card Number: "<<crdNr;

    if(crdNr == ""){
        qDebug()<<"The String is Empty";
    }else{
        QJsonObject connectRequest{
            {"Code", "000"},
            {"Card Number", crdNr}
        };
        QJsonDocument message(connectRequest);
        this->sendRequest(atmSocket, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
    }
    //Send Card number for a validation with 000 code
    //In case of valid card entry





}


void atm_client::on_btnEnter_clicked()
{
    qDebug()<<"Inside Check PIN";
    QString pin = ui->ledtPin->text();
    qDebug()<<"PIN Number: "<<pin;

    if(pin == ""){
        qDebug()<<"The String is Empty";
    }else{
        QJsonObject connectRequest{
            {"Code", "001"},
            {"Plain Text", pin}
        };
        QJsonDocument message(connectRequest);
        this->sendRequest(atmSocket, QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
    }
}

