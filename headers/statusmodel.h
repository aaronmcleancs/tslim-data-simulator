#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include <QObject>
#include <QString>

class StatusModel : public QObject {
    Q_OBJECT

public:
    static StatusModel* getInstance();

    int getBatteryLevel() const { return batteryLevel; }
    int getRightText() const { return rightText; }

    void setBatteryLevel(int level);
    void setRightText(int level);

signals:
    void batteryLevelChanged(int level);
    void rightTextChanged(int level);

private:
    StatusModel(QObject *parent = nullptr);

    static StatusModel* instance;
    int batteryLevel;
    int rightText;
};

#endif
