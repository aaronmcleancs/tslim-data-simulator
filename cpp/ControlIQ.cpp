#include "headers/ControlIQ.h"
#include <QDebug>
#include "headers/BolusCalculator.h"

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
    int remainingInsulin = pump->getInsulinCartridge()->getRemainingInsulin();

    if (remainingInsulin <= 0) {
        qDebug() << "[ControlIQ] Error: Insulin cartridge is empty!";
        pump->recordAlert("Insulin Empty!", remainingInsulin);
        emit pump->errorOccurred("Insulin cartridge is empty.");
        return;
    }

    // Correction when glucose is 20 mmol/L or above.
    if (glucose > 10.0) {
        BolusCalculator b1;
        double total_carbs = pump->getCGM()->getCurrentCarbs();
        InsulinCartridge* Cartridge = pump->getInsulinCartridge();
        double correctionBolusUnits = b1.total_bolus( total_carbs,glucose, profile, Cartridge);

        pump->recordBolus(correctionBolusUnits, "Correction");
        emit pump->bolusDelivered(correctionBolusUnits);
        qDebug() << "[ControlIQ] Correction bolus:" << correctionBolusUnits << "units for glucose" << glucose;
        pump->getCGM()->applyInsulinEffect(correctionBolusUnits);
    }
    else if (glucose < range.first || glucose <= 3.9) {
        // If too low, suspend basal delivery.
        emit pump->basalDeliveryStopped();
        pump->recordAlert("Glucose Low - Suspend", glucose);
        qDebug() << "[ControlIQ] Glucose low (" << glucose << "). Basal suspended.";
    }
    else {
        // For normal values, deliver the normal basal rate.
        pump->recordBasalRateChange(basal);
        qDebug() << "[ControlIQ] Glucose normal (" << glucose << "). Basal delivered.";
    }
}


