#include "headers/BolusCalculator.h"
#include "headers/Profile.h"

int BolusCalculator :: Carb_Bolus_Calculation(int total_carbs){
     return static_cast<int>(total_carbs / activeProfile->getCarbRatio());
}

int BolusCalculator :: Correction_Bolus_Calculation(int current_glucose){
    QPair<double, double> range = activeProfile->getTargetGlucoseRange();
        double target = (range.first + range.second) / 2.0;
        return static_cast<int>((current_glucose - target) / activeProfile->getCorrectionFactor());
}

int BolusCalculator::total_bolus(int total_carbs, int current_glucose) {
    int carbBolus = Carb_Bolus_Calculation(total_carbs);
    int correctionBolus = Correction_Bolus_Calculation(current_glucose);
    int totalBolus = carbBolus + correctionBolus;
    bolus = totalBolus;
    return totalBolus;
}
