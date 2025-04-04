#include "headers/InsulinCartridge.h"

InsulinCartridge::InsulinCartridge(QObject *parent) : QObject(parent), remainingInsulin(100) // Defaulted to 100 units change if u guys want
{}

int InsulinCartridge::getRemainingInsulin() const
{
    return remainingInsulin;
}

void InsulinCartridge::setRemainingInsulin(int units)
{
    if (units < 0) {
        units = 0; // haha insulin cant go negative right guys :D
    }

    if (remainingInsulin != units) {
        remainingInsulin = units;
        emit insulinLevelChanged(remainingInsulin);
    }
}
