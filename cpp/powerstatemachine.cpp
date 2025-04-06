#include "headers/powerstatemachine.h"

PowerStateMachine::PowerStateMachine(StatusModel* model, QObject *parent)
    : QObject(parent),
      model(model),
      powered(false),
      minBatteryLevel(5)
{
    connect(model, &StatusModel::batteryLevelChanged,
            this, &PowerStateMachine::handleBatteryChange);
}

bool PowerStateMachine::isPowered() const
{
    return powered;
}

int PowerStateMachine::getMinBatteryLevel() const
{
    return minBatteryLevel;
}

bool PowerStateMachine::powerOn()
{
    if (!powered) {
        if (model->getBatteryLevel() > minBatteryLevel || model->isBatteryCharging()) {
            setPowered(true);
            return true;
        }
        return false;
    }
    return true;
}

void PowerStateMachine::powerOff()
{
    if (powered) {
        setPowered(false);
        emit systemShuttingDown();
    }
}

void PowerStateMachine::handleBatteryChange(int level)
{
    if (!powered) {
        return;
    }

    if (level <= minBatteryLevel && !model->isBatteryCharging()) {
        powerOff();
    }
}

void PowerStateMachine::setPowered(bool newState)
{
    if (powered != newState) {
        powered = newState;
        emit poweredChanged(powered);
    }
}
