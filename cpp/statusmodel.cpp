#include "../headers/statusmodel.h"

StatusModel* StatusModel::instance = nullptr;

StatusModel* StatusModel::getInstance() {
    if (!instance) {
        instance = new StatusModel();
    }
    return instance;
}

StatusModel::StatusModel(QObject *parent)
    : QObject(parent)
    , rightText(0)
{
    // create/initialize battery
    battery = new Battery(this);

    // connect battery's signals to our own signals for forward changes
    connect(battery, &Battery::chargeLevelChanged,
            this, &StatusModel::batteryLevelChanged);
    connect(battery, &Battery::chargingChanged,
            this, &StatusModel::batteryChargingChanged);
}

int StatusModel::getBatteryLevel() const {
    return battery->getChargeLevel();
}

void StatusModel::setBatteryLevel(int level) {
    if (getBatteryLevel() != level) {
        battery->setChargeLevel(level);
    }
}

bool StatusModel::isBatteryCharging() const {
    return battery->isCharging();
}

void StatusModel::setBatteryCharging(bool charging) {
    if (isBatteryCharging() != charging) {
        battery->setCharging(charging);
    }
}

void StatusModel::configureBatteryDrain(int interval, int amount) {
    battery->setDrainInterval(interval);
    battery->setDrainAmount(amount);
}

int StatusModel::getRightText() const {
    return rightText;
}

void StatusModel::setRightText(int level) {
    if (rightText != level) {
        rightText = level;
        emit rightTextChanged(level);
    }
}
