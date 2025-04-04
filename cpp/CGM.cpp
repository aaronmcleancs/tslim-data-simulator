#include "headers/CGM.h"
#include "headers/Profile.h"
#include "headers/Battery.h"
#include "headers/contentwidget.h"
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>

CGM::CGM(QObject *parent)
    : QObject(parent), currentGlucoseLevel(0.0), controlIQActive(false), monitoringTimer(new QTimer(this)), battery(nullptr), profile(nullptr)
{
    monitoringTimer->setInterval(1000);
    connect(monitoringTimer, &QTimer::timeout, this, &CGM::update);

    connect(this, &CGM::newGlucoseReading, this, &CGM::updateGlucoseLevelFromControlIQ);
}

CGM::~CGM()
{
    delete monitoringTimer;
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
    //qDebug() << "CGM: Monitoring started";
}


void CGM::stopMonitoring()
{
    monitoringTimer->stop();
    qDebug() << "CGM monitoring stopped.";
}

double CGM::getCurrentCarbs() const
{
    return currentCarbs;
}

double CGM::getCurrentGlucoseLevel() const
{
    return currentGlucoseLevel;
}

double CGM::estimateCarbs()
{
    if (profile) {
        QVector<GlucoseReading> readings = profile->getGlucoseReadings();
        if (readings.size() < 2) return 0;  // Need at least 2 readings to estimate carbs

        double lastGlucose = readings.last().value;
        double prevGlucose = readings[readings.size() - 2].value;

        double glucoseRise = lastGlucose - prevGlucose;
        double glucoseToCarbFactor = 5.0;  // Estimate: 5 mg/dL per gram of carbs

        return glucoseRise / glucoseToCarbFactor;
    }
    return 0;
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
void CGM::update()
{
    qDebug() << currentGlucoseLevel << "and" << controlIQActive << "and "<< controlIQNewGlucoseLevel;
    if (!profile) {
        qDebug() << "Profile is null, glucose reading not added!";
        return;
    }
    if (battery && battery->getChargeLevel() == 0) {
        stopMonitoring();
        qDebug() << "Battery is empty. Stopping CGM monitoring.";
        return;
    }

    double glucose = currentGlucoseLevel;

    if (controlIQActive == true) {
        emit newGlucoseReading(glucose);
        glucose = controlIQNewGlucoseLevel;  // ControlIQ adjusted glucose
        qDebug() << "CGM using adjusted glucose from ControlIQ:" << glucose;
    } else {
        qDebug() << "CGM using normal data:" << glucose;

        fetchLatestGlucoseReading();
    }


    GlucoseReading newReading;
    newReading.value = glucose;
    newReading.timestamp = QDateTime::currentDateTime();  // Timestamp
    profile->addGlucoseReading(newReading);
}


void CGM::updateGlucoseLevelFromControlIQ(double newGlucoseLevel)
{
    controlIQNewGlucoseLevel = newGlucoseLevel;
    qDebug() << "CGM received glucose reading from ControlIQ:" << newGlucoseLevel;
}
