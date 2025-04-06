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
    if (!profile)
        return;

    auto range = profile->getTargetGlucoseRange();  // e.g., (min=3.9, max=10.0)
    double basal = profile->getBasalRate();
    double correctionFactor = profile->getCorrectionFactor();
    int remainingInsulin = pump->getInsulinCartridge()->getRemainingInsulin();

    if (remainingInsulin <= 0) {
        qDebug() << "[ControlIQ] Error: Insulin cartridge is empty!";
        pump->recordAlert("Insulin Empty!", remainingInsulin);
        emit pump->errorOccurred("Insulin cartridge is empty.");
        return;
    }

    // Correction when glucose is 20 mmol/L or above.
     if (glucose >= 20.0) {
        double target = 6.1;
        double difference = glucose - target;
        // Deliver 100%
        double correctionBolusUnits = (difference * 1.0) / correctionFactor;
        if (correctionBolusUnits > remainingInsulin)
            correctionBolusUnits = remainingInsulin;

        pump->recordBolus(correctionBolusUnits, "Correction");
        emit pump->bolusDelivered(correctionBolusUnits);
        pump->getInsulinCartridge()->setRemainingInsulin(remainingInsulin - correctionBolusUnits);
        qDebug() << "[ControlIQ] Correction bolus:" << correctionBolusUnits << "units for glucose" << glucose;

        // Apply insulin effect: assume each unit lowers glucose by 1.0 mmol/L.
        double reduction = correctionBolusUnits * 1.0;
        pump->getCGM()->applyInsulinEffect(reduction);
    }
    else if (glucose >= 10.0) {
        double target = 6.1;
        double difference = glucose - target;
        // Deliver 60% of the difference (converted by correction factor)
        double correctionBolusUnits = (difference * 0.6) / correctionFactor;
        if (correctionBolusUnits > remainingInsulin)
            correctionBolusUnits = remainingInsulin;

        pump->recordBolus(correctionBolusUnits, "Correction");
        emit pump->bolusDelivered(correctionBolusUnits);
        pump->getInsulinCartridge()->setRemainingInsulin(remainingInsulin - correctionBolusUnits);
        qDebug() << "[ControlIQ] Correction bolus:" << correctionBolusUnits << "units for glucose" << glucose;

        // Apply insulin effect: assume each unit lowers glucose by 1.0 mmol/L.
        double reduction = correctionBolusUnits * 1.0;
        pump->getCGM()->applyInsulinEffect(reduction);
    }
    else if (glucose < range.first) {
        // If too low, suspend basal delivery.
        emit pump->basalDeliveryStopped();
        pump->recordAlert("Glucose Low - Suspend", glucose);
        qDebug() << "[ControlIQ] Glucose low (" << glucose << "). Basal suspended.";
    }
    else {
        // For normal values, deliver the normal basal rate.
        pump->recordBasalRateChange(basal);
        emit pump->basalDeliveryStarted();
        qDebug() << "[ControlIQ] Glucose normal (" << glucose << "). Basal delivered.";
    }
}

void ControlIQ::triggerSafetyCheck(double glucose) {
    if (glucose < 2.8 || glucose > 20.0) {
        pump->recordAlert("Critical Glucose", glucose);
        qDebug() << "[ControlIQ] CRITICAL: Glucose=" << glucose << ". ControlIQ will activate safety.";
    }
}
