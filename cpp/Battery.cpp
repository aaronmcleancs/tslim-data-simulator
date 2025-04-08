#include "headers/Battery.h"

Battery::Battery(QObject *parent)
    : QObject(parent)
    , chargeLevel(100)    // full charge
    , charging(false)     // start with draining state
    , drainInterval(10000) // drain every 10 seconds by default
    , drainAmount(1)      // drain by 1
{
    // create and set up the drain timer
    drainTimer = new QTimer(this);
    drainTimer->setInterval(drainInterval);
    connect(drainTimer, &QTimer::timeout, this, &Battery::drainBattery);

    // start the timer since charging is initially false
    updateDrainTimer();
}

int Battery::getChargeLevel() const {
    return chargeLevel;
}

void Battery::setChargeLevel(int level) {
    // ensure level is within valid range
    int newLevel = qBound(0, level, 100);

    if (chargeLevel != newLevel) {
        chargeLevel = newLevel;
        emit chargeLevelChanged(chargeLevel);
    }
}

bool Battery::isCharging() const {
    return charging;
}

void Battery::setCharging(bool isCharging) {
    if (charging != isCharging) {
        charging = isCharging;

        // update timer based on new charging state
        updateDrainTimer();

        emit chargingChanged(charging);
    }
}

int Battery::getDrainInterval() const {
    return drainInterval;
}

void Battery::setDrainInterval(int interval) {
    if (drainInterval != interval && interval > 0) {
        drainInterval = interval;
        drainTimer->setInterval(interval);
        emit drainIntervalChanged(interval);
    }
}

int Battery::getDrainAmount() const {
    return drainAmount;
}

void Battery::setDrainAmount(int amount) {
    if (drainAmount != amount && amount > 0) {
        drainAmount = amount;
        emit drainAmountChanged(amount);
    }
}


void Battery::resetToFull() {
    setChargeLevel(100);
}

void Battery::drainBattery() {
    if (charging) {
        // in charging state increase battery level until max 100
        int newLevel = qMin(100, chargeLevel + drainAmount);

        // update charge level
        if (newLevel != chargeLevel) {
            chargeLevel = newLevel;
            emit chargeLevelChanged(chargeLevel);
        }
    } else {
        // in draining state decrease battery level
        int newLevel = qMax(0, chargeLevel - drainAmount);

        // update charge level
        if (newLevel != chargeLevel) {
            chargeLevel = newLevel;
            emit chargeLevelChanged(chargeLevel);
        }

        // if battery is drained stop the timer when reaches 0
        if (chargeLevel <= 0) {
            drainTimer->stop();
        }
    }
}

void Battery::updateDrainTimer() {
    // timer works for charging/draining regardless
    if (!drainTimer->isActive()) {
        drainTimer->start();
    }

    // if battery is empty/not charging stop the timer
    if (chargeLevel <= 0 && !charging) {
        drainTimer->stop();
    }
}
