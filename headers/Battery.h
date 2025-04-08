#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QTimer>

class Battery : public QObject {
    Q_OBJECT
    Q_PROPERTY(int chargeLevel READ getChargeLevel WRITE setChargeLevel NOTIFY chargeLevelChanged)
    Q_PROPERTY(bool charging READ isCharging WRITE setCharging NOTIFY chargingChanged)
    Q_PROPERTY(int drainInterval READ getDrainInterval WRITE setDrainInterval NOTIFY drainIntervalChanged)
    Q_PROPERTY(int drainAmount READ getDrainAmount WRITE setDrainAmount NOTIFY drainAmountChanged)

public:
    explicit Battery(QObject *parent = nullptr);

    // charge level methods
    int getChargeLevel() const;
    void setChargeLevel(int level);

    // charging state methods
    bool isCharging() const;
    void setCharging(bool charging);

    // drain configuration
    int getDrainInterval() const;
    void setDrainInterval(int interval);

    int getDrainAmount() const;
    void setDrainAmount(int amount);

public slots:
    void drainBattery();    // slot called by timer to drain battery
    void resetToFull();     // reset battery to 100%

signals:
    void chargeLevelChanged(int newLevel);
    void chargingChanged(bool charging);
    void drainIntervalChanged(int interval);
    void drainAmountChanged(int amount);

private:
    int chargeLevel;
    bool charging;
    QTimer* drainTimer;
    int drainInterval;
    int drainAmount;

    void updateDrainTimer(); // update timer based on charging state
};

#endif // BATTERY_H
