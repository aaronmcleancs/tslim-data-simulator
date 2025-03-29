#include <QApplication>
#include <QDebug>

#include "headers/Pump.h"
#include "headers/Battery.h"
#include "headers/InsulinCartridge.h"
#include "headers/CGM.h"
#include "headers/UI.h"
#include "headers/Profile.h"
#include "headers/BolusCalculator.h"
#include "headers/authmanager.h"
#include "headers/lockscreen.h"
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
    // Create and initialize the status bar
    StatusBar* statusBar = new StatusBar(nullptr);
    StatusBar* statusBar1 = new StatusBar(nullptr);

    // Get auth manager
    AuthManager* authManager = AuthManager::getInstance();
    LockScreen* lockScreen = new LockScreen(statusBar);
    // Create main window but don't show it yet
    MainWindow* mainWindow = new MainWindow(statusBar1);

    QObject::connect(lockScreen, &LockScreen::unlocked, [=]() {
        mainWindow->show();
        lockScreen->hide();
    });

    // Connect auth state changes
    QObject::connect(authManager, &AuthManager::authStateChanged, [=](bool authenticated) {
        if (authenticated) {
            mainWindow->show();
            lockScreen->hide();
        } else {
            mainWindow->hide();
            lockScreen->show();
        }
    });

    // Show the appropriate window based on auth state
    if (authManager->isAuthenticated()) {
        mainWindow->show();
    } else {
        lockScreen->show();
    }

    return a.exec();
}
