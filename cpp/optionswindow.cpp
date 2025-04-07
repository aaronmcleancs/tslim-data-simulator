#include "headers/optionswindow.h"
#include "ui_optionswindow.h"
#include <QMessageBox>
#include "headers/contentwidget.h"

OptionsWindow::OptionsWindow(Pump* pump, ContentWidget* contentWidget, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsWindow),
    pump(pump),
    contentWidget(contentWidget)
{
    ui->setupUi(this);

    if (pump && pump->getActiveProfile()) {
        double currentBasalRate = pump->getActiveProfile()->getBasalRate();
        ui->basalRate->setValue(currentBasalRate);
    }
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}


void OptionsWindow::on_acceptButton_clicked()
{
    if (pump && pump->getActiveProfile()) {
        double newBasalRate = ui->basalRate->value();
        pump->updateProfile(pump->getActiveProfile()->getName(), "BR", newBasalRate);
        pump->recordBasalRateChange(newBasalRate);
        pump->recordAlert("Basal rate successfully updated to " , newBasalRate);
        contentWidget->updateSettingsTab();
    }
}


void OptionsWindow::on_pushButton_clicked()
{
    emit powerOff();
}

