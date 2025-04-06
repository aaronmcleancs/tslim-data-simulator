#ifndef CGM_H
#define CGM_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QDateTime>
#include "Profile.h"

class Battery;
class Pump;
class CGM : public QObject {
    Q_OBJECT
public:
    explicit CGM(Pump* pump, QObject *parent = nullptr);
    ~CGM();

    void setProfile(Profile* p);
    void startMonitoring();
    void stopMonitoring();
    double getCurrentGlucoseLevel() const;
    double getCurrentCarbs() const;
    QDateTime getStartTime() const {return startTime;};
    void fetchLatestGlucoseReading();
    void setControlIQActive(bool active);
    bool isControlIQActive() const;
    void applyInsulinEffect(double effect);
    void estimateCarbs();

signals:
    void newGlucoseReading(double glucose);

public slots:
    void update();
    void updateGlucoseLevelFromControlIQ(double newGlucoseLevel);

private:
    QDateTime startTime;
    bool CGMON;
    double currentGlucoseLevel;
    bool controlIQActive;
    QTimer* monitoringTimer;
    Battery* battery;
    Profile* profile;
    double controlIQNewGlucoseLevel;
    double insulinEffect;
    double currentcarbs;
    Pump* pump;
};

#endif // CGM_H
