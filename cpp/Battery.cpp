#include "headers/Battery.h"

Battery::Battery(QObject *parent)
    : QObject(parent)
    , chargeLevel(100)    // Start with full battery
    , charging(false)     // Start in discharging state
    , drainInterval(10000) // Drain every 10 seconds by default
    , drainAmount(1)      // Drain 1% per interval by default
{
    // Create and set up the drain timer
    drainTimer = new QTimer(this);
    drainTimer->setInterval(drainInterval);
    connect(drainTimer, &QTimer::timeout, this, &Battery::drainBattery);

    // Start the timer since charging is initially false
    updateDrainTimer();
}

int Battery::getChargeLevel() const {
    return chargeLevel;
}

void Battery::setChargeLevel(int level) {
    // Ensure level is within valid range
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

        // Update timer based on new charging state
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

void Battery::drainBattery() {
    // Only drain if we're not charging
    if (!charging) {
        // Get current level and calculate new level
        int newLevel = qMax(0, chargeLevel - drainAmount);

        // Update charge level
        if (newLevel != chargeLevel) {
            chargeLevel = newLevel;
            emit chargeLevelChanged(chargeLevel);
        }

        // If battery is depleted, stop the timer
        if (chargeLevel <= 0) {
            drainTimer->stop();
        }
    }
}

void Battery::resetToFull() {
    setChargeLevel(100);
}

void Battery::updateDrainTimer() {
    // If charging, stop the drain timer
    if (charging) {
        if (drainTimer->isActive()) {
            drainTimer->stop();
        }
    }
    // If not charging and battery not empty, start the drain timer
    else {
        if (!drainTimer->isActive() && chargeLevel > 0) {
            drainTimer->start();
        }
    }
}
