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
#include "headers/statusmodel.h"
#include "mainwindow.h"
#include "bolus.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Get status model instance
    StatusModel* statusModel = StatusModel::getInstance();

    // Create status bar
    StatusBar* statusBar = new StatusBar(nullptr);
    MainWindow* mainWindow = new MainWindow(statusBar);

    // Get auth manager instance
    AuthManager* authManager = AuthManager::getInstance();

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

    // Connect battery level to status bar
    QObject::connect(statusModel, &StatusModel::batteryLevelChanged,
                    [statusBar](int level) {
                        // Update battery level in UI
                        // Replace with your actual method to update battery in statusBar
                        // For example:
                        // statusBar->updateBatteryLevel(level);
                    });
    Pump* pump = mainWindow->getPump();
    if(pump == nullptr){
        qDebug() << "null pump in main";
    }else{
        qDebug() << "pump created in main";
    }
    bolus* Bolus = new bolus(pump, nullptr);

    // Set up connections
    QObject::connect(mainWindow, &MainWindow::bolusShift, [=]() {
        Bolus->show();
    });

    QObject::connect(Bolus, &bolus::mainShift, [=]() {
        Bolus->hide();
        mainWindow->navigateToRoute(Route::CONTENT);
    });

    QObject::connect(Bolus, &bolus::BolusInitiated, [=]() {
        statusBar->setBolus(true);
    });

    // Show the main window
    mainWindow->show();
    return a.exec();
}
