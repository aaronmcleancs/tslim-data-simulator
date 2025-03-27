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

    int getCurrentGlucose();
    int getCurrentCarbs();
    void updateGlucoseLevel(double glucose);

signals:
    void glucoseLevelUpdated(double glucose);

private:
    double currentGlucose = 0;
    double currentCarbs = 0;
};

#endif
