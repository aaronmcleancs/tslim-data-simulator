#ifndef CGM_H
#define CGM_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include "Profile.h"
#include "Battery.h"

class CGM : public QObject
{
    Q_OBJECT
public:
    explicit CGM(QObject *parent = nullptr);
    ~CGM();

    void startMonitoring();
    void stopMonitoring();
    void setProfile(Profile* p);
    double getCurrentGlucoseLevel() const;
    double getCurrentCarbs() const;
    double estimateCarbs();
    void setGlucoseLabel(QLabel *label);
    void setControlIQActive(bool active);
    bool isControlIQActive() const;

signals:
    void newGlucoseReading(double glucoseLevel); // To ControlIQ
public slots:
    void updateGlucoseLevelFromControlIQ(double newGlucoseLevel); // From ControlIQ

private slots:
    void update();

private:
    double currentGlucoseLevel;
    bool controlIQActive;
    double currentCarbs;
    QTimer *monitoringTimer;
    Battery *battery;
    Profile *profile;
    double controlIQNewGlucoseLevel;
    void fetchLatestGlucoseReading();  // From profile
};

#endif
