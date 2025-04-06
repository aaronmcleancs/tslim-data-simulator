#include "headers/ControlIQ.h"
#include <QDebug>

ControlIQ::ControlIQ(Pump* p, QObject *parent)
    : QObject(parent), pump(p)
{
    if (pump && pump->getCGM()) {
        connect(pump->getCGM(), &CGM::newGlucoseReading, this, &ControlIQ::evaluate);
        qDebug() << "ControlIQ connected to CGM's newGlucoseReading signal";
    }
}


void ControlIQ::evaluate(double glucose)
{
    if ( !pump || !pump->getCGM() || !pump->getActiveProfile()) return;

    //qDebug() << "ControlIQ evaluating with glucose:" << glucose;
    adjustInsulinDelivery(glucose);
    triggerSafetyCheck(glucose);


    // Proceed with sending glucose to CGM
    double newGlucose = glucose;  // Adjusted glucose value after insulin calculations
    emit pump->getCGM()->newGlucoseReading(newGlucose);
}




void ControlIQ::adjustInsulinDelivery(double glucose) {
    Profile* profile = pump->getActiveProfile();
    if (!profile) return;

    auto range = profile->getTargetGlucoseRange();
    double basal = profile->getBasalRate();
    double correctionFactor = profile->getCorrectionFactor();

    int remainingInsulin = pump->getInsulinCartridge()->getRemainingInsulin();

    if (remainingInsulin <= 0) {
        qDebug() << "[ControlIQ] Error: Insulin cartridge is empty!";
        emit pump->errorOccurred("Insulin cartridge is empty.");
        return;
    }

    if (glucose < range.first) {
        emit pump->basalDeliveryStopped();
        pump->recordAlert("Glucose Low - Suspend", glucose);
        qDebug() << "[ControlIQ] Glucose low (" << glucose << "). Basal suspended.";
    }
    else if (glucose > range.second + 2.0) {
        double excess = glucose - range.second;
        double units = excess / correctionFactor;

        if (units > remainingInsulin) {
            units = remainingInsulin;  // Limit to the remaining insulin
        }

        pump->recordBolus(units, "Correction");
        emit pump->bolusDelivered(units);
        pump->getInsulinCartridge()->setRemainingInsulin(remainingInsulin - units);
        qDebug() << "[ControlIQ] Glucose very high (" << glucose << "). Correction bolus:" << units;
    }
    else if (glucose > range.second) {
        double increasedBasal = basal * 1.5;

        if (increasedBasal > remainingInsulin) {
            increasedBasal = remainingInsulin;
        }

        pump->recordBasalRateChange(increasedBasal);
        emit pump->basalDeliveryStarted();
        pump->getInsulinCartridge()->setRemainingInsulin(remainingInsulin - increasedBasal);
        qDebug() << "[ControlIQ] Glucose elevated (" << glucose << "). Temporary increased basal.";
    }
    else {
        pump->recordBasalRateChange(basal);
        emit pump->basalDeliveryStarted();
        qDebug() << "[ControlIQ] Glucose normal (" << glucose << "). Basal delivered.";
    }
}


void ControlIQ::triggerSafetyCheck(double glucose)
{
    if (glucose < 2.8 || glucose > 20.0) {
        emit pump->errorOccurred("Glucose dangerously out of bounds!");
        pump->recordAlert("Critical Glucose", glucose);
        qDebug() << "[ControlIQ] CRITICAL: Glucose=" << glucose << ". ControlIQ stopped for safety.";
    }
}
