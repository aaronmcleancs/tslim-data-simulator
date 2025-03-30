#include "headers/CGM.h"
#include "headers/Profile.h"
#include "headers/Battery.h"
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>

CGM::CGM(QObject *parent)
    : QObject(parent), currentGlucoseLevel(0.0), monitoringTimer(new QTimer(this)), battery(nullptr), profile(nullptr)
{
    monitoringTimer->setInterval(1000);  // Default to 1-second interval
    connect(monitoringTimer, &QTimer::timeout, this, &CGM::update);
}

CGM::~CGM()
{
    delete monitoringTimer;
}

void CGM::startMonitoring()
{
    if (battery && battery->getChargeLevel() > 0) {
        monitoringTimer->start();
    } else {
        qDebug() << "Battery is empty. CGM will not start.";
    }
}

void CGM::stopMonitoring()
{
    monitoringTimer->stop();
}

double CGM::getCurrentCarbs() const
{
    return currentCarbs;
}

double CGM::getCurrentGlucoseLevel() const
{
    return currentGlucoseLevel;
}

double CGM::estimateCarbs() {
    if (profile) {
        QVector<GlucoseReading> readings = profile->getGlucoseReadings();
        if (readings.size() < 2) return 0; // Need at least 2 readings

        double lastGlucose = readings.last().value;
        double prevGlucose = readings[readings.size() - 2].value;

        double glucoseRise = lastGlucose - prevGlucose;
        double glucoseToCarbFactor = 5.0;  //  estimate: 5 mg/dL per gram of carbs (if im suppose to use carbs from somwhere else we can overlook this)

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

void CGM::update()
{
    if (battery && battery->getChargeLevel() == 0) {
        stopMonitoring();
        qDebug() << "Battery is empty. Stopping CGM";
        return;
    }

    fetchLatestGlucoseReading();

    double estimatedCarbs = estimateCarbs();
    qDebug() << "Estimated Carbs Based on Glucose: " << estimatedCarbs;

    if (glucoseLabel) {
            glucoseLabel->setText(QString::number(currentGlucoseLevel));
        }

        qDebug() << "Current Glucose Level: " << currentGlucoseLevel;
    }
