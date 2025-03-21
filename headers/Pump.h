#ifndef PUMP_H
#define PUMP_H

#include <QObject>
#include <QVector>
#include "Battery.h"
#include "InsulinCartridge.h"
#include "CGM.h"
#include "Profile.h"
#include "UI.h"

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class Pump : public QObject
{
    Q_OBJECT
public:
    explicit Pump(QObject *parent = nullptr);

    Battery* getBattery() const;
    InsulinCartridge* getInsulinCartridge() const;
    CGM* getCGM() const;
    UI* getUI() const;

    void addProfile(Profile* profile);
    void removeProfile(Profile* profile);
    QVector<Profile*> getProfiles() const;

    void selectActiveProfile(Profile* profile);
    Profile* getActiveProfile() const;

public slots:
    void deliverBolus(double units);
    void startBasalDelivery();
    void stopBasalDelivery();

signals:
    void bolusDelivered(double units);
    void basalDeliveryStarted();
    void basalDeliveryStopped();
    void errorOccurred(const QString &error);

private:
    Battery *battery;
    InsulinCartridge *insulinCartridge;
    CGM *cgm;
    UI *ui;
    QVector<Profile*> profiles;
    Profile *activeProfile;
};

#endif