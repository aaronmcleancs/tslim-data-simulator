#include "headers/BolusCalculator.h"
#include "headers/Profile.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "mainwindow.h"
#include "headers/Pump.h"


double BolusCalculator :: Carb_Bolus_Calculation(int total_carbs){
    Pump p1;
    Profile* profile = p1.getActiveProfile();
    return total_carbs/profile->getCarbRatio();
}

double
BolusCalculator :: Correction_Bolus_Calculation(int current_glucose){
    Pump p1;
    Profile* profile = p1.getActiveProfile();
    double target = (profile->getTargetGlucoseRange().first + profile->getTargetGlucoseRange().second) / 2.0;
    return (current_glucose - target) / profile->getCorrectionFactor();
}

double BolusCalculator::total_bolus(int total_carbs, int current_glucose) {
    int carbBolus = Carb_Bolus_Calculation(total_carbs);
    int correctionBolus = Correction_Bolus_Calculation(current_glucose);
    double totalBolus = carbBolus + correctionBolus;
    bolus = totalBolus;

    return totalBolus;
}


void MainWindow::on_bolusButton_clicked()
{
    emit bolusShift();
    qDebug()<<"shifting to bolus calculator.... ";

}

void MainWindow::on_radioButton_clicked()
{
    QRadioButton *rb2 = findChild<QRadioButton*>("radioButton_2");
    rb2->setEnabled(false);

    QLabel *label = findChild<QLabel*>("label_3");
    QLabel *label2 = findChild<QLabel*>("label_4");
    QLineEdit *l1 = findChild<QLineEdit*>("Glucose_input");
    QLineEdit *l2 = findChild<QLineEdit*>("Carbs_input");
    QPushButton *p1 = findChild<QPushButton*>("calculate");

    label->setEnabled(true);
    label2->setEnabled(true);
    l1->setEnabled(true);
    l2->setEnabled(true);
    p1->setEnabled(true);

}
void MainWindow::on_radioButton_2_clicked()
{
    QRadioButton *rb1 = findChild<QRadioButton*>("radioButton");
    rb1->setEnabled(false);

    CGM c1;
    int carbs = c1.getCurrentCarbs();
    int glucose = c1.getCurrentGlucoseLevel();

    BolusCalculator b1;
    b1.total_bolus(carbs, glucose);
}


void MainWindow::on_calculate_clicked()
{
    QLineEdit *l1 = findChild<QLineEdit*>("Glucose_input");
    QLineEdit *l2 = findChild<QLineEdit*>("Carbs_input");
    BolusCalculator b1;
    b1.total_bolus(l2->text().toInt(),l2->text().toInt());

}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"Bolus delivery stopped";
}






