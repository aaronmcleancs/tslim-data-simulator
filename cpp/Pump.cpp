#include "headers/Pump.h"
#include <QDebug>

Pump::Pump(){

}

Pump::~Pump(){

}

void Pump::CreateProfile(QString n, float br, float cr, float cf, float t){
    Profile* new_profile = new Profile(n, br, cr, cf, t);
    profiles.append(new_profile);
}

void Pump::DeleteProfile(QString name) {
    for (int i =  ️0; i < profiles.size(); i++) {
        if (profiles[i]->getName() == name) {
            delete profiles[i];
            profiles.remove(i);
            return;
        }
    }
}

void Pump::UpdateProfile(QString name, QString setting, float val){
    int index = FindIndex(name);
    if(index == -1){
        qDebug("no profile found matching that name (edit)");
        return;
    }
    if(setting == "basal rate"){
        profiles[index]->setBasalRates(val);
    }else if(setting == "carb ratio"){
        profiles[index]->setCarbRatio(val);
    }else if(setting == "correction factor"){
        profiles[index]->setCorrectionFactor(val);
    }else if(setting == "target"){
        profiles[index]->setTarget(val);
    }
}

int Pump::FindIndex(QString name){
    for(int i = 0; i < profiles.size(); i++){
        if(profiles[i]->getName() == name){
            return i;
        }
    }
    return -1; //error
}

void Pump::SelectProfile(QString name){
    int index = FindIndex(name);
    if(index == -1){
        qDebug("no profile found matching that name (select)");
        return;
    }
    active_profile = profiles[index];
}