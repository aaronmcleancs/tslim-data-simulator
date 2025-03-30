#include "headers/Battery.h"

Battery::Battery(QObject *parent) : QObject(parent), chargeLevel(100) {}

int Battery::getChargeLevel() const {
    return chargeLevel;
}

void Battery::setChargeLevel(int level) {
    if (chargeLevel != level) {
        chargeLevel = level;
        emit chargeLevelChanged(chargeLevel);
    }
}
