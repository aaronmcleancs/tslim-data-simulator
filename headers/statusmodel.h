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

    // Battery related methods
    int getBatteryLevel() const;
    void setBatteryLevel(int level);

    bool isBatteryCharging() const;
    void setBatteryCharging(bool charging);

    // Configure battery drain parameters
    void configureBatteryDrain(int interval, int amount);

    // Right text methods
    int getRightText() const;
    void setRightText(int level);

signals:
    void batteryLevelChanged(int newLevel);
    void batteryChargingChanged(bool charging);
    void rightTextChanged(int level);

private:
    StatusModel(QObject *parent = nullptr);
    static StatusModel* instance;

    // Member variables
    Battery* battery;
    int rightText;
};

#endif // STATUSMODEL_H
