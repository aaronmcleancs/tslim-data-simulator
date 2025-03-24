#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QString>

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(const QString &name, double br, double cr, double cf, double t, QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &name);

    double getBasalRate() const;
    void setBasalRate(double rate) {basal_rates = rate;}

    double getCarbRatio() const;
    void setCarbRatio(double ratio) {carb_ratio = ratio;}

    double getCorrectionFactor() const;
    void setCorrectionFactor(double factor) {correction_factor = factor;}

    double getTarget() const;
    void setTarget(double tar) {target = tar;}

private:
    QString profileName;
    double basalRate;
    double carbRatio;
    double correctionFactor;
    double target;
};

#endif