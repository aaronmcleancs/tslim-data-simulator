#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "headers/authmanager.h"
#include "statusbar.h"
#include "headers/contentwidget.h"
#include "headers/lockscreen.h"

// Enum to represent different screens in the application
enum class Route {
    LOCK_SCREEN,
    CONTENT,
    BOLUS,
    SETTINGS
    // Add more routes as needed
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


    void setupBloodSugarGraph();
    void loadGraphData();
    void setBolusState(bool b){bolusState= b;}


signals:
    void bolusShift();
    void bolusStopped();

private slots:
    void onAuthStateChanged(bool authenticated);

private:
    Ui::MainWindow *ui;
    QVBoxLayout *routerLayout;
    StatusBar *statusBar;

    LockScreen *lockScreen;
    ContentWidget *contentWidget;
    QWidget *currentWidget;

    bool bolusState;


};

#endif // MAINWINDOW_H
