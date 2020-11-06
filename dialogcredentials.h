#ifndef DIALOGCREDENTIALS_H
#define DIALOGCREDENTIALS_H

//#include "auth.h"
#include <QDialog>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTcpServer>
#include <QUrl>
#include <QFileInfo>
#include <QMutex>
#include <QDir>

#include "spotifycredentials.h"


namespace Ui {
class DialogCredentials;
}

class DialogCredentials : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCredentials(QWidget *parent = nullptr);
    ~DialogCredentials() override;

private slots:
    void Authenticate();

private:
    Ui::DialogCredentials *ui;
    SpotifyCredentials *credentials;
    QTcpServer *server;
    QString m_clientIdText, m_clientSecretText, m_redirect, m_accessToken, m_refreshToken;

};

#endif // DIALOGCREDENTIALS_H
