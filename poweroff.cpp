#include "poweroff.h"
#include "ui_poweroff.h"

PowerOff::PowerOff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PowerOff)
{
    ui->setupUi(this);
}

PowerOff::~PowerOff()
{
    delete ui;
}

void PowerOff::on_powerOnButton_clicked()
{
    emit powerOn();
}

