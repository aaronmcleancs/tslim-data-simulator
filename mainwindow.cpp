#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Bolus_Groupbox->setVisible(false);
    ui->groupBox_2->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}












