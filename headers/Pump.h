#ifndef PUMP_H
#define PUMP_H

#include <QObject>
#include <QVector>
#include "Battery.h"
#include "InsulinCartridge.h"
#include "CGM.h"
#include "Profile.h"
#include "PumpHistory.h"
#include "UI.h"

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class Pump : public QObject
{
    Q_OBJECT
public:
    explicit Pump(QObject *parent = nullptr);
    ~Pump();

    Battery* getBattery() const;
    InsulinCartridge* getInsulinCartridge() const;
    CGM* getCGM() const;
    UI* getUI() const;
    PumpHistory* getPumpHistory() const;

    void createProfile(QString n, double br, double cr, double cf, double tmin, double tmax);
    void removeProfile(QString name);
    void updateProfile(QString name, QString setting, double val);
    int findIndex(QString name);
    QVector<Profile*> getProfiles() const;

    void selectActiveProfile(QString name);
    Profile* getActiveProfile() const;
    QVector<Profile*>& getProfiles();

    void updateSettings();
    
    // PumpHistory methods
    void recordBasalRateChange(double rate);
    void recordBolus(double amount, const QString &bolusType, int duration = 0, double carbInput = 0.0, double bgInput = 0.0);
    void recordAlert(const QString &alertType, double bgValue);
    void recordSettingChange(const QString &settingName, const QString &oldValue, const QString &newValue);
    void recordStatusEvent(const QString &statusType, const QString &statusDetails);
    
    // Status and history access methods
    BolusEvent getLastBolus() const;
    BasalRateEvent getCurrentBasalRate() const;
    QVector<PumpEvent> getRecentEvents(int count) const;

public slots:

signals:
    void bolusDelivered(double units);
    void basalDeliveryStarted();
    void basalDeliveryStopped();
    void errorOccurred(const QString &error);
    void alertTriggered(const QString &alertType, double bgValue);
    void settingChanged(const QString &settingName, const QString &oldValue, const QString &newValue);
    void statusUpdated(const QString &statusType, const QString &statusDetails);

protected :
    Profile *activeProfile;

private:
    Battery *battery;
    InsulinCartridge *insulinCartridge;
    CGM *cgm;
    UI *ui;
    PumpHistory *pumpHistory;
    QVector<Profile*> profiles;
};

#endif
