#ifndef BOLUS_H
#define BOLUS_H

#include <QDialog>
#include "headers/Profile.h"
#include "headers/Pump.h"
#include "headers/ControlIQ.h"
#include "headers/contentwidget.h"
#include <QWidget>

namespace Ui {
class bolus;
}

class bolus : public QWidget
{
    Q_OBJECT

public:
    explicit bolus(Pump* pump, QWidget *parent = nullptr);
    ~bolus();
    bool bolusState = false;

signals:
    void mainShift();
    void BolusInitiated();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_checkBox_2_stateChanged(int arg1);

private:
    Ui::bolus *ui;
    Pump *pump;
    ControlIQ *controlIQ;

//    void on_pushButton_4_clicked();

//    void on_pushButton_3_clicked();

//    void on_pushButton_2_clicked();

//    void on_pushButton_clicked();

    void on_InitiateBolus_clicked();

private:
    double totalBolus;
    bool checked = false;
};

#endif // BOLUS_H
