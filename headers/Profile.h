#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QPair>
#include <QVector>


struct GlucoseReading {
    QDateTime timestamp;
    double value; // mmol/L
    
    QJsonObject toJson() const;
    static GlucoseReading fromJson(const QJsonObject &json);
};

struct InsulinDose {
    QDateTime timestamp;
    double amount; 
    QString type;
    
    QJsonObject toJson() const;
    static InsulinDose fromJson(const QJsonObject &json);
};

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(const QString &name, QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &name);

    double getBasalRate() const;
    void setBasalRate(double rate) {basal_rates = rate;}

    double getCarbRatio() const;
    void setCarbRatio(double ratio) {carb_ratio = ratio;}

    double getCorrectionFactor() const;
    void setCorrectionFactor(double factor);

private:
    QString profileName;
    double basalRate;
    double carbRatio;
    double correctionFactor;
};

#endif