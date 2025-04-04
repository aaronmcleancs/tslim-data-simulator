#ifndef BOLUS_H
#define BOLUS_H

#include <QWidget>

namespace Ui {
class bolus;
}

class bolus : public QWidget
{
    Q_OBJECT

public:
    explicit bolus(QWidget *parent = nullptr);
    ~bolus();

signals:
    void mainShift();
    void BolusInitiated();

private slots:


    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_InitiateBolus_clicked();

private:
    Ui::bolus *ui;
    double totalBolus;
    bool checked = false;
};

#endif // BOLUS_H
