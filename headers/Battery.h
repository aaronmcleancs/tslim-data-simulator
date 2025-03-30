#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class Battery : public QObject
{
    Q_OBJECT
public:
    explicit Battery(QObject *parent = nullptr);

    int getChargeLevel() const;
    void setChargeLevel(int level);

signals:
    void chargeLevelChanged(int newLevel);

private:
    int chargeLevel;
};

#endif