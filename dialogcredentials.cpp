#include "dialogcredentials.h"
#include "ui_dialogcredentials.h"

DialogCredentials::DialogCredentials(QWidget *parent) : QDialog(parent),
    ui(new Ui::DialogCredentials)
{
    ui->setupUi(this);

    credentials = new SpotifyCredentials();
    server = nullptr;

    ui->lineEditClientId->setText(credentials->clientId());
    ui->lineEditClientSecret->setText(credentials->clientSecret());

    if(credentials->redirectUri().isEmpty())
        ui->lineEditRedirectUri->setText(QString("http://localhost:8888"));
    else
        ui->lineEditRedirectUri->setText(credentials->redirectUri());

    QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(ui->pushButtonConfirm, &QAbstractButton::clicked, this, &DialogCredentials::Authenticate);
}

DialogCredentials::~DialogCredentials()
{
    delete ui;
}

void DialogCredentials::Authenticate()
{
    m_clientIdText = ui->lineEditClientId->text().trimmed();
    m_clientSecretText = ui->lineEditClientSecret->text().trimmed();
    m_redirect = ui->lineEditRedirectUri->text().trimmed();

    ui->lineEditClientId->setDisabled(true);
    ui->lineEditClientSecret->setDisabled(true);
    ui->lineEditRedirectUri->setDisabled(true);

    if (server == nullptr)
    {
        server = new QTcpServer(this);
        if (!server->listen(QHostAddress::LocalHost, 8888))
        {
            QMessageBox::warning(this,
                                 "server error",
                                 QString("failed to start a temporary server on port 8888: %1")
                                 .arg(server->errorString()));
            return;
        }
        QTcpServer::connect(server, &QTcpServer::newConnection, [this]()
        {
            // Read
            auto socket = server->nextPendingConnection();
            socket->waitForReadyRead();
            auto response = QString(socket->readAll());

            // Client might want to request favicon or something
            if (!response.contains("?code="))
            {
                socket->close();
                return;
            }

            // Do magic with code received
            // GET /?code=<code> HTTP...
            auto left = response.left(response.indexOf(" HTTP"));
            auto code = left.right(left.length() - left.indexOf("?code=") - 6);
            auto status = credentials->auth(code, m_redirect, m_clientIdText, m_clientSecretText);

            // Write
            socket->write(QString("HTTP/1.1 200 OK\r\n\r\n%1")
                          .arg(status.isEmpty()
                               ? "success, you can now return to spotify-qt"
                               : QString("failed to authenticate: %1").arg(status)).toUtf8());
            socket->flush();
            socket->waitForBytesWritten(3000);
            socket->close();

            // Close it all down if ok
            if (status.isEmpty())
            {
                credentials->save();
                server->close();
                delete server;
                accept();
            }
            else
            {
                // Otherwise re-enable fields
                ui->lineEditClientId->setDisabled(false);
                ui->lineEditClientSecret->setDisabled(false);
                ui->lineEditRedirectUri->setDisabled(false);
            }
        });
    }
    auto url = credentials->authUrl(m_clientIdText, m_redirect);
    QDesktopServices::openUrl(QUrl(url));

}
