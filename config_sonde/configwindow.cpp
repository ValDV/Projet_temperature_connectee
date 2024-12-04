#include "configwindow.h"
#include "ui_configwindow.h"
#include <QMessageBox>

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);

    ui->idLineEdit->setText("Test");

    // Charger les ports série disponibles dans la ComboBox
    loadSerialPorts();
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::loadSerialPorts()
{
    ui->portComboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->portComboBox->addItem(info.portName());
    }
}

void ConfigWindow::on_sendButton_clicked()
{
    // Récupérer les valeurs des champs
    QString id = ui->idLineEdit->text();
    QString ssid = ui->ssidLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // Vérifier que tous les champs sont remplis
    if (id.isEmpty() || ssid.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs.");
        return;
    }

    // Configurer et ouvrir le port série
    serialPort.setPortName(ui->portComboBox->currentText());
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série.");
        return;
    }

    // Créer le message à envoyer
    QString message = QString("ID:%1\nSSID:%2\nPASSWORD:%3\n").arg(id, ssid, password);

    // Envoyer le message
    serialPort.write(message.toUtf8());
    serialPort.close();

    QMessageBox::information(this, "Envoyé", "Les données ont été envoyées avec succès.");
}
