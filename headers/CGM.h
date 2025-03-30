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
    double getCurrentGlucoseLevel() const;
    double getCurrentCarbs() const;
    double estimateCarbs();
    void setGlucoseLabel(QLabel *label);

private slots:
    void update();

private:
    double currentGlucoseLevel;
    double currentCarbs;
    QTimer *monitoringTimer;
    Battery *battery;
    Profile *profile;
    QLabel *glucoseLabel;

    void fetchLatestGlucoseReading();
};

#endif // CGM_H
