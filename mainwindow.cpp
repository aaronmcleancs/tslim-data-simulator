#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar = new StatusBar(this);
    ui->statusBarContainer->layout()->addWidget(statusBar);

    // Find and hide widgets
    QGroupBox *bolusGroupBox = findChild<QGroupBox*>("Bolus_Groupbox");
    if (bolusGroupBox) {
        bolusGroupBox->setVisible(false);
    }
    
    QGroupBox *groupBox2 = findChild<QGroupBox*>("groupBox_2");
    if (groupBox2) {
        groupBox2->setVisible(false);
    }

    connect(ui->profile_create_button, &QPushButton::clicked, this, &MainWindow::createProfile);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tabWidget_currentChanged);
    
    // If there's a profile_select_button, connect it as well
    if (ui->profile_select_button) {
        connect(ui->profile_select_button, &QPushButton::clicked, this, [this]() {
            if (ui->profile_list->currentItem()) {
                QString profileName = ui->profile_list->currentItem()->text();
                pump->selectActiveProfile(profileName);
                updateHistoryTab();
            }
        });
    }

    pump = new Pump(nullptr);
    
    QStringList profileNames = Profile::getAvailableProfiles();
    for (const QString &name : profileNames) {
        Profile* prof = new Profile(name, nullptr);
        pump->getProfiles().append(prof);
        ui->profile_list->addItem(name);
        
        // If this is the first profile, select it and add some test data
        if (pump->getProfiles().size() == 1) {
            pump->selectActiveProfile(name);
            
            // Add some test glucose readings
            GlucoseReading reading1;
            reading1.timestamp = QDateTime::currentDateTime().addSecs(-3600 * 2); // 2 hours ago
            reading1.value = 5.6;
            prof->addGlucoseReading(reading1);
            
            GlucoseReading reading2;
            reading2.timestamp = QDateTime::currentDateTime().addSecs(-3600); // 1 hour ago
            reading2.value = 7.2;
            prof->addGlucoseReading(reading2);
            
            // Add some test insulin doses
            InsulinDose dose1;
            dose1.timestamp = QDateTime::currentDateTime().addSecs(-3600 * 1.5); // 1.5 hours ago
            dose1.amount = 4.2;
            dose1.type = "Bolus";
            prof->addInsulinDose(dose1);
            
            InsulinDose dose2;
            dose2.timestamp = QDateTime::currentDateTime().addSecs(-1800); // 30 min ago
            dose2.amount = 1.8;
            dose2.type = "Correction";
            prof->addInsulinDose(dose2);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createProfile() {
    QString name = ui->new_profile_name_box->toPlainText().trimmed();
    double br = ui->new_profile_br_box->value();
    double cf = ui->new_profile_cf_box->value();
    double cr = ui->new_profile_cr_box->value();
    double tmax = ui->new_profile_min_box->value();
    double tmin = ui->new_profile_min_box->value();

    pump->createProfile(name, br, cr, cf, tmin, tmax);
    ui->profile_list->addItem(name);
}

void MainWindow::updateHistoryTab() {
    ui->historyTable->clearContents();
    ui->historyTable->setRowCount(0);
    
    Profile* activeProfile = pump->getActiveProfile();
    if (!activeProfile) {
        return;
    }
    
    // Get all glucose readings and insulin doses
    QVector<GlucoseReading> glucoseReadings = activeProfile->getGlucoseReadings();
    QVector<InsulinDose> insulinDoses = activeProfile->getInsulinDoses();
    
    // Calculate total rows needed
    int totalRows = glucoseReadings.size() + insulinDoses.size();
    ui->historyTable->setRowCount(totalRows);
    
    int currentRow = 0;
    
    // Add glucose readings to the table
    for (const GlucoseReading &reading : glucoseReadings) {
        QTableWidgetItem *dateTimeItem = new QTableWidgetItem(reading.timestamp.toString("yyyy-MM-dd hh:mm"));
        QTableWidgetItem *typeItem = new QTableWidgetItem("Glucose");
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(reading.value) + " mmol/L");
        QTableWidgetItem *detailsItem = new QTableWidgetItem();
        
        // Set items as not editable
        dateTimeItem->setFlags(dateTimeItem->flags() & ~Qt::ItemIsEditable);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
        detailsItem->setFlags(detailsItem->flags() & ~Qt::ItemIsEditable);
        
        ui->historyTable->setItem(currentRow, 0, dateTimeItem);
        ui->historyTable->setItem(currentRow, 1, typeItem);
        ui->historyTable->setItem(currentRow, 2, valueItem);
        ui->historyTable->setItem(currentRow, 3, detailsItem);
        
        currentRow++;
    }
    
    // Add insulin doses to the table
    for (const InsulinDose &dose : insulinDoses) {
        QTableWidgetItem *dateTimeItem = new QTableWidgetItem(dose.timestamp.toString("yyyy-MM-dd hh:mm"));
        QTableWidgetItem *typeItem = new QTableWidgetItem("Insulin");
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(dose.amount) + " U");
        QTableWidgetItem *detailsItem = new QTableWidgetItem(dose.type);
        
        // Set items as not editable
        dateTimeItem->setFlags(dateTimeItem->flags() & ~Qt::ItemIsEditable);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
        detailsItem->setFlags(detailsItem->flags() & ~Qt::ItemIsEditable);
        
        ui->historyTable->setItem(currentRow, 0, dateTimeItem);
        ui->historyTable->setItem(currentRow, 1, typeItem);
        ui->historyTable->setItem(currentRow, 2, valueItem);
        ui->historyTable->setItem(currentRow, 3, detailsItem);
        
        currentRow++;
    }
    
    // Resize columns to content
    ui->historyTable->resizeColumnsToContents();
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    // Check if the current tab is the history tab (index 0)
    if (index == 0) {
        updateHistoryTab();
    }
}
