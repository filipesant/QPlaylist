#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playercontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PlayerController *controller = new PlayerController(this);
    setCentralWidget(controller->layout());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CredentialsDialog()
{

}

