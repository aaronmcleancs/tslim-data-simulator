#include "headers/BolusCalculator.h"
#include "headers/Profile.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "mainwindow.h"


int BolusCalculator :: Carb_Bolus_Calculation(int total_carbs){
     return static_cast<int>(total_carbs / activeProfile->getCarbRatio());
}

int BolusCalculator :: Correction_Bolus_Calculation(int current_glucose){
    QPair<double, double> range = activeProfile->getTargetGlucoseRange();
        double target = (range.first + range.second) / 2.0;
        return static_cast<int>((current_glucose - target) / activeProfile->getCorrectionFactor());
}

void BolusCalculator::total_bolus(int total_carbs, int current_glucose) {
    int carbBolus = Carb_Bolus_Calculation(total_carbs);
    int correctionBolus = Correction_Bolus_Calculation(current_glucose);
    int totalBolus = carbBolus + correctionBolus;
    bolus = totalBolus;
    QGroupBox *gb1 = findChild<QGroupBox*>("Bolus_Groupbox");
    QGroupBox *gb2 = findChild<QGroupBox*>("groupBox_2");

    gb1->setVisible(false);
    gb2->setVisible(true);

    QLabel *l1 = findChild<QLabel*>("label_5");
    QLabel *l2 = findChild<QLabel*>("label_6");
    QLabel *l3 = findChild<QLabel*>("label_7");

    l1->setText("Current Carbs : " + QString::number(total_carbs));
    l2->setText("Current Glucose : " + QString::number(total_carbs));
    l3->setText("Total Bolus : " + QString :: number(totalBolus));

}


void MainWindow::on_bolusButton_clicked()
{
    QGroupBox *gb1 = findChild<QGroupBox*>("Bolus_Groupbox");
    gb1->setVisible(true);
    gb1->setEnabled(true);

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
    int glucose = c1.getCurrentGlucose();

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
    QGroupBox *gb2 = findChild<QGroupBox*>("groupBox_2");
    gb2->setVisible(false);
}






