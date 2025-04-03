#include "bolus.h"
#include "ui_bolus.h"
#include "headers/CGM.h"
#include "headers/BolusCalculator.h"

bolus::bolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bolus)
{
    ui->setupUi(this);

}

bolus::~bolus()
{
    delete ui;
}


