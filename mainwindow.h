#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers/BolusCalculator.h"
#include "headers/Pump.h"
#include "headers/authmanager.h"
#include "headers/QCustomPlot.h"
#include "statusbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(StatusBar *sb, QWidget *parent = nullptr);
    ~MainWindow();


    void setupBloodSugarGraph();

signals:
    void bolusShift();


private slots:
    void onAuthStateChanged(bool authenticated);
    void on_bolusButton_clicked();
    void on_radioButton_clicked();

    void on_calculate_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_clicked();

    void createProfile();
    void editProfile();
    void selectProfile();
    void deleteProfile();
    
    void updateHistoryTab();
    void updateSettingsTab();
    void on_tabWidget_currentChanged(int index);

    void on_setting_pin_update_button_clicked();

private:
    Ui::MainWindow *ui;
    Pump *pump;
    StatusBar *statusBar;

};
#endif // MAINWINDOW_H
