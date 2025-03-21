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
    explicit Profile(const QString &name, QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &name);

    double getBasalRate() const;
    void setBasalRate(double rate);

    double getCarbRatio() const;
    void setCarbRatio(double ratio);

    double getCorrectionFactor() const;
    void setCorrectionFactor(double factor);

private:
    QString profileName;
    double basalRate;
    double carbRatio;
    double correctionFactor;
};

#endif