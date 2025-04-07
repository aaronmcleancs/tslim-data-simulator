#include "headers/contentwidget.h"
#include "ui_contentwidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "headers/authmanager.h"
#include "headers/optionswindow.h"


ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContentWidget)
{
    ui->setupUi(this);
    pump = new Pump();

    if(pump == nullptr){
        qDebug() << "null pump in cwidget construct";
    }

    QGroupBox *bolusGroupBox = findChild<QGroupBox*>("Bolus_Groupbox");
    if (bolusGroupBox) {
        bolusGroupBox->setVisible(false);
    }

    QGroupBox *groupBox2 = findChild<QGroupBox*>("groupBox_2");
    if (groupBox2) {
        groupBox2->setVisible(false);
    }

    connect(ui->profile_create_button, &QPushButton::clicked, this, &ContentWidget::createProfile);
    connect(ui->profile_edit_button, &QPushButton::clicked, this, &ContentWidget::editProfile);
    connect(ui->profile_delete_button, &QPushButton::clicked, this, &ContentWidget::deleteProfile);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &ContentWidget::on_tabWidget_currentChanged);

    if (ui->profile_select_button) {
        connect(ui->profile_select_button, &QPushButton::clicked, this, [this]() {
            if (ui->profile_list->currentItem()) {
                QString profileName = ui->profile_list->currentItem()->text();
                pump->selectActiveProfile(profileName);
                updateHistoryTab();
                updateSettingsTab();
            }
        });
    }

    // Connect the bolus button if it exists
    if (ui->bolusButton) {
        connect(ui->bolusButton, &QPushButton::clicked, this, &ContentWidget::openBolus);
    }
    if (ui->optionsButton) {
        connect(ui->optionsButton, &QPushButton::clicked, this, &ContentWidget::openOptions);
    }


    QStringList profileNames = Profile::getAvailableProfiles();
    for (const QString &name : profileNames) {
        Profile* prof = new Profile(name, nullptr);
        pump->getProfiles().append(prof);
        ui->profile_list->addItem(name);


        if (pump->getProfiles().size() == 1) {
            pump->selectActiveProfile(name);

            GlucoseReading reading1;
            reading1.timestamp = QDateTime::currentDateTime().addSecs(-3600 * 2);
            reading1.value = 5.6;
            prof->addGlucoseReading(reading1);

            GlucoseReading reading2;
            reading2.timestamp = QDateTime::currentDateTime().addSecs(-3600);
            reading2.value = 7.2;
            prof->addGlucoseReading(reading2);


            InsulinDose dose1;
            dose1.timestamp = QDateTime::currentDateTime().addSecs(-3600 * 1.5);
            dose1.amount = 4.2;
            dose1.type = "Bolus";
            prof->addInsulinDose(dose1);

            InsulinDose dose2;
            dose2.timestamp = QDateTime::currentDateTime().addSecs(-1800);
            dose2.amount = 1.8;
            dose2.type = "Correction";
            prof->addInsulinDose(dose2);
        }
    }

    if (pump->getActiveProfile()) {
        updateHistoryTab();
        updateSettingsTab();

        QString activeName = pump->getActiveProfile()->getName();
        for (int i = 0; i < ui->profile_list->count(); ++i) {
            if (ui->profile_list->item(i)->text() == activeName) {
                ui->profile_list->setCurrentRow(i);
                break;
            }
        }
    }
    if (pump) {
        connect(pump, &Pump::alertTriggered, this, &ContentWidget::displayAlert);
    }

    connect(ui->profile_create_button, SIGNAL(released()), this, SLOT(on_createProfileButton_clicked()));

    setupBloodSugarGraph();
    loadGraphData();
    connect(pump->getActiveProfile(), &Profile::glucoseReadingAdded, this, &ContentWidget::loadGraphData);

    // Connect to profile changes
    connect(pump, &Pump::activeProfileChanged, this, [this](Profile* newProfile) {
        // Disconnect from previous profile
        if (pump->getActiveProfile()) {
            disconnect(pump->getActiveProfile(), &Profile::glucoseReadingAdded,
                     this, &ContentWidget::loadGraphData);
        }

        // Connect to new profile
        if (newProfile) {
            connect(newProfile, &Profile::glucoseReadingAdded,
                   this, &ContentWidget::loadGraphData);
        }
    });

    activeTimeTimer = new QTimer(this);
    activeTimeTimer->setInterval(1000);
    connect(activeTimeTimer, &QTimer::timeout, this, &ContentWidget::updateActiveTime);
    activeTimeTimer->start();
    connect(pump->getInsulinCartridge(), &InsulinCartridge::insulinLevelChanged,
            this, [this](int level) {
                ui->iobValue->setText(QString::number(level) + " U");
            });

}

ContentWidget::~ContentWidget()
{
    delete ui;
}

void ContentWidget::on_optionsButton_clicked()
{
    qDebug() << "Navigate to the options";
}

void ContentWidget::displayAlert(const QString &alertMessage, double bgValue)
{
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString fullAlert = alertMessage + " (BG: " + QString::number(bgValue) + ")";

    int rowCount = ui->alertsTable->rowCount();
    ui->alertsTable->insertRow(rowCount);

    QTableWidgetItem *alertItem = new QTableWidgetItem(fullAlert);
    QTableWidgetItem *timeItem = new QTableWidgetItem(timeStamp);

    alertItem->setFlags(alertItem->flags() & ~Qt::ItemIsEditable);
    timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);

    ui->alertsTable->setItem(rowCount, 0, alertItem);
    ui->alertsTable->setItem(rowCount, 1, timeItem);

    ui->alertsTable->scrollToItem(timeItem);
}


void ContentWidget::updateActiveTime() {
    if (pump && pump->getCGM() && pump->getCGM()->getStartTime().isValid()) {
        qint64 secondsElapsed = pump->getCGM()->getStartTime().secsTo(QDateTime::currentDateTime());
        QTime timeElapsed(0,0);
        timeElapsed = timeElapsed.addSecs(secondsElapsed);
        ui->activeTime->setText(timeElapsed.toString("hh:mm:ss"));
    }
}


Pump* ContentWidget::getPump() const {
    return pump;
}

void ContentWidget::createProfile() {
    QString name = ui->new_profile_name_box->toPlainText().trimmed();
    if (name.isEmpty()) {
        qDebug() << "name cannot be empty";
        return;
    }
    double br = ui->new_profile_br_box->value();
    double cf = ui->new_profile_cf_box->value();
    double cr = ui->new_profile_cr_box->value();
    double tmax = ui->new_profile_max_box->value();
    double tmin = ui->new_profile_min_box->value();

    if (pump->findIndex(name) != -1) {
        qDebug() << "Profile already exists";
        return;
    }

    pump->createProfile(name, br, cr, cf, tmin, tmax);
    ui->profile_list->addItem(name);
    ui->new_profile_name_box->clear();

    pump->selectActiveProfile(name);

    updateHistoryTab();
    updateSettingsTab();

    for (int i = 0; i < ui->profile_list->count(); ++i) {
        if (ui->profile_list->item(i)->text() == name) {
            ui->profile_list->setCurrentRow(i);
            break;
        }
    }
}

void ContentWidget::deleteProfile(){
    QListWidgetItem *selected = ui->profile_list->currentItem();
    if (selected) {
        QString name = selected->text();
        pump->removeProfile(name);
        delete ui->profile_list->takeItem(ui->profile_list->row(selected));
    }
}

void ContentWidget::editProfile(){
    QString name = ui->edit_profile_name_box->toPlainText().trimmed();
    QString setting = ui->edit_profile_setting_box->currentText();
    double value = ui->edit_val_box->value();
    pump->updateProfile(name, setting, value);
}

void ContentWidget::selectProfile(){
    if (!ui->profile_list->currentItem()) {
        return;
    }

    QString name = ui->profile_list->currentItem()->text().trimmed();
    qDebug() << "selected profile name:" << name;

    pump->selectActiveProfile(name);

    updateHistoryTab();
    updateSettingsTab();
}

void ContentWidget::updateHistoryTab() {
    ui->historyTextEdit->clear();
    
    Profile* activeProfile = pump->getActiveProfile();
    if (!activeProfile) {
        return;
    }

    QVector<GlucoseReading> glucoseReadings = activeProfile->getGlucoseReadings();
    QVector<InsulinDose> insulinDoses = activeProfile->getInsulinDoses();
    PumpHistory* pumpHistory = pump->getPumpHistory();
    QVector<BasalRateEvent> basalEvents;
    QVector<BolusEvent> bolusEvents;
    QVector<AlertEvent> alertEvents;
    QVector<SettingChangeEvent> settingEvents;
    QVector<StatusEvent> statusEvents;

    if (pumpHistory) {
        basalEvents = pumpHistory->getBasalRateEvents();
        bolusEvents = pumpHistory->getBolusEvents();
        alertEvents = pumpHistory->getAlertEvents();
        settingEvents = pumpHistory->getSettingChangeEvents();
        statusEvents = pumpHistory->getStatusEvents();
    }

    // Create a multimap to sort all events by timestamp (newest first)
    QMultiMap<QDateTime, QString> sortedEvents;
    
    // Format and add glucose readings
    for (const GlucoseReading &reading : glucoseReadings) {
        QString eventText = QString("[%1] GLUCOSE: %2 mmol/L (CGM Reading)")
                             .arg(reading.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(reading.value);
        sortedEvents.insert(reading.timestamp, eventText);
    }
    
    // Format and add insulin doses
    for (const InsulinDose &dose : insulinDoses) {
        QString eventText = QString("[%1] INSULIN: %2 U (%3)")
                             .arg(dose.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(dose.amount)
                             .arg(dose.type);
        sortedEvents.insert(dose.timestamp, eventText);
    }
    
    // Format and add basal rate events
    for (const BasalRateEvent &event : basalEvents) {
        QString eventText = QString("[%1] BASAL: %2 U/hr (Basal Rate Change)")
                             .arg(event.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(event.rate);
        sortedEvents.insert(event.timestamp, eventText);
    }
    
    // Format and add bolus events
    for (const BolusEvent &event : bolusEvents) {
        QString details = event.bolusType;
        if (event.duration > 0) {
            details += QString(", %1 min").arg(event.duration);
        }
        if (event.carbInput > 0) {
            details += QString(", %1 g carbs").arg(event.carbInput);
        }
        if (event.bgInput > 0) {
            details += QString(", BG: %1 mmol/L").arg(event.bgInput);
        }
        
        QString eventText = QString("[%1] BOLUS: %2 U (%3)")
                             .arg(event.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(event.amount)
                             .arg(details);
        sortedEvents.insert(event.timestamp, eventText);
    }
    
    // Format and add alert events
    for (const AlertEvent &event : alertEvents) {
        QString acknowledged = event.acknowledged ? "Acknowledged" : "Not Acknowledged";
        QString eventText = QString("[%1] ALERT: %2 (BG: %3 mmol/L, %4)")
                             .arg(event.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(event.alertType)
                             .arg(event.bgValue)
                             .arg(acknowledged);
        sortedEvents.insert(event.timestamp, eventText);
    }
    
    // Format and add setting change events
    for (const SettingChangeEvent &event : settingEvents) {
        QString eventText = QString("[%1] SETTING: %2 (Changed from %3 to %4)")
                             .arg(event.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(event.settingName)
                             .arg(event.oldValue)
                             .arg(event.newValue);
        sortedEvents.insert(event.timestamp, eventText);
    }
    
    // Format and add status events
    for (const StatusEvent &event : statusEvents) {
        QString eventText = QString("[%1] STATUS: %2 (%3)")
                             .arg(event.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(event.statusType)
                             .arg(event.statusDetails);
        sortedEvents.insert(event.timestamp, eventText);
    }
    
    // Display all events in the text edit, newest first
    QString historyText;
    auto it = sortedEvents.end();
    while (it != sortedEvents.begin()) {
        --it;
        historyText += it.value() + "\n";
    }
    
    ui->historyTextEdit->setText(historyText);
}

void ContentWidget::updateSettingsTab() {
    Profile* activeProfile = pump->getActiveProfile();
    if (!activeProfile) {

        ui->setting_name_label->setText("Name: None");
        ui->setting_br_label->setText("Basal Rate: N/A");
        ui->setting_cr_label->setText("Carb Ratio: N/A");
        ui->setting_cf_label->setText("Correction Factor: N/A");
        return;
    }


    ui->setting_name_label->setText("Name: " + activeProfile->getName());
    ui->setting_br_label->setText("Basal Rate: " + QString::number(activeProfile->getBasalRate()) + " U/hr");
    ui->setting_cr_label->setText("Carb Ratio: " + QString::number(activeProfile->getCarbRatio()) + " g/U");
    ui->setting_cf_label->setText("Correction Factor: " + QString::number(activeProfile->getCorrectionFactor()) + " mmol/L/U");


    QPair<double, double> targetRange = activeProfile->getTargetGlucoseRange();
    if (ui->active_profile_groupbox->findChild<QLabel*>("setting_target_label") == nullptr) {
        QLabel* targetLabel = new QLabel(ui->active_profile_groupbox);
        targetLabel->setObjectName("setting_target_label");
        targetLabel->setGeometry(10, 250, 241, 51);
        QFont font;
        font.setPointSize(16);
        targetLabel->setFont(font);
        targetLabel->show();
    }

    QLabel* targetLabel = ui->active_profile_groupbox->findChild<QLabel*>("setting_target_label");
    if (targetLabel) {
        targetLabel->setText("Target Range: " + QString::number(targetRange.first) +
                          " - " + QString::number(targetRange.second) + " mmol/L");
        QFont font;
        font.setPointSize(16);
        targetLabel->setFont(font);
    }
}

void ContentWidget::on_tabWidget_currentChanged(int index) {
    if (index == 0) {
        updateHistoryTab();
    } else if (index == 2) {
        updateSettingsTab();
    }
}

void ContentWidget::setupBloodSugarGraph() {
    QCustomPlot *plot = ui->graph;

    // Remove default axis labels and numbers
    plot->xAxis->setTickLabels(false); // Hide x-axis numbers
    plot->xAxis->setLabel(""); // Remove x-axis label
    plot->yAxis->setLabel("Blood Sugar (mmol/L)"); // Changed unit to mmol/L for 0-20 range

    // Configure y-axis for mmol/L range (0-20)
    plot->yAxis->setRange(0, 20);
    plot->yAxis->grid()->setPen(QPen(QColor(200,200,200), 1, Qt::DotLine));

    // Configure x-axis for 20 discrete readings
    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickCount(20); // Exactly 20 vertical grid lines
    fixedTicker->setTickStep(1); // One unit per reading
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
    plot->xAxis->setTicker(fixedTicker);
    plot->xAxis->setRange(0, 20); // 20 readings

    // Style the graph
    plot->addGraph();
    plot->graph(0)->setName("Blood Glucose");
    plot->graph(0)->setPen(QPen(QColor(0, 120, 215), 2)); // Modern blue

    // Add colored zones for blood sugar ranges
    QCPItemRect *lowRange = new QCPItemRect(plot);
    lowRange->setPen(Qt::NoPen);
    lowRange->setBrush(QColor(255, 100, 100, 30)); // Red tint for low
    lowRange->topLeft->setTypeY(QCPItemPosition::ptPlotCoords);
    lowRange->topLeft->setCoords(0, 4);
    lowRange->bottomRight->setTypeY(QCPItemPosition::ptPlotCoords);
    lowRange->bottomRight->setCoords(20, 0);

    QCPItemRect *targetRange = new QCPItemRect(plot);
    targetRange->setPen(Qt::NoPen);
    targetRange->setBrush(QColor(100, 255, 100, 30)); // Green tint for target
    targetRange->topLeft->setTypeY(QCPItemPosition::ptPlotCoords);
    targetRange->topLeft->setCoords(0, 10);
    targetRange->bottomRight->setTypeY(QCPItemPosition::ptPlotCoords);
    targetRange->bottomRight->setCoords(20, 4);

    plot->setBackground(Qt::white);
    plot->replot();
}

void ContentWidget::loadGraphData() {
    if (!pump || !pump->getActiveProfile()) {
        qDebug() << "no pump or profile";
        return;
    }

    // Get the historical readings
    const QVector<GlucoseReading>& readings = pump->getActiveProfile()->getGlucoseReadings();
    ui->label_2->setText(QString::number(readings.last().value) + " mmol/L");
    // i just  put this here for convienience, gotta update graph so might aswell update lebell aswell
    QVector<double> xValues, yValues;
    int pointCount = 0;

    // Convert readings to plot data (last 20 readings only)
    int startIdx = qMax(0, readings.size() - 20);
    for (int i = startIdx; i < readings.size(); ++i) {
        xValues.append(pointCount++);
        yValues.append(readings[i].value);
    }

    // Update the graph
    if (!xValues.isEmpty()) {
        ui->graph->graph(0)->setData(xValues, yValues);
        ui->graph->xAxis->setRange(0, qMin(19, xValues.size()-1));
        ui->graph->yAxis->setRange(0, 20); // 0-20 mmol/L range
        ui->graph->replot();
    }
}

void ContentWidget::on_setting_pin_update_button_clicked() {
    QString newPin = ui->setting_pin_1->text() + ui->setting_pin_2->text() + ui->setting_pin_3->text() + ui->setting_pin_4->text();
    AuthManager* authManager = AuthManager::getInstance();

    authManager->setPinCode(newPin);
}

void ContentWidget::on_cancelBolus_clicked()
{
    qDebug() << "Cancel Bolus button pressed.";
    emit cancelBolusRequested();
}
