#include "headers/CGM.h"
#include "headers/Profile.h"
#include "headers/Battery.h"
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>
#include "headers/Pump.h";

CGM::CGM(Pump* pump, QObject *parent)
    : QObject(parent),CGMON(true), currentGlucoseLevel(7.0), controlIQActive(true),
    monitoringTimer(new QTimer(this)), battery(nullptr), profile(nullptr), insulinEffect(0.0),pump(pump) {
    monitoringTimer->setInterval(5000);
    connect(monitoringTimer, &QTimer::timeout, this, &CGM::update);
    startTime = QDateTime::currentDateTime();
    connect(this, &CGM::newGlucoseReading, this, &CGM::updateGlucoseLevelFromControlIQ);
    fetchLatestGlucoseReading();
}

CGM::~CGM()
{
    delete monitoringTimer;
}

void CGM::applyInsulinEffect(double effect) {
    insulinEffect += effect;
}

void CGM::setProfile(Profile* p) {
    profile = p;
    qDebug() << "CGM: Profile set to" << (profile ? profile->getName() : "null");
}

void CGM::startMonitoring() {
    if (!monitoringTimer) {
        monitoringTimer = new QTimer(this);
        connect(monitoringTimer, &QTimer::timeout, this, &CGM::update);
    }
    monitoringTimer->start(5000); // or whatever interval
    CGMON=true;
    //qDebug() << "CGM: Monitoring started";
    startTime = QDateTime::currentDateTime();
}


void CGM::stopMonitoring()
{
    monitoringTimer->stop();
    qDebug() << "CGM monitoring stopped.";
    CGMON =false;
}

double CGM::getCurrentCarbs() const
{
    return currentcarbs;
}

double CGM::getCurrentGlucoseLevel() const
{
    return currentGlucoseLevel;
}

void CGM::estimateCarbs()
{
    if (profile) {
        QVector<GlucoseReading> readings = profile->getGlucoseReadings();
        if (!(readings.size() < 2)) { // Need at least 2 readings to estimate carbs

            double lastGlucose = readings.last().value;
            double prevGlucose = readings[readings.size() - 2].value;

            double glucoseRise = lastGlucose - prevGlucose;
            double glucoseToCarbFactor = 5.0;  // Estimate: 5 mg/dL per gram of carbs

            currentcarbs = glucoseRise / glucoseToCarbFactor;
        }
    }

}


void CGM::fetchLatestGlucoseReading()
{
    if (profile) {
        QVector<GlucoseReading> readings = profile->getGlucoseReadings();
        if (!readings.isEmpty()) {
            currentGlucoseLevel = readings.last().value;
        }
    }
}

void CGM::setControlIQActive(bool active) {
    controlIQActive = active;
    qDebug() << "ControlIQ active set to:" << controlIQActive;
}

// Getter for controlIQActive
bool CGM::isControlIQActive() const {
    return controlIQActive;
}

// IM GONNA FIX THIS TONIGHT
void CGM::update() {
    if (!profile) {
        qDebug() << "Profile is null, glucose reading not added!";
        return;
    }
    if (battery && battery->getChargeLevel() == 0) {
        stopMonitoring();
        qDebug() << "Battery is empty. Stopping CGM monitoring.";
        return;
    } else if (!CGMON) {
        startMonitoring();
        return;
    }

    double glucose = currentGlucoseLevel;

    // Natural rise in glucose
    glucose += 0.3;  // Slow, modest rise per 5-second cycle.

    // Apply basal insulin effect.
    // For simulation, assume each unit of basal insulin lowers glucose by 0.05 mmol/L per cycle.
    if (pump && pump->getActiveProfile()) {
        double basal = pump->getActiveProfile()->getBasalRate();
        double basalEffect = basal * 0.05;
        glucose -= basalEffect;

        // Convert basal from hourly to per-cycle units (assuming cycle is 5 seconds)
        double basalUnitsPerCycle = basal / (3600 / 5); // basal per 5 seconds
        pump->getInsulinCartridge()->setRemainingInsulin(
            pump->getInsulinCartridge()->getRemainingInsulin() - basalUnitsPerCycle
            );
        emit pump->recordBasalRateChange(basalUnitsPerCycle);
    }


    qDebug() << glucose << "glucose before controliq";

    if (controlIQActive == true) {
        emit newGlucoseReading(glucose);
        // If ControlIQ modifies the value, use its adjusted value.
        glucose = controlIQNewGlucoseLevel;
    } else {
        //qDebug() << "CGM using normal data:" << glucose;
        fetchLatestGlucoseReading();
    }

    // Apply any insulin effect (from correction boluses or manual bolus)
    glucose -= insulinEffect;

    // Decay the insulin effect over time
    insulinEffect *= 0.9;
    if (insulinEffect < 0.01)
        insulinEffect = 0;

    // Record the new glucose reading
    GlucoseReading newReading;
    newReading.value = glucose;
    newReading.timestamp = QDateTime::currentDateTime();
    profile->addGlucoseReading(newReading);

    // Update current glucose level for next cycle.
    currentGlucoseLevel = glucose;
}



void CGM::updateGlucoseLevelFromControlIQ(double newGlucoseLevel)
{
    controlIQNewGlucoseLevel = newGlucoseLevel;
    //qDebug() << "CGM received glucose reading from ControlIQ:" << newGlucoseLevel;
}
