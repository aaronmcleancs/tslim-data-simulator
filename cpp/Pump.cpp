#include "headers/Pump.h"
#include "mainwindow.h"
#include <QDebug>

Pump::Pump(QObject *parent)
    : QObject(parent), activeProfile(nullptr), battery(nullptr),
      insulinCartridge(nullptr), cgm(nullptr), pumpHistory(nullptr)
{
    insulinCartridge = new InsulinCartridge(this);
    cgm = new CGM(this);
    qDebug() << "Pump constructor: CGM initialized"; // Using this purely for debug purposes
}

Pump::~Pump(){
    for (Profile* profile : profiles) {
        profile->saveProfile();
        delete profile;
    }
    profiles.clear();
    
    delete battery;
    delete insulinCartridge;
    delete cgm;
    delete pumpHistory;
}

Battery* Pump::getBattery() const {
    return battery;
}

InsulinCartridge* Pump::getInsulinCartridge() const {
    return insulinCartridge;
}

CGM* Pump::getCGM() const {
    return cgm;
}

PumpHistory* Pump::getPumpHistory() const {
    return pumpHistory;
}

void Pump::createProfile(QString n, double br, double cr, double cf, double tmin, double tmax){
    if (Profile::createProfile(n)) {
        Profile* new_profile = new Profile(n, nullptr);
        new_profile->setBasalRate(br);
        new_profile->setCarbRatio(cr);
        new_profile->setCorrectionFactor(cf);
        new_profile->setTargetGlucoseRange(tmin, tmax);
        new_profile->saveProfile();
        profiles.append(new_profile);
        qDebug() << "Created profile:" << n;
        if (pumpHistory) {
            delete pumpHistory;
        }
        pumpHistory = new PumpHistory(n, this);
        
        // Store the current active profile
        Profile* previousActive = activeProfile;
        activeProfile = new_profile;
        
        recordStatusEvent("Profile Creation", "Created new profile: " + n);
        
        recordSettingChange("Basal Rate", "0.0", QString::number(br));
        recordSettingChange("Carb Ratio", "0.0", QString::number(cr));
        recordSettingChange("Correction Factor", "0.0", QString::number(cf));
        recordSettingChange("Target Glucose Min", "0.0", QString::number(tmin));
        recordSettingChange("Target Glucose Max", "0.0", QString::number(tmax));
        activeProfile = previousActive;
    }
}

void Pump::removeProfile(QString name) {
    bool found = false;
    for (int i = profiles.size() - 1; i >= 0; i--) {
        if (profiles[i]->getName() == name) {
            if (activeProfile == profiles[i]) {
                activeProfile = nullptr;
                if (pumpHistory) {
                    delete pumpHistory;
                    pumpHistory = nullptr;
                }
            }
            Profile* remove = profiles[i];
            profiles.remove(i);
            delete remove;
            found = true;
        }
    }
    if (found) {
        if (!Profile::deleteProfile(name)) {
            qWarning() << "Failed to delete profile from storage:" << name;
        }
    } else {
        qWarning() << "Profile not found for removal:" << name;
    }
}

void Pump::updateProfile(QString name, QString setting, double val){
    int index = findIndex(name);
    if(index == -1){
        qDebug("no profile found matching that name (edit)");
        return;
    }
    
    QString oldValue = "0.0";
    
    if(setting == "basal rate"){
        oldValue = QString::number(profiles[index]->getBasalRate());
        profiles[index]->setBasalRate(val);
        
        if (pumpHistory && activeProfile && activeProfile->getName() == name) {
            recordBasalRateChange(val);
        }
    }else if(setting == "carb ratio"){
        oldValue = QString::number(profiles[index]->getCarbRatio());
        profiles[index]->setCarbRatio(val);
    }else if(setting == "correction factor"){
        oldValue = QString::number(profiles[index]->getCorrectionFactor());
        profiles[index]->setCorrectionFactor(val);
    }else if(setting == "target min"){
        oldValue = QString::number(profiles[index]->getTargetGlucoseRange().first);
        double max = profiles[index]->getTargetGlucoseRange().second;
        profiles[index]->setTargetGlucoseRange(val, max);
    }else if(setting == "target max"){
        oldValue = QString::number(profiles[index]->getTargetGlucoseRange().second);
        double min = profiles[index]->getTargetGlucoseRange().first;
        profiles[index]->setTargetGlucoseRange(min, val);
    }
    
    qDebug() << "changed" << setting << "for profile" << name << "to" << val;
    profiles[index]->saveProfile();
    
    // record setting change in history
    if (pumpHistory && activeProfile && activeProfile->getName() == name) {
        recordSettingChange(setting, oldValue, QString::number(val));
    }

    updateSettings();
}

int Pump::findIndex(QString name){
    for(int i = 0; i < profiles.size(); i++){
        qDebug() << "profile" << i << "name:" << profiles[i]->getName();
        if(profiles[i]->getName() == name){
            return i;
        }
    }
    return -1; //error
}

void Pump::selectActiveProfile(QString name){
    int index = findIndex(name);
    if(index == -1){
        qDebug("no profile found matching that name (select)");
        return;
    }
    
    QString oldProfile = (activeProfile) ? activeProfile->getName() : "None";
    activeProfile = profiles[index];
    qDebug() << "Active profile is now " << name;
    
    if (pumpHistory) {
        delete pumpHistory;
    }
    pumpHistory = new PumpHistory(name, this);
    if (cgm) {
            cgm->setProfile(activeProfile);
            cgm->startMonitoring();
            qDebug() << "CGM started monitoring with profile:" << activeProfile->getName();
        } else {
            qWarning() << "CGM is null in selectActiveProfile!";
        }

    
    recordStatusEvent("Profile Change", "Changed active profile from " + oldProfile + " to " + name);
}

QVector<Profile*>& Pump::getProfiles() {
    return profiles;
}

Profile* Pump::getActiveProfile() const {
    return activeProfile;
}

void Pump::updateSettings(){
    // Any additional functionality needed
}

// PumpHistory recording methods
void Pump::recordBasalRateChange(double rate) {
    if (!pumpHistory || !activeProfile) {
        return;
    }
    
    BasalRateEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = "BasalRateChange";
    event.rate = rate;
    
    pumpHistory->addBasalRateEvent(event);
    emit basalDeliveryStarted();
}

void Pump::recordBolus(double amount, const QString &bolusType, int duration, double carbInput, double bgInput) {
    if (!pumpHistory || !activeProfile) {
        return;
    }
    
    BolusEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = "Bolus";
    event.amount = amount;
    event.bolusType = bolusType;
    event.duration = duration;
    event.carbInput = carbInput;
    event.bgInput = bgInput;
    
    pumpHistory->addBolusEvent(event);
    emit bolusDelivered(amount);
}

void Pump::recordAlert(const QString &alertType, double bgValue) {
    if (!pumpHistory || !activeProfile) {
        return;
    }
    
    AlertEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = "Alert";
    event.alertType = alertType;
    event.bgValue = bgValue;
    event.acknowledged = false;
    
    pumpHistory->addAlertEvent(event);
    emit alertTriggered(alertType, bgValue);
}

void Pump::recordSettingChange(const QString &settingName, const QString &oldValue, const QString &newValue) {
    if (!pumpHistory || !activeProfile) {
        return;
    }
    
    SettingChangeEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = "SettingChange";
    event.settingName = settingName;
    event.oldValue = oldValue;
    event.newValue = newValue;
    
    pumpHistory->addSettingChangeEvent(event);
    emit settingChanged(settingName, oldValue, newValue);
}

void Pump::recordStatusEvent(const QString &statusType, const QString &statusDetails) {
    if (!pumpHistory || !activeProfile) {
        return;
    }
    
    StatusEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = "Status";
    event.statusType = statusType;
    event.statusDetails = statusDetails;
    
    pumpHistory->addStatusEvent(event);
    emit statusUpdated(statusType, statusDetails);
}

// Status and history access methods
BolusEvent Pump::getLastBolus() const {
    if (!pumpHistory) {
        return BolusEvent();
    }
    return pumpHistory->getLastBolus();
}

BasalRateEvent Pump::getCurrentBasalRate() const {
    if (!pumpHistory) {
        return BasalRateEvent();
    }
    return pumpHistory->getCurrentBasalRate();
}

QVector<PumpEvent> Pump::getRecentEvents(int count) const {
    if (!pumpHistory) {
        return QVector<PumpEvent>();
    }
    return pumpHistory->getRecentEvents(count);
}
