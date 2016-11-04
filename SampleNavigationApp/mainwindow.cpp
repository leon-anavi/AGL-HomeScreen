#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // no window decoration
    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
