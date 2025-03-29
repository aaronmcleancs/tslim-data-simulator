#include "headers/Pump.h"
#include "mainwindow.h"
#include <QDebug>

Pump::Pump(QObject *parent)
    : QObject(parent), activeProfile(nullptr), battery(nullptr), 
      insulinCartridge(nullptr), cgm(nullptr), ui(nullptr)
{
    // Initialize other components as needed
}

Pump::~Pump(){
    // Save profiles before deleting
    for (Profile* profile : profiles) {
        profile->saveProfile();
        delete profile;
    }
    profiles.clear();
    
    // Clean up other components
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
    }
}

void Pump::removeProfile(QString name) {
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i]->getName() == name) {
            Profile::deleteProfile(name);
            delete profiles[i];
            profiles.remove(i);
            return;
        }
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
}

QVector<Profile*>& Pump::getProfiles() {
    return profiles;
}

Profile* Pump::getActiveProfile() const {
    return activeProfile;
}
