#include "headers/InsulinCartridge.h"
#include "headers/Pump.h"
#include "statusbar.h"


InsulinCartridge::InsulinCartridge(Pump* pump, QObject *parent) : QObject(parent), pump(pump), remainingInsulin(300) // Defaulted to 100 units change if u guys want
{
    connect(this, &InsulinCartridge::insulinLevelChanged,statusBar, &StatusBar::onUnitsChanged);
}

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
    if (remainingInsulin == 0){
        pump->recordAlert("INSULIN EMPTY", remainingInsulin);
    }else if (remainingInsulin <=5 ){
        pump->recordAlert("INSULIN LOW", remainingInsulin);
    }

}
