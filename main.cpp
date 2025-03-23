#include <QApplication>
#include <QDebug>

//#include "headers/Pump.h"
//#include "headers/Battery.h"
//#include "headers/InsulinCartridge.h"
//#include "headers/CGM.h"
//#include "headers/UI.h"
//#include "headers/Profile.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Pump pump;

//    pump.getBattery()->setChargeLevel(100);
//    pump.getInsulinCartridge()->setRemainingInsulin(300);
//    pump.getCGM()->updateGlucoseLevel(5.5);

//    pump.getUI()->displayStatus("Pump initialized successfully!");

//    Profile* defaultProfile = new Profile("Default", &pump);
//    defaultProfile->setBasalRate(1.2);
//    defaultProfile->setCarbRatio(10.0);
//    defaultProfile->setCorrectionFactor(2.0);

//    pump.addProfile(defaultProfile);
//    pump.selectActiveProfile(defaultProfile);

//    qDebug() << "Active Profile:" << pump.getActiveProfile()->getName();

//    pump.deliverBolus(5.0);

    MainWindow w;
    w.show();

    return a.exec();
}
