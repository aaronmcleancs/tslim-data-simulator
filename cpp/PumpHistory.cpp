#include "headers/PumpHistory.h"

const QString PumpHistory::PUMP_HISTORY_KEY = "PumpHistory";

QJsonObject PumpEvent::toJson() const {
    QJsonObject json;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    json["eventType"] = eventType;
    return json;
}

PumpEvent PumpEvent::fromJson(const QJsonObject &json) {
    PumpEvent event;
    if (json.contains("timestamp")) {
        event.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    }
    if (json.contains("eventType")) {
        event.eventType = json["eventType"].toString();
    }
    return event;
}

QJsonObject BasalRateEvent::toJson() const {
    QJsonObject json = PumpEvent::toJson();
    json["rate"] = rate;
    return json;
}

BasalRateEvent BasalRateEvent::fromJson(const QJsonObject &json) {
    BasalRateEvent event;
    PumpEvent baseEvent = PumpEvent::fromJson(json);
    event.timestamp = baseEvent.timestamp;
    event.eventType = baseEvent.eventType;
    
    if (json.contains("rate")) {
        event.rate = json["rate"].toDouble();
    }
    return event;
}

QJsonObject BolusEvent::toJson() const {
    QJsonObject json = PumpEvent::toJson();
    json["amount"] = amount;
    json["bolusType"] = bolusType;
    json["duration"] = duration;
    json["carbInput"] = carbInput;
    json["bgInput"] = bgInput;
    return json;
}

BolusEvent BolusEvent::fromJson(const QJsonObject &json) {
    BolusEvent event;
    PumpEvent baseEvent = PumpEvent::fromJson(json);
    event.timestamp = baseEvent.timestamp;
    event.eventType = baseEvent.eventType;
    
    if (json.contains("amount")) {
        event.amount = json["amount"].toDouble();
    }
    if (json.contains("bolusType")) {
        event.bolusType = json["bolusType"].toString();
    }
    if (json.contains("duration")) {
        event.duration = json["duration"].toInt();
    }
    if (json.contains("carbInput")) {
        event.carbInput = json["carbInput"].toDouble();
    }
    if (json.contains("bgInput")) {
        event.bgInput = json["bgInput"].toDouble();
    }
    return event;
}


QJsonObject AlertEvent::toJson() const {
    QJsonObject json = PumpEvent::toJson();
    json["alertType"] = alertType;
    json["bgValue"] = bgValue;
    json["acknowledged"] = acknowledged;
    return json;
}

AlertEvent AlertEvent::fromJson(const QJsonObject &json) {
    AlertEvent event;
    PumpEvent baseEvent = PumpEvent::fromJson(json);
    event.timestamp = baseEvent.timestamp;
    event.eventType = baseEvent.eventType;
    
    if (json.contains("alertType")) {
        event.alertType = json["alertType"].toString();
    }
    if (json.contains("bgValue")) {
        event.bgValue = json["bgValue"].toDouble();
    }
    if (json.contains("acknowledged")) {
        event.acknowledged = json["acknowledged"].toBool();
    }
    return event;
}

QJsonObject SettingChangeEvent::toJson() const {
    QJsonObject json = PumpEvent::toJson();
    json["settingName"] = settingName;
    json["oldValue"] = oldValue;
    json["newValue"] = newValue;
    return json;
}

SettingChangeEvent SettingChangeEvent::fromJson(const QJsonObject &json) {
    SettingChangeEvent event;
    PumpEvent baseEvent = PumpEvent::fromJson(json);
    event.timestamp = baseEvent.timestamp;
    event.eventType = baseEvent.eventType;
    
    if (json.contains("settingName")) {
        event.settingName = json["settingName"].toString();
    }
    if (json.contains("oldValue")) {
        event.oldValue = json["oldValue"].toString();
    }
    if (json.contains("newValue")) {
        event.newValue = json["newValue"].toString();
    }
    return event;
}


QJsonObject StatusEvent::toJson() const {
    QJsonObject json = PumpEvent::toJson();
    json["statusType"] = statusType;
    json["statusDetails"] = statusDetails;
    return json;
}

StatusEvent StatusEvent::fromJson(const QJsonObject &json) {
    StatusEvent event;
    PumpEvent baseEvent = PumpEvent::fromJson(json);
    event.timestamp = baseEvent.timestamp;
    event.eventType = baseEvent.eventType;
    
    if (json.contains("statusType")) {
        event.statusType = json["statusType"].toString();
    }
    if (json.contains("statusDetails")) {
        event.statusDetails = json["statusDetails"].toString();
    }
    return event;
}

PumpHistory::PumpHistory(const QString &profileName, QObject *parent) : QObject(parent),
    profileName(profileName)
{
    
    basalRateEvents.clear();
    bolusEvents.clear();
    alertEvents.clear();
    settingChangeEvents.clear();
    statusEvents.clear();
    
    
    if (!profileName.isEmpty()) {
        loadHistory();
    }
}

PumpHistory::~PumpHistory() {
    saveHistory();
}

void PumpHistory::addBasalRateEvent(const BasalRateEvent &event) {
    basalRateEvents.append(event);
    saveHistory();
}

void PumpHistory::addBolusEvent(const BolusEvent &event) {
    bolusEvents.append(event);
    saveHistory();
}

void PumpHistory::addAlertEvent(const AlertEvent &event) {
    alertEvents.append(event);
    saveHistory();
}

void PumpHistory::addSettingChangeEvent(const SettingChangeEvent &event) {
    settingChangeEvents.append(event);
    saveHistory();
}

void PumpHistory::addStatusEvent(const StatusEvent &event) {
    statusEvents.append(event);
    saveHistory();
}

QVector<BasalRateEvent> PumpHistory::getBasalRateEvents(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return basalRateEvents;
    }

    QVector<BasalRateEvent> filteredEvents;
    for (const BasalRateEvent &event : basalRateEvents) {
        bool include = true;
        if (start.isValid() && event.timestamp < start) {
            include = false;
        }
        if (end.isValid() && event.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredEvents.append(event);
        }
    }
    return filteredEvents;
}

QVector<BolusEvent> PumpHistory::getBolusEvents(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return bolusEvents;
    }

    QVector<BolusEvent> filteredEvents;
    for (const BolusEvent &event : bolusEvents) {
        bool include = true;
        if (start.isValid() && event.timestamp < start) {
            include = false;
        }
        if (end.isValid() && event.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredEvents.append(event);
        }
    }
    return filteredEvents;
}

QVector<AlertEvent> PumpHistory::getAlertEvents(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return alertEvents;
    }

    QVector<AlertEvent> filteredEvents;
    for (const AlertEvent &event : alertEvents) {
        bool include = true;
        if (start.isValid() && event.timestamp < start) {
            include = false;
        }
        if (end.isValid() && event.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredEvents.append(event);
        }
    }
    return filteredEvents;
}

QVector<SettingChangeEvent> PumpHistory::getSettingChangeEvents(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return settingChangeEvents;
    }

    QVector<SettingChangeEvent> filteredEvents;
    for (const SettingChangeEvent &event : settingChangeEvents) {
        bool include = true;
        if (start.isValid() && event.timestamp < start) {
            include = false;
        }
        if (end.isValid() && event.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredEvents.append(event);
        }
    }
    return filteredEvents;
}

QVector<StatusEvent> PumpHistory::getStatusEvents(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return statusEvents;
    }

    QVector<StatusEvent> filteredEvents;
    for (const StatusEvent &event : statusEvents) {
        bool include = true;
        if (start.isValid() && event.timestamp < start) {
            include = false;
        }
        if (end.isValid() && event.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredEvents.append(event);
        }
    }
    return filteredEvents;
}

BolusEvent PumpHistory::getLastBolus() const {
    if (bolusEvents.isEmpty()) {
        return BolusEvent();
    }
    
    
    BolusEvent lastBolus = bolusEvents.first();
    for (const BolusEvent &event : bolusEvents) {
        if (event.timestamp > lastBolus.timestamp) {
            lastBolus = event;
        }
    }
    return lastBolus;
}

BasalRateEvent PumpHistory::getCurrentBasalRate() const {
    if (basalRateEvents.isEmpty()) {
        return BasalRateEvent();
    }
    
    
    BasalRateEvent currentRate = basalRateEvents.first();
    for (const BasalRateEvent &event : basalRateEvents) {
        if (event.timestamp > currentRate.timestamp) {
            currentRate = event;
        }
    }
    return currentRate;
}

QVector<PumpEvent> PumpHistory::getRecentEvents(int count) const {
    
    QVector<PumpEvent> allEvents;
    
    
    for (const BasalRateEvent &event : basalRateEvents) {
        PumpEvent baseEvent;
        baseEvent.timestamp = event.timestamp;
        baseEvent.eventType = event.eventType;
        allEvents.append(baseEvent);
    }
    
    for (const BolusEvent &event : bolusEvents) {
        PumpEvent baseEvent;
        baseEvent.timestamp = event.timestamp;
        baseEvent.eventType = event.eventType;
        allEvents.append(baseEvent);
    }
    
    for (const AlertEvent &event : alertEvents) {
        PumpEvent baseEvent;
        baseEvent.timestamp = event.timestamp;
        baseEvent.eventType = event.eventType;
        allEvents.append(baseEvent);
    }
    
    for (const SettingChangeEvent &event : settingChangeEvents) {
        PumpEvent baseEvent;
        baseEvent.timestamp = event.timestamp;
        baseEvent.eventType = event.eventType;
        allEvents.append(baseEvent);
    }
    
    for (const StatusEvent &event : statusEvents) {
        PumpEvent baseEvent;
        baseEvent.timestamp = event.timestamp;
        baseEvent.eventType = event.eventType;
        allEvents.append(baseEvent);
    }
    
    
    std::sort(allEvents.begin(), allEvents.end(), [](const PumpEvent &a, const PumpEvent &b) {
        return a.timestamp > b.timestamp;
    });
    
    
    if (count > 0 && count < allEvents.size()) {
        return allEvents.mid(0, count);
    }
    
    return allEvents;
}

bool PumpHistory::saveHistory() {
    if (profileName.isEmpty()) {
        return false;
    }
    
    QSettings settings("TandemPump", "COMP3004A4");
    
    
    QJsonArray basalArray;
    for (const BasalRateEvent &event : basalRateEvents) {
        basalArray.append(event.toJson());
    }
    QJsonDocument basalDoc(basalArray);
    settings.setValue("BasalRateEvents_" + profileName, basalDoc.toJson(QJsonDocument::Compact));
    
    
    QJsonArray bolusArray;
    for (const BolusEvent &event : bolusEvents) {
        bolusArray.append(event.toJson());
    }
    QJsonDocument bolusDoc(bolusArray);
    settings.setValue("BolusEvents_" + profileName, bolusDoc.toJson(QJsonDocument::Compact));
    
    
    QJsonArray alertArray;
    for (const AlertEvent &event : alertEvents) {
        alertArray.append(event.toJson());
    }
    QJsonDocument alertDoc(alertArray);
    settings.setValue("AlertEvents_" + profileName, alertDoc.toJson(QJsonDocument::Compact));
    
    
    QJsonArray settingArray;
    for (const SettingChangeEvent &event : settingChangeEvents) {
        settingArray.append(event.toJson());
    }
    QJsonDocument settingDoc(settingArray);
    settings.setValue("SettingChangeEvents_" + profileName, settingDoc.toJson(QJsonDocument::Compact));
    
    
    QJsonArray statusArray;
    for (const StatusEvent &event : statusEvents) {
        statusArray.append(event.toJson());
    }
    QJsonDocument statusDoc(statusArray);
    settings.setValue("StatusEvents_" + profileName, statusDoc.toJson(QJsonDocument::Compact));
    
    settings.sync();
    return settings.status() == QSettings::NoError;
}

bool PumpHistory::loadHistory() {
    if (profileName.isEmpty()) {
        qDebug() << "PumpHistory: Cannot load history for empty profile name";
        return false;
    }
    
    QSettings settings("TandemPump", "COMP3004A4");
    
    
    bool historyExists = false;
    historyExists |= settings.contains("BasalRateEvents_" + profileName);
    historyExists |= settings.contains("BolusEvents_" + profileName);
    historyExists |= settings.contains("AlertEvents_" + profileName);
    historyExists |= settings.contains("SettingChangeEvents_" + profileName);
    historyExists |= settings.contains("StatusEvents_" + profileName);
    
    if (!historyExists) {
        qDebug() << "PumpHistory: No history data found for profile" << profileName;
        return true; 
    }
    
    
    QString basalString = settings.value("BasalRateEvents_" + profileName).toString();
    if (!basalString.isEmpty()) {
        QJsonDocument basalDoc = QJsonDocument::fromJson(basalString.toUtf8());
        if (!basalDoc.isNull() && basalDoc.isArray()) {
            QJsonArray basalArray = basalDoc.array();
            basalRateEvents.clear();
            
            for (const QJsonValue &value : basalArray) {
                if (value.isObject()) {
                    basalRateEvents.append(BasalRateEvent::fromJson(value.toObject()));
                }
            }
        }
    }
    
    
    QString bolusString = settings.value("BolusEvents_" + profileName).toString();
    if (!bolusString.isEmpty()) {
        QJsonDocument bolusDoc = QJsonDocument::fromJson(bolusString.toUtf8());
        if (!bolusDoc.isNull() && bolusDoc.isArray()) {
            QJsonArray bolusArray = bolusDoc.array();
            bolusEvents.clear();
            
            for (const QJsonValue &value : bolusArray) {
                if (value.isObject()) {
                    bolusEvents.append(BolusEvent::fromJson(value.toObject()));
                }
            }
        }
    }
    
    
    QString alertString = settings.value("AlertEvents_" + profileName).toString();
    if (!alertString.isEmpty()) {
        QJsonDocument alertDoc = QJsonDocument::fromJson(alertString.toUtf8());
        if (!alertDoc.isNull() && alertDoc.isArray()) {
            QJsonArray alertArray = alertDoc.array();
            alertEvents.clear();
            
            for (const QJsonValue &value : alertArray) {
                if (value.isObject()) {
                    alertEvents.append(AlertEvent::fromJson(value.toObject()));
                }
            }
        }
    }
    
    
    QString settingString = settings.value("SettingChangeEvents_" + profileName).toString();
    if (!settingString.isEmpty()) {
        QJsonDocument settingDoc = QJsonDocument::fromJson(settingString.toUtf8());
        if (!settingDoc.isNull() && settingDoc.isArray()) {
            QJsonArray settingArray = settingDoc.array();
            settingChangeEvents.clear();
            
            for (const QJsonValue &value : settingArray) {
                if (value.isObject()) {
                    settingChangeEvents.append(SettingChangeEvent::fromJson(value.toObject()));
                }
            }
        }
    }
    
    
    QString statusString = settings.value("StatusEvents_" + profileName).toString();
    if (!statusString.isEmpty()) {
        QJsonDocument statusDoc = QJsonDocument::fromJson(statusString.toUtf8());
        if (!statusDoc.isNull() && statusDoc.isArray()) {
            QJsonArray statusArray = statusDoc.array();
            statusEvents.clear();
            
            for (const QJsonValue &value : statusArray) {
                if (value.isObject()) {
                    statusEvents.append(StatusEvent::fromJson(value.toObject()));
                }
            }
        }
    }
    
    qDebug() << "PumpHistory: Loaded" 
             << basalRateEvents.size() << "basal events," 
             << bolusEvents.size() << "bolus events,"
             << alertEvents.size() << "alert events,"
             << settingChangeEvents.size() << "setting changes,"
             << statusEvents.size() << "status events for profile" << profileName;
    
    return true;
} 