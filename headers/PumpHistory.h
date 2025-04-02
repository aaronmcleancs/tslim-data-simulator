#ifndef PUMPHISTORY_H
#define PUMPHISTORY_H

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
#include <QDebug>


struct PumpEvent {
    QDateTime timestamp;
    QString eventType;
    
    virtual QJsonObject toJson() const;
    static PumpEvent fromJson(const QJsonObject &json);
};

struct BasalRateEvent : public PumpEvent {
    double rate; 
    
    QJsonObject toJson() const override;
    static BasalRateEvent fromJson(const QJsonObject &json);
};

struct BolusEvent : public PumpEvent {
    double amount; 
    QString bolusType; 
    int duration; 
    double carbInput; 
    double bgInput; 
    
    QJsonObject toJson() const override;
    static BolusEvent fromJson(const QJsonObject &json);
};

struct AlertEvent : public PumpEvent {
    QString alertType; 
    double bgValue; 
    bool acknowledged; 
    
    QJsonObject toJson() const override;
    static AlertEvent fromJson(const QJsonObject &json);
};

struct SettingChangeEvent : public PumpEvent {
    QString settingName; 
    QString oldValue; 
    QString newValue; 
    
    QJsonObject toJson() const override;
    static SettingChangeEvent fromJson(const QJsonObject &json);
};


struct StatusEvent : public PumpEvent {
    QString statusType; 
    QString statusDetails; 
    
    QJsonObject toJson() const override;
    static StatusEvent fromJson(const QJsonObject &json);
};

class PumpHistory : public QObject
{
    Q_OBJECT
public:
    explicit PumpHistory(const QString &profileName, QObject *parent = nullptr);
    ~PumpHistory();
    
    
    void addBasalRateEvent(const BasalRateEvent &event);
    void addBolusEvent(const BolusEvent &event);
    void addAlertEvent(const AlertEvent &event);
    void addSettingChangeEvent(const SettingChangeEvent &event);
    void addStatusEvent(const StatusEvent &event);
    
    
    QVector<BasalRateEvent> getBasalRateEvents(const QDateTime &start = QDateTime(), 
                                             const QDateTime &end = QDateTime()) const;
    QVector<BolusEvent> getBolusEvents(const QDateTime &start = QDateTime(), 
                                      const QDateTime &end = QDateTime()) const;
    QVector<AlertEvent> getAlertEvents(const QDateTime &start = QDateTime(), 
                                      const QDateTime &end = QDateTime()) const;
    QVector<SettingChangeEvent> getSettingChangeEvents(const QDateTime &start = QDateTime(), 
                                                     const QDateTime &end = QDateTime()) const;
    QVector<StatusEvent> getStatusEvents(const QDateTime &start = QDateTime(), 
                                        const QDateTime &end = QDateTime()) const;
    
    
    BolusEvent getLastBolus() const;
    BasalRateEvent getCurrentBasalRate() const;
    QVector<PumpEvent> getRecentEvents(int count) const;
    
    
    bool saveHistory();
    bool loadHistory();
    
private:
    QString profileName;
    
    QVector<BasalRateEvent> basalRateEvents;
    QVector<BolusEvent> bolusEvents;
    QVector<AlertEvent> alertEvents;
    QVector<SettingChangeEvent> settingChangeEvents;
    QVector<StatusEvent> statusEvents;
    
    static const QString PUMP_HISTORY_KEY;
};

#endif 