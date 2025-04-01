#include "../headers/statusmodel.h"

StatusModel* StatusModel::instance = nullptr;

StatusModel* StatusModel::getInstance() {
    if (!instance) {
        instance = new StatusModel();
    }
    return instance;
}

StatusModel::StatusModel(QObject *parent) : QObject(parent), batteryLevel(0), rightText(0) {
}

void StatusModel::setBatteryLevel(int level) {
    if (batteryLevel != level) {
        batteryLevel = level;
        emit batteryLevelChanged(level);
    }
}

void StatusModel::setRightText(int level) {
    if (rightText != level) {
        rightText = level;
        emit rightTextChanged(level);
    }
}
