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

    ui->batteryToggleButton->setCheckable(true);


    ui->progressBar->setVisible(false);

    StatusModel* model = StatusModel::getInstance();
    connect(model, &StatusModel::batteryLevelChanged, this, &StatusBar::onBatteryLevelChanged);
    connect(model, &StatusModel::rightTextChanged, this, &StatusBar::onUnitsChanged);
    connect(ui->batteryToggleButton, &QPushButton::toggled, [](bool checked) {
        StatusModel* statusModel = StatusModel::getInstance();
        statusModel->setBatteryCharging(checked);
    });

    onBatteryLevelChanged(model->getBatteryLevel());
    onUnitsChanged(model->getRightText());
}

StatusBar::~StatusBar()
{
    delete ui;
}

void StatusBar::onBatteryLevelChanged(int level) {
    ui->batteryStatus->setValue(level);
}

void StatusBar::onUnitsChanged(int level) {

    ui->insulinLevel->setValue(level);

    ui->insulinLevel->setFormat(QString::number(level) + " U");
    setBolus(level);
}

void StatusBar :: setBolus(bool b){
    ui->progressBar->setTextVisible(b);
}
