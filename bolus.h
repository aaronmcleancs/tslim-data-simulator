#ifndef BOLUS_H
#define BOLUS_H

#include <QDialog>
#include "headers/Profile.h"
#include "headers/Pump.h"
#include "headers/ControlIQ.h"
#include "headers/contentwidget.h"

namespace Ui {
class bolus;
}

class bolus : public QDialog
{
    Q_OBJECT

public:
    explicit bolus(Pump* pump, QWidget *parent = nullptr);
    ~bolus();

signals:
    void mainShift();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_radioButton_clicked();

private:
    Ui::bolus *ui;
    Pump *pump;
    ControlIQ *controlIQ;
};

#endif // BOLUS_H
