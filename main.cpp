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

    // get status model instance
    StatusModel* statusModel = StatusModel::getInstance();

    // create status bar
    StatusBar* statusBar = new StatusBar(nullptr);
    MainWindow* mainWindow = new MainWindow(statusBar);
    Pump* pump = mainWindow->getPump();


    // get auth manager instance
    AuthManager* authManager = AuthManager::getInstance();

    // monitor battery level changes
    QObject::connect(statusModel, &StatusModel::batteryLevelChanged,
                    [pump](int level) {
                        qDebug() << "Battery level: " << level << "%";

                        // Example of how to handle low battery warnings
                        if (level <= 20 && level > 10) {
                            qDebug() << "Warning: Battery low";
                            pump->recordAlert("Battery low", level);

                        } else if (level <= 10) {
                            qDebug() << "Warning: Battery critically low";
                            pump->recordAlert("Battery critically low!", level);


                        }
                    });

    // connect battery level to status bar
    QObject::connect(statusModel, &StatusModel::batteryLevelChanged,
                    [statusBar](int level) {
                        // Update battery level in UI
                        // Replace with your actual method to update battery in statusBar
                        // For example:
                        // statusBar->updateBatteryLevel(level);
                    });
    if(pump == nullptr){
        qDebug() << "null pump in main";
    }else{
        qDebug() << "pump created in main";
    }
    bolus* Bolus = mainWindow->getBolus();

    // set up connections
    QObject::connect(mainWindow, &MainWindow::bolusShift, [=]() {
        Bolus->show();
    });

    QObject::connect(Bolus, &bolus::mainShift, [=]() {
        Bolus->hide();
        mainWindow->navigateToRoute(Route::CONTENT);
    });

    QObject::connect(pump->getInsulinCartridge(), &InsulinCartridge::insulinLevelChanged,
                     [statusBar](int insulin) {
                         statusBar->onUnitsChanged(insulin);
                     });
    QObject::connect(Bolus, &bolus::BolusInitiated, [=]() {
        statusBar->setBolus(true);
    });

    // showing main window
    mainWindow->show();
    return a.exec();
}
