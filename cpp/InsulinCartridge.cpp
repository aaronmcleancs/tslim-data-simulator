#include "InsulinCartridge.h"

// Example skeleton for initial repo
// Feel free to change any of this to your implementation

InsulinCartridge::InsulinCartridge(QObject *parent)
    : QObject(parent), remainingInsulin(300)
{
}

int InsulinCartridge::getRemainingInsulin() const
{
    return remainingInsulin;
}

void InsulinCartridge::setRemainingInsulin(int units)
{
    if(remainingInsulin != units){
        remainingInsulin = units;
        emit insulinLevelChanged(remainingInsulin);
    }
}