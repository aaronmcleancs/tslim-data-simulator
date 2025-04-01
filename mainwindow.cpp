#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(StatusBar *sb, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBar = sb;
    ui->statusBarContainer->layout()->addWidget(statusBar);

//    ui->Bolus_Groupbox->setVisible(false);
//    ui->groupBox_2->setVisible(false);

    QGroupBox *bolusGroupBox = findChild<QGroupBox*>("Bolus_Groupbox");
    if (bolusGroupBox) {
        bolusGroupBox->setVisible(false);
    }
    
    QGroupBox *groupBox2 = findChild<QGroupBox*>("groupBox_2");
    if (groupBox2) {
        groupBox2->setVisible(false);
    }

    connect(ui->profile_create_button, &QPushButton::clicked, this, &MainWindow::createProfile);
    connect(ui->profile_edit_button, &QPushButton::clicked, this, &MainWindow::editProfile);
    connect(ui->profile_delete_button, &QPushButton::clicked, this, &MainWindow::deleteProfile);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tabWidget_currentChanged);
    
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
    connect(ui->profile_create_button, SIGNAL(released()), this, SLOT(on_createProfileButton_clicked()));

    pump = new Pump(nullptr);

    AuthManager* authManager = AuthManager::getInstance();
    connect(authManager, &AuthManager::authStateChanged,
            this, &MainWindow::onAuthStateChanged);
}

void MainWindow::onAuthStateChanged(bool authenticated)
{
    if (!authenticated) {
        close();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createProfile() {
    QString name = ui->new_profile_name_box->toPlainText().trimmed();
    if (name.isEmpty()) {
        qDebug() << "name cannot be empty";
        return;
    }
    double br = ui->new_profile_br_box->value();
    double cf = ui->new_profile_cf_box->value();
    double cr = ui->new_profile_cr_box->value();
    double tmax = ui->new_profile_min_box->value();
    double tmin = ui->new_profile_min_box->value();

    // Check for existing profile first
    if (pump->findIndex(name) != -1) {
        qDebug() << "Profile already exists";
        return;
    }

    pump->createProfile(name, br, cr, cf, tmin, tmax);
    ui->profile_list->addItem(name);
    ui->new_profile_name_box->clear();
}

void MainWindow::deleteProfile(){
    QListWidgetItem *selected = ui->profile_list->currentItem();
    if (selected) {
        QString name = selected->text();
        pump->removeProfile(name);
        delete ui->profile_list->takeItem(ui->profile_list->row(selected));
    }}

void MainWindow::editProfile(){
    QString name = ui->edit_profile_name_box->toPlainText().trimmed();
    QString setting = ui->edit_profile_setting_box->currentText();
    double value = ui->edit_val_box->value();
    pump->updateProfile(name, setting, value);
}

void MainWindow::selectProfile(){
    QString name = ui->profile_list->currentItem()->text();
    pump->selectActiveProfile(name);
}

void MainWindow::updateHistoryTab() {
    ui->historyTable->clearContents();
    ui->historyTable->setRowCount(0);
    
    Profile* activeProfile = pump->getActiveProfile();
    if (!activeProfile) {
        return;
    }
    
    
    QVector<GlucoseReading> glucoseReadings = activeProfile->getGlucoseReadings();
    QVector<InsulinDose> insulinDoses = activeProfile->getInsulinDoses();
    
    
    int totalRows = glucoseReadings.size() + insulinDoses.size();
    ui->historyTable->setRowCount(totalRows);
    
    int currentRow = 0;
    
    
    for (const GlucoseReading &reading : glucoseReadings) {
        QTableWidgetItem *dateTimeItem = new QTableWidgetItem(reading.timestamp.toString("yyyy-MM-dd hh:mm"));
        QTableWidgetItem *typeItem = new QTableWidgetItem("Glucose");
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(reading.value) + " mmol/L");
        QTableWidgetItem *detailsItem = new QTableWidgetItem();
        
        
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
    
    
    for (const InsulinDose &dose : insulinDoses) {
        QTableWidgetItem *dateTimeItem = new QTableWidgetItem(dose.timestamp.toString("yyyy-MM-dd hh:mm"));
        QTableWidgetItem *typeItem = new QTableWidgetItem("Insulin");
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(dose.amount) + " U");
        QTableWidgetItem *detailsItem = new QTableWidgetItem(dose.type);
        
        
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
    
    ui->historyTable->resizeColumnsToContents();
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    
    if (index == 0) {
        updateHistoryTab();
    }
}
