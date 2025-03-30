#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers/BolusCalculator.h"
#include "headers/Pump.h"
#include "statusbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bolusButton_clicked();
    void on_radioButton_clicked();

    void on_calculate_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_clicked();

    void createProfile();
    
    void updateHistoryTab();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Pump *pump;
    StatusBar *statusBar;

};
#endif // MAINWINDOW_H
