#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network_server.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class gameServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addMessage(QString mes);
    network_server * myNet;
    Ui::MainWindow * getUI();
    void setIU(Ui::MainWindow *);


    //void newConnection();

private slots:
    void on_btnStart_clicked();


    void on_btnClear_clicked();

    void on_btnClose_clicked();


    void on_btnUpdate_clicked();
    void readyRead(QJsonDocument);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
