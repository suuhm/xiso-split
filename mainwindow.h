#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/QNetworkAccessManager>
#include <string>
#include <QString>
#include <QFile>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void processReadyRead();

    void uploadFinished(QNetworkReply *reply);  // Upload finish slot
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);  // Upload progress slot

private:
    Ui::MainWindow *ui;
    QString m_fileName;
    QFile *m_file;
    QNetworkAccessManager *m_manager;
};

#endif // MAINWINDOW_H
