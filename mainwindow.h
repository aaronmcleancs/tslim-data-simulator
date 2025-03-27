#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers/BolusCalculator.h"

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
