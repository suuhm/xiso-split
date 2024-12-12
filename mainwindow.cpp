#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QProgressBar>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionNumber();
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &MainWindow::uploadFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QStringList iso = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                "$HOME",
                "Images (*.iso *.xbe *.*)");


    QString is0="";
    //Start bash
    QProcess process;
    QProcess process_deb;
    QString PATHSH="./get-xiso-sh/";
    QString debstr="";

    //connect(&process, SIGNAL(readyReadStandardOutput()), this, MainWindow:);

    for(int x=0 ; x<iso.size();x++)
    {
        //is0 += iso[x]+"\n";
        is0 = iso[x];

        //process_deb.startDetached();
        //setlabel
        ui->label->setText("Convert file: " + PATHSH+"get-xiso.sh "+is0);

        //start bash for iso [x]
        process.start(PATHSH+"get-xiso.sh", QStringList()<< "--file"<<is0);
        //process.startDetached("/usr/bin/bash", QStringList()<< PATHSH+"get-xiso.sh"+" --file "+is0);
        //process.startDetached("/usr/bin/ls");

        debstr ="--- START LOGS (LIST SIZE = " + QString::number(iso.size()) + " )---\n";
        if (process.waitForStarted(-1)) {
            debstr +="\nwaitforstarte=true";
            while(process.waitForReadyRead(-1)) {
                debstr +="\nwaitforread -> "+QString::number(x+1);
                debstr += process.readAll();
                ui->textEdit->setText(debstr);
            }
        }

        ui->progressBar->setValue((x+1)*100/iso.size());
        /*
        while(process.Running)
        {
            //pw*100/G
            ui->progressBar->setValue((x+1)*100/iso.size());
        }
        */
        //terminate process
        process.waitForFinished();
        process.close();
    }
    //ui->label->setText(is0);


}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

/*
void MainWindow::on_pushButton_5_clicked()
{

    //CONNECTION
    QUrl url(ui->lineEdit->text());
    url.setUserName("xbox");
    url.setPassword("xbox");
    url.setPort(21);

    // Start upload
    QNetworkReply *reply = m_manager->get(QNetworkRequest(url));
    // And connect to the progress upload signal

    if(reply->isOpen())
    {
    //connect(reply, &QNetworkReply::uploadProgress, this, &MainWindow::uploadProgress);
    ui->label_2->setStyleSheet("QLabel { background-color : green; color : white; }");
    }
}*/

// FTP-Verbindung prüfen
void MainWindow::on_pushButton_5_clicked()
{
    //QUrl url(ui->lineEdit->text());
    /* CONFIG */
    QUrl url;
    url.setHost("192.168.88.42");
    url.setPath("/C/config.xml");
    //url.se
    //url.setScheme("ftp");
    url.setUserName("xbox");
    url.setPassword("xbox");
    url.setPort(21);

    //QUrl url("ftp://xbox:xbox@192.168.88.42:21/C/config.xml");


    if (url.isValid()) {
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.head(request);
        //ui->textEdit->setText("doop");

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                ui->textEdit->setText("File exists.");
            } else {
                //ui->textEdit->setText(QString("Error: %1").arg(reply->errorString()));
                ui->textEdit->setText("doop");
            }
            reply->deleteLater();
        });
    } else {
        ui->textEdit->setText("Invalid URL.");
    }
}

void MainWindow::processReadyRead()
{
// none
}


void MainWindow::on_pushButton_7_clicked()
{
    //SLECT FILES
    m_fileName = QFileDialog::getOpenFileName(this, "Get Any File");
    ui->label->setText(m_fileName);
}

void MainWindow::on_pushButton_6_clicked()
{
    //UPLOAD FILES
    m_file = new QFile(m_fileName);

    // Next, you need information about the file name
    // The upload path to the server should look like this
    // ftp://example.com/path/to/file/filename.txt
    // That is, we specify the protocol -> ftp
    // Server -> example.com
    // The path where the file will be located -> path/to/file/
    // And the name of the file itself, which we take from QFileInfo -> filename.txt
    QFileInfo fileInfo(*m_file);
    QUrl url(ui->lineEdit_2->text() + fileInfo.fileName());
    url.setHost(ui->lineEdit->text());
    url.setScheme("ftp");
    url.setUserName("xbox");
    url.setPassword("xbox");
    url.setPort(21);
    ui->label->setText(url.toString(QUrl::None));
    if(url.isValid())
    ui->textEdit->setText("validdd");
    else {
        ui->textEdit->setText("not valid");
    }

    //if (m_file->open(QIODevice::ReadOnly))
    if(url.isValid())
    {
        // Start upload
        QNetworkReply *reply = m_manager->put(QNetworkRequest(url), m_file);
        // And connect to the progress upload signal
        connect(reply, &QNetworkReply::uploadProgress, this, &MainWindow::uploadProgress);
        //connect()
    }

}

void MainWindow::uploadFinished(QNetworkReply *reply)
{
    // If the upload was successful without errors
    if (!reply->error())
    {
        m_file->close();
        m_file->deleteLater();  // delete object of file
        reply->deleteLater();   // delete object of reply
    }
}

void MainWindow::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    // Display the progress of the upload
    ui->progressBar->setValue(qint64(100) * bytesSent/bytesTotal);
}
