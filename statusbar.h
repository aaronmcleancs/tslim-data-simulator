#include "headers/statusmodel.h"

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QFrame>

namespace Ui {
class StatusBar;
}

class StatusBar : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);
    ~StatusBar();
    static StatusBar* getInstance();
    void setBolus(bool b);
public slots:
    void onUnitsChanged(int level);
private:
    Ui::StatusBar *ui;
    static StatusBar* instance;

private slots:
    void onBatteryLevelChanged(int level);

};

#endif
