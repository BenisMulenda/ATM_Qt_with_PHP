#ifndef ATM_CLIENT_H
#define ATM_CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDialog>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QSerialPort>
//add to cmake


QT_BEGIN_NAMESPACE
namespace Ui { class atm_client; }
QT_END_NAMESPACE

class atm_client : public QMainWindow
{
    Q_OBJECT

public:
    atm_client(QWidget *parent = nullptr);
    ~atm_client();
    /* feedback */
    void displayFeedback(QString);


    /*connection functions*/
    void connectToHost();

    /*security function*/
    QString md5Hash(QString);

    /*network connection test*/
    QString pingHost(QTcpSocket *);

    /*network request*/
    void sendRequest(QTcpSocket*, QString);


    /* Display functionality */
    void cToggleVisible(bool);
    void pToggleVisible(bool);
    void aToggleVisible(bool);

    /* Functionality */
    void makeDeposit();
    void withdraw();
    void inquiry();

    /*Clear all edits*/
    void clearAllEdits();

public slots:
    void hostResponse();

private slots:
    void on_btnSubmitCard_clicked();

    void on_btnEnter_clicked();

private:
    Ui::atm_client *ui;
    /*port where commnication is gonna occur*/
    QString port;

    /*identifier of the ATM*/
    QTcpSocket * atmSocket;
};
#endif // ATM_CLIENT_H
