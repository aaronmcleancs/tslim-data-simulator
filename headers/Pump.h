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

    void createProfile(QString n, double br, double cr, double cf, double t);
    void removeProfile(QString name);
    void updateProfile(QString name, QString setting, double val);
    int findIndex(QString name);
    QVector<Profile*> getProfiles() const;

    void selectActiveProfile(QString name);
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