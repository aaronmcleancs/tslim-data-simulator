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
#include "headers/statusmodel.h"  // Added the StatusModel header
#include "mainwindow.h"
#include "bolus.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Get status model instance
    StatusModel* statusModel = StatusModel::getInstance();

    // Battery is already initialized with defaults in the Battery constructor:
    // - Level: 100%
    // - Drain rate: 1% per minute
    // - Charging state: false (discharging)

    // Monitor battery level changes
    QObject::connect(statusModel, &StatusModel::batteryLevelChanged,
                    [](int level) {
                        qDebug() << "Battery level: " << level << "%";

                        // Example of how to handle low battery warnings
                        if (level <= 20 && level > 10) {
                            qDebug() << "Warning: Battery low";
                        } else if (level <= 10) {
                            qDebug() << "Warning: Battery critically low";
                        }
                    });

    // Your existing code
    StatusBar* statusBar = new StatusBar(nullptr);
    StatusBar* statusBar1 = new StatusBar(nullptr);

    AuthManager* authManager = AuthManager::getInstance();
    LockScreen* lockScreen = new LockScreen(statusBar);

    bolus* Bolus = new bolus(nullptr);

    MainWindow* mainWindow = new MainWindow(statusBar1);

    // Connect battery level to your status bars
    // Assuming your StatusBar class has a method to update battery level
    QObject::connect(statusModel, &StatusModel::batteryLevelChanged,
                    [statusBar, statusBar1](int level) {
                        // Update battery level in UI
                        // Replace with your actual method to update battery in statusBar
                        // For example:
                        // statusBar->updateBatteryLevel(level);
                        // statusBar1->updateBatteryLevel(level);
                    });

    // Your existing connections
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

    // Add charging toggles for demo/testing purposes
    // These would typically be connected to actual charger detection
    // For example, you might add buttons in your UI to simulate plugging/unplugging

    // Example: You could add methods to your MainWindow class:
    // QObject::connect(mainWindow, &MainWindow::chargerConnected, [statusModel]() {
    //     statusModel->setBatteryCharging(true);
    // });
    //
    // QObject::connect(mainWindow, &MainWindow::chargerDisconnected, [statusModel]() {
    //     statusModel->setBatteryCharging(false);
    // });

    if (authManager->isAuthenticated()) {
        mainWindow->show();
    } else {
        lockScreen->show();
    }

    return a.exec();
}
