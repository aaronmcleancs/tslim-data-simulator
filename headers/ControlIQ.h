#ifndef CONTROLIQ_H
#define CONTROLIQ_H

#include <QObject>
#include "Pump.h"
#include "Profile.h"

class ControlIQ : public QObject
{
    Q_OBJECT
public:
    explicit ControlIQ(Pump* pump, QObject *parent = nullptr);

public slots:
    void evaluate(double glucose);

private:
    Pump* pump;
    void adjustInsulinDelivery(double glucose);
    void triggerSafetyCheck(double glucose);
};

#endif // CONTROLIQ_H
