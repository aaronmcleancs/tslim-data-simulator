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

    connect(ui->profile_create_button, SIGNAL(released()), this, SLOT (on_createProfileButton_clicked()));

    pump = new Pump(nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createProfileButton_clicked() {
    QString name = ui->new_profile_name_box->toPlainText().trimmed();
    double br = ui->new_profile_br_box->value();
    double cf = ui->new_profile_cf_box->value();
    double cr = ui->new_profile_cr_box->value();
    double tmax = ui->new_profile_min_box->value();
    double tmin = ui->new_profile_min_box->value();

    pump->createProfile(name, br, cr, cf, tmin, tmax);
    ui->profile_list->addItem(name);

}
