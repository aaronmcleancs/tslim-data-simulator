#ifndef CGM_H
#define CGM_H

#include <QObject>

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class CGM : public QObject
{
    Q_OBJECT
public:
    explicit CGM(QObject *parent = nullptr);

    double getCurrentGlucose() const;
    void updateGlucoseLevel(double glucose);

signals:
    void glucoseLevelUpdated(double glucose);

private:
    double currentGlucose;
};

#endif