#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include <QObject>
#include <QString>
#include "headers/Battery.h"

class StatusModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int batteryLevel READ getBatteryLevel WRITE setBatteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(bool batteryCharging READ isBatteryCharging WRITE setBatteryCharging NOTIFY batteryChargingChanged)
    Q_PROPERTY(int rightText READ getRightText WRITE setRightText NOTIFY rightTextChanged)

public:
    static StatusModel* getInstance();

    // battery related methods
    int getBatteryLevel() const;
    void setBatteryLevel(int level);

    bool isBatteryCharging() const;
    void setBatteryCharging(bool charging);

    // configure battery drain parameters
    void configureBatteryDrain(int interval, int amount);

    // right text methods (insulin on board)
    int getRightText() const;
    void setRightText(int level);

signals:
    void batteryLevelChanged(int newLevel);
    void batteryChargingChanged(bool charging);
    void rightTextChanged(int level);

private:
    StatusModel(QObject *parent = nullptr);
    static StatusModel* instance;

    // member variables
    Battery* battery;
    int rightText;
};

#endif // STATUSMODEL_H
