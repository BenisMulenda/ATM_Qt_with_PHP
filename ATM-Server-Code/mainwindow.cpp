#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myNet = new network_server();

}

/* PUBLIC FUNCTIONS */
/*
 * @brief MainWindow::getUI
 * retrurns the ui pointer
 */
Ui::MainWindow * MainWindow::getUI(){
    return ui;
}

/*
 * @brief gameServer::setUI
 * sets the ui pointer
 */
void MainWindow::setIU(Ui::MainWindow * ui){
    this->ui = ui;
}
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * @brief gameServer::addMessage
 * Updates the feedback edit with the new information being passed in
 * @param message - a string that will be displayed
 */
void MainWindow::addMessage(QString message){
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    ui->edtStatus->append("\n");
    ui->edtStatus->append(message + " ("+ date.toString()+ " "+ time.toString()+")");
}

/* SLOTS */
/*
 * @brief gameServer::button clicked
 * starts up the server
 */
void MainWindow::on_btnStart_clicked()
{

    int port = 5002;

    bool success = myNet->listen(QHostAddress::Any,quint16(port));

    if(success){
        addMessage("Server Successfully started");
        connect(myNet, SIGNAL(receivedMessage(QJsonDocument)), this, SLOT(readyRead(QJsonDocument )), Qt::UniqueConnection);

    }else{
        addMessage("Server has failed to start");
    }
}
void MainWindow::readyRead(QJsonDocument jDoc){
    QJsonObject jObj = jDoc.object();
   addMessage(jObj["Code"].toString());
}
/*
 * @brief gameServer::button clicked
 * cleans up the feedback edit
 */
void MainWindow::on_btnClear_clicked()
{
    ui->edtStatus->setText("");
}


/*
 * @brief gameServer::close connection
 * Closes the server
 *
 */
void MainWindow::on_btnClose_clicked()
{
    delete myNet;
    addMessage("Server has closed");
    myNet = new network_server();
}

/*
 * @brief gameServer::Update
 * Clears the feedback display and adds the current sequence of queries
 *
 */
void MainWindow::on_btnUpdate_clicked()
{
    QList<QString>::iterator i;
    ui->edtStatus->setText("");
    for (i = myNet->jMes.begin(); i != myNet->jMes.end(); ++i)
        addMessage(*i);
}

