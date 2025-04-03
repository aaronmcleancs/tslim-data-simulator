#include "bolus.h"
#include "ui_bolus.h"
#include "headers/CGM.h"
#include "headers/BolusCalculator.h"

bolus::bolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bolus)
{
    ui->setupUi(this);
    ui->groupBox->setVisible(false);
        ui->groupBox_2->setVisible(false);
        ui->radioButton->setVisible(false);
        ui->radioButton_2->setVisible(false);
}

bolus::~bolus()
{
    delete ui;
}

void bolus::on_pushButton_clicked()
{
    ui->pushButton_2->setEnabled(true);
       ui->pushButton_3->setEnabled(true);
       ui->groupBox->setVisible(false);
       ui->groupBox_2->setVisible(false);
       ui->radioButton->setVisible(false);
       ui->radioButton_2->setVisible(false);
    emit mainShift();
}


void bolus::on_pushButton_2_clicked()
{
    QPushButton *pb2 = findChild<QPushButton*>("pushButton_3");
       pb2->setEnabled(false);
       ui->groupBox->setVisible(true);
}


void bolus::on_pushButton_3_clicked()
{
    ui->pushButton_2->setDisabled(true);
       ui->groupBox_2->setVisible(true);
       CGM c1;
       double carbs = c1.getCurrentCarbs();
       double glucose = c1.getCurrentGlucoseLevel();
       ui->label_6->setText(QString::number(carbs));
       ui->label_7->setText(QString::number(glucose));
       qDebug()<<"Automatic mode..... ";

}


void bolus::on_pushButton_4_clicked()
{
    BolusCalculator b1;
        int bolus = b1.total_bolus(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
        qDebug()<<"bolus"<<bolus;
        ui->radioButton->setVisible(true);

        ui->radioButton_2->setVisible(true);
}


void bolus::on_pushButton_5_clicked()
{
    ui->radioButton->setVisible(true);
    ui->radioButton_2->setVisible(true);
}

