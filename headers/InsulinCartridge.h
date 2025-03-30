#ifndef INSULINCARTRIDGE_H
#define INSULINCARTRIDGE_H

#include <QObject>

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class InsulinCartridge : public QObject
{
    Q_OBJECT
public:
    explicit InsulinCartridge(QObject *parent = nullptr);

    int getRemainingInsulin() const;
    void setRemainingInsulin(int units);

signals:
    void insulinLevelChanged(int newLevel);

private:
    int remainingInsulin;
};

#endif