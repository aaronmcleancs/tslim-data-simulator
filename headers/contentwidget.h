#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include "headers/BolusCalculator.h"
#include "headers/Pump.h"
#include "headers/QCustomPlot.h"
#include "statusbar.h"

namespace Ui {
class ContentWidget;
}

class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = nullptr);
    ~ContentWidget();

    void setupBloodSugarGraph();
    void loadGraphData();
    void updateHistoryTab();
    void updateSettingsTab();
    Pump* getPump() const;

public slots:
    void on_tabWidget_currentChanged(int index);
    void on_setting_pin_update_button_clicked();
    void createProfile();
    void editProfile();
    void selectProfile();
    void deleteProfile();

    void setBolus(bool b){bolus = b;}

signals:
    void openBolus();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ContentWidget *ui;
    Pump *pump;
    bool bolus = false;
};

#endif // CONTENTWIDGET_H
