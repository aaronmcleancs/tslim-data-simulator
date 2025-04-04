#include "bolus.h"
#include "ui_bolus.h"
#include "headers/CGM.h"
#include "headers/BolusCalculator.h"

bolus::bolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bolus)
{
    ui->setupUi(this);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_8->setVisible(false);
    ui->checkBox->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_10->setVisible(false);
    ui->label_11->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_9->setVisible(false);
    ui->InitiateBolus->setVisible(false);

}

bolus::~bolus()
{
    delete ui;
}



void bolus::on_pushButton_4_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()){
        qDebug()<<"Both information required form manual bolus calculations.... ";
        return;
    }
    ui->label_8->setVisible(true);
    ui->label_4->setVisible(true);
    BolusCalculator b1;
    totalBolus = b1.total_bolus(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
    ui->label_4->setText(QString :: number(totalBolus,'g',5));
    ui->checkBox->setVisible(true);

}


void bolus::on_pushButton_3_clicked()
{
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_8->setVisible(false);
    ui->checkBox->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_10->setVisible(false);
    ui->label_11->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_9->setVisible(false);
    ui->InitiateBolus->setVisible(false);
    checked = false;
    emit mainShift();
}





void bolus::on_checkBox_stateChanged(int arg1)
{
    if(!checked){

        ui->label_5->setVisible(true);
        ui->label_6->setVisible(true);
        ui->lineEdit_3->setVisible(true);
        ui->lineEdit_4->setVisible(true);
        ui->pushButton_2->setVisible(true);
        checked = !checked;
    }else{

        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->lineEdit_3->setVisible(false);
        ui->lineEdit_4->setVisible(false);
        ui->pushButton_2->setVisible(false);
        checked = !checked;
    }
}


void bolus::on_pushButton_2_clicked()
{
    double delivery = (totalBolus * ui->lineEdit_3->text().toInt())/100;
    ui->label_7->setVisible(true);
    ui->label_9->setVisible(true);
    ui->label_10->setVisible(true);
    ui->label_11->setVisible(true);
    ui->InitiateBolus->setVisible(true);
    ui->checkBox->setEnabled(false);
    ui->label_9->setText(QString :: number(delivery,'g',5));
    ui->label_11->setText(QString :: number(totalBolus - delivery,'g',5));

}


void bolus::on_checkBox_clicked()
{
}

void bolus::on_pushButton_clicked()
{
}


void bolus::on_InitiateBolus_clicked()
{
    qDebug()<<"Bolus Initiated....";
    emit BolusInitiated();
}

