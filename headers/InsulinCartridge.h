#ifndef INSULINCARTRIDGE_H
#define INSULINCARTRIDGE_H

#include <QObject>
#include "statusbar.h"
class Pump;
class InsulinCartridge : public QObject
{
    Q_OBJECT
public:
    explicit InsulinCartridge(Pump* pump, QObject *parent = nullptr);

    int getRemainingInsulin() const;
    void setRemainingInsulin(int units);

signals:
    void insulinLevelChanged(int newLevel);

private:
    Pump* pump;
    int remainingInsulin;
};

#endif
