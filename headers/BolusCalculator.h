#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include "Pump.h"

#include <QObject>

class BolusCalculator : public Pump{
    Q_OBJECT
public:

    double Carb_Bolus_Calculation(int total_carbs);
    double Correction_Bolus_Calculation(int current_glucose);
    double total_bolus(int total_carbs, int current_glucose,Profile*);
    void setCurrentProfile(Profile* p);

private:
    double bolus = 0.0;
    Profile* activeProfile;

};

#endif // BOLUSCALCULATOR_H
