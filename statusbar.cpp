#include "statusbar.h"
#include "ui_statusbar.h"

StatusBar* StatusBar::instance = nullptr;

StatusBar* StatusBar::getInstance() {
    if (!instance) {
        instance = new StatusBar(nullptr);
    }
    return instance;
}

StatusBar::StatusBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StatusBar)
{
    ui->setupUi(this);
    StatusModel* model = StatusModel::getInstance();
    connect(model, &StatusModel::batteryLevelChanged, this, &StatusBar::onBatteryLevelChanged);
    connect(model, &StatusModel::rightTextChanged, this, &StatusBar::onUnitsChanged);

    onBatteryLevelChanged(model->getBatteryLevel());
    onUnitsChanged(model->getRightText());
}

StatusBar::~StatusBar()
{
    delete ui;
}

void StatusBar::onBatteryLevelChanged(int level) {
    // implement battery level update logic here
    ui->batteryStatus->setValue(level);
}

void StatusBar::onUnitsChanged(int level) {
    // implement units text update logic here
    ui->insulinLevel->setValue(level);
}
