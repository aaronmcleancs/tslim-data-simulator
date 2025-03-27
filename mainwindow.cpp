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
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_bolusButton_clicked()
{
    QGroupBox groupbox("Bolus_Groupbox");
    groupbox.setEnabled(true);
    QRadioButton radioButton("radioButton");
    QRadioButton radioButton2("radioButton_2");
    BolusCalculator b1;
    if(radioButton.isChecked()==true){
        QLabel label1("label3");
        label1.setEnabled(true);
        QLabel label2("label4");
        label2.setEnabled(true);
        QLineEdit line("Glucose_input");
        line.setEnabled(true);
        QLineEdit line2("Carbs_input");
        line2.setEnabled(true);
        QPushButton submit("calculate");
        submit.setEnabled(true);
        bool ok;
        int glucose = 0;
        glucose = line.text().toInt(&ok);

        int carbs = 0;
        carbs = line2.text().toInt(&ok);

        int total_bolus = b1.total_bolus(carbs, glucose);


    }

    else if (radioButton2.isChecked()==true){
        CGM g1;
        int glucose = g1.getCurrentGlucose();
        int carbs = g1.getCurrentCarbs();
        int total_bolus = b1.total_bolus(carbs, glucose);


    }
}


void MainWindow::on_calculate_clicked()
{
    QGroupBox groupbox("Bolus_Groupbox");
    groupbox.setEnabled(false);
}

