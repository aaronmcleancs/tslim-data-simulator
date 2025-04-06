#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "headers/authmanager.h"
#include "statusbar.h"
#include "headers/contentwidget.h"
#include "headers/lockscreen.h"
#include "headers/powerstatemachine.h"
#include "poweroff.h"
#include "bolus.h"

// Enum to represent different screens in the application
enum class Route {
    LOCK_SCREEN,
    CONTENT,
    BOLUS,
    SETTINGS,
    POWER_OFF
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(StatusBar *statusBar, QWidget *parent = nullptr);
    ~MainWindow();


    // Navigate to a specific route
    void navigateToRoute(Route route);
    Pump* getPump() const;
    ContentWidget* getContentWidget() const;
    bolus* getBolus() const;



    void setupBloodSugarGraph();
    void loadGraphData();
    void setBolusState(bool b){bolusState= b;}

    bool isPumpReady() const { return pump != nullptr; }

signals:
    void bolusShift();
    void fullyInitialized();

private slots:
    void onAuthStateChanged(bool authenticated);
    void onPowerStateChanged(bool power);

    void on_homeButton_clicked();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *routerLayout;
    StatusBar *statusBar;
    PowerStateMachine *powerStateMachine;

    LockScreen *lockScreen;
    ContentWidget *contentWidget;
    PowerOff *powerOffWidget;
    bolus *bolusWidget;
    QWidget *currentWidget;
    Pump* pump;
    bool bolusState;

};

#endif // MAINWINDOW_H
