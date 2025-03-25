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
    ~Profile();

    QString getName() const;
    void setName(const QString &name);

    double getBasalRate() const;
    void setBasalRate(double rate);

    double getCarbRatio() const;
    void setCarbRatio(double ratio);

    double getCorrectionFactor() const;
    void setCorrectionFactor(double factor);
    
    void addGlucoseReading(const GlucoseReading &reading);
    QVector<GlucoseReading> getGlucoseReadings(const QDateTime &start = QDateTime(), 
                                               const QDateTime &end = QDateTime()) const;
    
    void addInsulinDose(const InsulinDose &dose);
    QVector<InsulinDose> getInsulinDoses(const QDateTime &start = QDateTime(), 
                                         const QDateTime &end = QDateTime()) const;
    
    void setTargetGlucoseRange(double min, double max);
    QPair<double, double> getTargetGlucoseRange() const;

    bool saveProfile();
    bool loadProfile(const QString &name);
    
    static QStringList getAvailableProfiles();
    static bool deleteProfile(const QString &name);
    static bool createProfile(const QString &name);

private:
    QString profileName;
    double basalRate;
    double carbRatio;
    double correctionFactor;
    
    double targetGlucoseMin;
    double targetGlucoseMax;
    
    QVector<GlucoseReading> glucoseReadings;
    QVector<InsulinDose> insulinDoses;
    
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);
    
    bool saveHistoricalData();
    bool loadHistoricalData();
    
    static const QString PROFILES_KEY;
};

#endif