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
#include "bolus.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StatusBar* statusBar = new StatusBar(nullptr);
    StatusBar* statusBar1 = new StatusBar(nullptr);

    AuthManager* authManager = AuthManager::getInstance();
    LockScreen* lockScreen = new LockScreen(statusBar);


    bolus* Bolus = new bolus(nullptr);

    MainWindow* mainWindow = new MainWindow(statusBar1);

    QObject::connect(lockScreen, &LockScreen::unlocked, [=]() {
        mainWindow->show();
        lockScreen->hide();
    });
    QObject::connect(mainWindow, &MainWindow::bolusShift, [=]() {
        mainWindow->hide();
        Bolus->show();
    });
    QObject::connect(Bolus, &bolus::mainShift, [=]() {
        mainWindow->show();
        Bolus->hide();
    });

    QObject::connect(authManager, &AuthManager::authStateChanged, [=](bool authenticated) {
        if (authenticated) {
            mainWindow->show();
            lockScreen->hide();
        } else {
            mainWindow->hide();
            lockScreen->show();
        }
    });

    if (authManager->isAuthenticated()) {
        mainWindow->show();
    } else {
        lockScreen->show();
    }

    return a.exec();
}
