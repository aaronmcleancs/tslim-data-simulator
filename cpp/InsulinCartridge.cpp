#include "headers/InsulinCartridge.h"
#include "headers/Pump.h";


InsulinCartridge::InsulinCartridge(Pump* pump, QObject *parent) : QObject(parent), pump(pump), remainingInsulin(100) // Defaulted to 100 units change if u guys want
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
