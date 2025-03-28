#include "headers/Pump.h"
#include "mainwindow.h"
#include <QDebug>

Pump::Pump(QObject *parent){

}

Pump::~Pump(){

}

void Pump::createProfile(QString n, double br, double cr, double cf, double tmin, double tmax){
    Profile* new_profile = new Profile(n, nullptr);
    new_profile->setBasalRate(br);
    new_profile->setCarbRatio(cr);
    new_profile->setTargetGlucoseRange(tmin, tmax);
    profiles.append(new_profile);
}

void Pump::removeProfile(QString name) {
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i]->getName() == name) {
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
