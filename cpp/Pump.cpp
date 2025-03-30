#include "headers/Pump.h"
#include "mainwindow.h"
#include <QDebug>

Pump::Pump(QObject *parent)
    : QObject(parent), activeProfile(nullptr), battery(nullptr),
      insulinCartridge(nullptr), cgm(nullptr), ui(nullptr)
{
    //any more constructor code here
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
    delete ui;
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
    }
}

void Pump::removeProfile(QString name) {
    // First ensure we don't have duplicates
    bool found = false;
    for (int i = profiles.size() - 1; i >= 0; i--) {
        if (profiles[i]->getName() == name) {
            if (activeProfile == profiles[i]) {
                activeProfile = nullptr;
            }
            delete profiles[i];
            profiles.remove(i);
            found = true;
        }
    }

    // Then remove from persistent storage
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
    if(setting == "basal rate"){
        profiles[index]->setBasalRate(val);
    }else if(setting == "carb ratio"){
        profiles[index]->setCarbRatio(val);
    }else if(setting == "correction factor"){
        profiles[index]->setCorrectionFactor(val);
    }else if(setting == "target min"){
        double max = profiles[index]->getTargetGlucoseRange().second;
        profiles[index]->setTargetGlucoseRange(val, max);
    }else if(setting == "target max"){
        double min = profiles[index]->getTargetGlucoseRange().first;
        profiles[index]->setTargetGlucoseRange(min, val);
    }
    qDebug() << "changed" << setting << "for profile" << name << "to" << val;
    profiles[index]->saveProfile();
}

int Pump::findIndex(QString name){
    for(int i = 0; i < profiles.size(); i++){
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
    activeProfile = profiles[index];
    qDebug() << "Active profile is now " << name;
}

QVector<Profile*>& Pump::getProfiles() {
    return profiles;
}

Profile* Pump::getActiveProfile() const {
    return activeProfile;
}
