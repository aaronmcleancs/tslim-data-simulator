#include "headers/Profile.h"

Profile::Profile(QString n, float br, float cr, float cf, float t, QObject *parent = nullptr){
    name = n;
    basal_rates = br;
    carb_ratio = cr;
    correction_factor = cf;
    target = t;
}

Profile::~Profile(){

}
