#include "CGM.h"

// Example skeleton for initial repo
// Feel free to change any of this to your implementation

CGM::CGM(QObject *parent)
    : QObject(parent), currentGlucose(5.5)
{
}

double CGM::getCurrentGlucose() const
{
    return currentGlucose;
}

void CGM::updateGlucoseLevel(double glucose)
{
    if(currentGlucose != glucose){
        currentGlucose = glucose;
        emit glucoseLevelUpdated(currentGlucose);
    }
}