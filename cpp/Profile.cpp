#include "headers/Profile.h"

const QString Profile::PROFILES_KEY = "Profiles";

QJsonObject GlucoseReading::toJson() const {
    QJsonObject json;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    json["value"] = value;
    return json;
}

GlucoseReading GlucoseReading::fromJson(const QJsonObject &json) {
    GlucoseReading reading;
    if (json.contains("timestamp")) {
        reading.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    }
    if (json.contains("value")) {
        reading.value = json["value"].toDouble();
    }
    return reading;
}

QJsonObject InsulinDose::toJson() const {
    QJsonObject json;
    json["timestamp"] = timestamp.toString(Qt::ISODate);
    json["amount"] = amount;
    json["type"] = type;
    return json;
}

InsulinDose InsulinDose::fromJson(const QJsonObject &json) {
    InsulinDose dose;
    if (json.contains("timestamp")) {
        dose.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    }
    if (json.contains("amount")) {
        dose.amount = json["amount"].toDouble();
    }
    if (json.contains("type")) {
        dose.type = json["type"].toString();
    }
    return dose;
}

Profile::Profile(const QString &name, QObject *parent) : QObject(parent),
    profileName(name),
    basalRate(0.0),
    carbRatio(0.0),
    correctionFactor(0.0),
    targetGlucoseMin(3.9),
    targetGlucoseMax(10.0)
{

    if (!name.isEmpty()) {
        loadProfile(name);
        loadHistoricalData();
    }

}

Profile::~Profile() {
    saveProfile();
    saveHistoricalData();
}

QString Profile::getName() const {
    return profileName;
}

void Profile::setName(const QString &name) {
    profileName = name;
}

double Profile::getBasalRate() const {
    return basalRate;
}

void Profile::setBasalRate(double rate) {
    basalRate = rate;
}

double Profile::getCarbRatio() const {
    return carbRatio;
}

void Profile::setCarbRatio(double ratio) {
    carbRatio = ratio;
}

double Profile::getCorrectionFactor() const {
    return correctionFactor;
}

void Profile::setCorrectionFactor(double factor) {
    correctionFactor = factor;
}

void Profile::setTargetGlucoseRange(double min, double max) {
    targetGlucoseMin = min;
    targetGlucoseMax = max;
}

QPair<double, double> Profile::getTargetGlucoseRange() const {
    return QPair<double, double>(targetGlucoseMin, targetGlucoseMax);
}

void Profile::addGlucoseReading(const GlucoseReading &reading) {
    glucoseReadings.append(reading);
    emit glucoseReadingAdded();
    saveHistoricalData();
}

QVector<GlucoseReading> Profile::getGlucoseReadings(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return glucoseReadings;
    }

    QVector<GlucoseReading> filteredReadings;

    for (const GlucoseReading &reading : glucoseReadings) {
        bool include = true;

        if (start.isValid() && reading.timestamp < start) {
            include = false;
        }
        if (end.isValid() && reading.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredReadings.append(reading);
        }
    }

    return filteredReadings;
}

void Profile::addInsulinDose(const InsulinDose &dose) {
    insulinDoses.append(dose);
    saveHistoricalData();
}

QVector<InsulinDose> Profile::getInsulinDoses(const QDateTime &start, const QDateTime &end) const {
    if (!start.isValid() && !end.isValid()) {
        return insulinDoses;
    }

    QVector<InsulinDose> filteredDoses;

    for (const InsulinDose &dose : insulinDoses) {
        bool include = true;

        if (start.isValid() && dose.timestamp < start) {
            include = false;
        }

        if (end.isValid() && dose.timestamp > end) {
            include = false;
        }
        if (include) {
            filteredDoses.append(dose);
        }
    }

    return filteredDoses;
}

QJsonObject Profile::toJson() const {
    QJsonObject json;
    json["name"] = profileName;
    json["basalRate"] = basalRate;
    json["carbRatio"] = carbRatio;
    json["correctionFactor"] = correctionFactor;
    json["targetGlucoseMin"] = targetGlucoseMin;
    json["targetGlucoseMax"] = targetGlucoseMax;
    return json;
}

void Profile::fromJson(const QJsonObject &json) {
    if (json.contains("name")) {
        profileName = json["name"].toString();
    }
    if (json.contains("basalRate")) {
        basalRate = json["basalRate"].toDouble();
    }
    if (json.contains("carbRatio")) {
        carbRatio = json["carbRatio"].toDouble();
    }
    if (json.contains("correctionFactor")) {
        correctionFactor = json["correctionFactor"].toDouble();
    }
    if (json.contains("targetGlucoseMin")) {
        targetGlucoseMin = json["targetGlucoseMin"].toDouble();
    }
    if (json.contains("targetGlucoseMax")) {
        targetGlucoseMax = json["targetGlucoseMax"].toDouble();
    }
}

bool Profile::saveProfile() {
    if (profileName.isEmpty()) {
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QStringList profiles = settings.value(PROFILES_KEY).toStringList();

    if (!profiles.contains(profileName)) {
        profiles.append(profileName);
        settings.setValue(PROFILES_KEY, profiles);
    }
    QJsonObject profileJson = toJson();
    QJsonDocument doc(profileJson);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    settings.setValue("Profile_" + profileName, jsonString);
    settings.sync();

    return settings.status() == QSettings::NoError;
}

bool Profile::loadProfile(const QString &name) {
    if (name.isEmpty()) {
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QStringList profiles = settings.value(PROFILES_KEY).toStringList();
    if (!profiles.contains(name)) {
        return false;
    }

    QString jsonString = settings.value("Profile_" + name).toString();
    if (jsonString.isEmpty()) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }

    fromJson(doc.object());
    return true;
}

bool Profile::saveHistoricalData() {
    if (profileName.isEmpty()) {
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QJsonArray glucoseArray;
    for (const GlucoseReading &reading : glucoseReadings) {
        glucoseArray.append(reading.toJson());
    }

    QJsonDocument glucoseDoc(glucoseArray);
    settings.setValue("GlucoseReadings_" + profileName, glucoseDoc.toJson(QJsonDocument::Compact));

    QJsonArray insulinArray;
    for (const InsulinDose &dose : insulinDoses) {
        insulinArray.append(dose.toJson());
    }

    QJsonDocument insulinDoc(insulinArray);
    settings.setValue("InsulinDoses_" + profileName, insulinDoc.toJson(QJsonDocument::Compact));

    settings.sync();
    return settings.status() == QSettings::NoError;
}

bool Profile::loadHistoricalData() {
    if (profileName.isEmpty()) {
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QString glucoseString = settings.value("GlucoseReadings_" + profileName).toString();
    if (!glucoseString.isEmpty()) {
        QJsonDocument glucoseDoc = QJsonDocument::fromJson(glucoseString.toUtf8());
        if (!glucoseDoc.isNull() && glucoseDoc.isArray()) {
            QJsonArray glucoseArray = glucoseDoc.array();
            glucoseReadings.clear();

            for (const QJsonValue &value : glucoseArray) {
                if (value.isObject()) {
                    glucoseReadings.append(GlucoseReading::fromJson(value.toObject()));
                }
            }
        }
    }

    QString insulinString = settings.value("InsulinDoses_" + profileName).toString();
    if (!insulinString.isEmpty()) {
        QJsonDocument insulinDoc = QJsonDocument::fromJson(insulinString.toUtf8());
        if (!insulinDoc.isNull() && insulinDoc.isArray()) {
            QJsonArray insulinArray = insulinDoc.array();
            insulinDoses.clear();

            for (const QJsonValue &value : insulinArray) {
                if (value.isObject()) {
                    insulinDoses.append(InsulinDose::fromJson(value.toObject()));
                }
            }
        }
    }

    return true;
}

QStringList Profile::getAvailableProfiles() {
    QSettings settings("TandemPump", "COMP3004A4");
    return settings.value(PROFILES_KEY).toStringList();
}

bool Profile::deleteProfile(const QString &name) {
    if (name.isEmpty()) {
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QStringList profiles = settings.value(PROFILES_KEY).toStringList();
    if (!profiles.contains(name)) {
        return false;
    }

    profiles.removeAll(name);
    settings.setValue(PROFILES_KEY, profiles);

    settings.remove("Profile_" + name);

    settings.remove("GlucoseReadings_" + name);
    settings.remove("InsulinDoses_" + name);
    settings.sync();

    return settings.status() == QSettings::NoError;
}

bool Profile::createProfile(const QString &name) {
    if (name.isEmpty()) {
        qDebug() << "empty name, creation failed";
        return false;
    }

    QSettings settings("TandemPump", "COMP3004A4");

    QStringList profiles = settings.value(PROFILES_KEY).toStringList();
    if (profiles.contains(name)) {
        qDebug() << "existing name";
        return false;
    }

    profiles.append(name);
    settings.setValue(PROFILES_KEY, profiles);

    Profile newProfile(name);
    return newProfile.saveProfile();
}
