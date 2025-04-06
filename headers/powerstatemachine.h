#ifndef POWERSTATEMACHINE_H
#define POWERSTATEMACHINE_H

#include <QObject>
#include <QDebug>
#include "headers/statusmodel.h"

class PowerStateMachine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool powered READ isPowered NOTIFY poweredChanged)

public:
    explicit PowerStateMachine(StatusModel* model, QObject *parent = nullptr);

    bool isPowered() const;
    int getMinBatteryLevel() const;

public slots:
    bool powerOn();
    void powerOff();
    void handleBatteryChange(int level);
signals:
    void poweredChanged(bool powered);
    void systemShuttingDown();

private:
    StatusModel* model;
    bool powered;
    int minBatteryLevel;
    void setPowered(bool newState);
};

#endif // POWERSTATEMACHINE_H
