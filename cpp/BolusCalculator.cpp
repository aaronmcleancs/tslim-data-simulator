#include "headers/BolusCalculator.h"
#include "headers/Profile.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "mainwindow.h"
#include "headers/Pump.h"


void BolusCalculator :: setCurrentProfile(Profile* p){
    activeProfile  = p;;
}


double BolusCalculator :: Carb_Bolus_Calculation(int total_carbs){
    if(activeProfile == nullptr){
        qDebug()<<"Current profile is not set";
    }
    return total_carbs/activeProfile->getCarbRatio();
//    return 5.6;
}

double
BolusCalculator :: Correction_Bolus_Calculation(int current_glucose){

    if(activeProfile == nullptr){
        qDebug()<<"Current profile is not set";
    }
    qDebug()<<activeProfile->getCarbRatio();;
    qDebug()<<activeProfile->getCorrectionFactor();
    qDebug()<<activeProfile->getTargetGlucoseRange();
    double target = (activeProfile->getTargetGlucoseRange().first + activeProfile->getTargetGlucoseRange().second) / 2.0;
    return (current_glucose - target) / activeProfile->getCorrectionFactor();

//      return 6.4;

}
double BolusCalculator::total_bolus(int total_carbs, int current_glucose,Profile* p, InsulinCartridge* c) {
    activeProfile = p;
    cartridge = c;
    int carbBolus = Carb_Bolus_Calculation(total_carbs);
    int correctionBolus = Correction_Bolus_Calculation(current_glucose);
    double totalBolus = carbBolus + correctionBolus;
    bolus = totalBolus - 1.75;

    return bolus;
}


//void MainWindow::on_bolusButton_clicked()
//{
//    emit bolusShift();
//    qDebug()<<"shifting to bolus calculator.... ";

//}

//void MainWindow::on_radioButton_clicked()
//{


//}
//void MainWindow::on_radioButton_2_clicked()
//{
//}


//void MainWindow::on_calculate_clicked()
//{


//}

//void MainWindow::on_pushButton_clicked()
//{
//    if(bolusState){
//        qDebug()<<"Bolus Stopped....";
//        emit bolusStopped();
//    }else{
//        qDebug()<<"Bolus is not initiated yet...";
//    }
//}
