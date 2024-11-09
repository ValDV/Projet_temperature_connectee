#include "configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    configureSerialPort();
    connect(ui->addButton, &QPushButton::clicked, this, &ConfigWindow::onAddSondeClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &ConfigWindow::onRemoveSondeClicked);
    connect(ui->configureButton, &QPushButton::clicked, this, &ConfigWindow::onConfigureSondeClicked);
}

ConfigWindow::~ConfigWindow() {
    delete ui;
}

void ConfigWindow::configureSerialPort() {
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
}

void ConfigWindow::onAddSondeClicked() {
    Sonde newSonde;
    newSonde.id = ui->idEdit->text();
    newSonde.name = ui->nameEdit->text();
    newSonde.ip = ui->ipEdit->text();
    newSonde.port = ui->portEdit->text().toInt();
    newSonde.mqttUser = ui->mqttUserEdit->text();
    newSonde.mqttPassword = ui->mqttPasswordEdit->text();
    newSonde.mqttTopic = ui->mqttTopicEdit->text();
    newSonde.isConfigured = false;

    sondes.insert(newSonde.id, newSonde);
    updateSondeList();
}

void ConfigWindow::onRemoveSondeClicked() {
    QString id = ui->idEdit->text();
    sondes.remove(id);
    updateSondeList();
}

void ConfigWindow::onConfigureSondeClicked() {
    QString id = ui->idEdit->text();
    if (sondes.contains(id)) {
        Sonde sonde = sondes.value(id);
        onSendConfiguration(sonde);
        sonde.isConfigured = true;
        sondes[id] = sonde;
    }
}

void ConfigWindow::onSendConfiguration(Sonde sonde) {
    if (serialPort.isOpen() || serialPort.open(QIODevice::WriteOnly)) {
        QString configMessage = QString("{\"id\":\"%1\",\"name\":\"%2\",\"ip\":\"%3\",\"port\":%4,\"mqttUser\":\"%5\",\"mqttPassword\":\"%6\",\"mqttTopic\":\"%7\"}")
                                .arg(sonde.id)
                                .arg(sonde.name)
                                .arg(sonde.ip)
                                .arg(sonde.port)
                                .arg(sonde.mqttUser)
                                .arg(sonde.mqttPassword)
                                .arg(sonde.mqttTopic);

        serialPort.write(configMessage.toUtf8());
        serialPort.flush();
    }
}

void ConfigWindow::updateSondeList() {
    ui->sondeList->clear();
    for (const auto &sonde : sondes) {
        ui->sondeList->addItem(sonde.id + " - " + sonde.name);
    }
}
