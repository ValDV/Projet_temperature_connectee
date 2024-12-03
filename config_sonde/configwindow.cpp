#include "configwindow.h"
#include "ui_configwindow.h"
#include <QMessageBox>

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConfigWindow)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);
    populateSerialPorts();

    // Configuration initiale du port série
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::populateSerialPorts()
{
    ui->portComboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->portComboBox->addItem(info.portName());
    }
}

void ConfigWindow::on_sendButton_clicked()
{
    // Récupération des paramètres saisis
    QString ssid = ui->ssidLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (ssid.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir les champs SSID et mot de passe !");
        return;
    }

    // Format des données au format JSON
    QString configData = QString("{\"ssid\":\"%1\", \"password\":\"%2\"}\n").arg(ssid, password);

    // Ouverture du port série sélectionné
    QString portName = ui->portComboBox->currentText();
    serial->setPortName(portName);
    if (!serial->open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série !");
        return;
    }

    // Envoi des données à l'Arduino
    serial->write(configData.toUtf8());
    serial->flush();

    QMessageBox::information(this, "Succès", "Paramètres envoyés !");
    serial->close();
}
