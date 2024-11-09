#include "configwindow.h"
#include "ui_configwindow.h"
#include "simulatedsonde.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigWindow),
    simulatedSonde(nullptr)
{
    ui->setupUi(this);

    ui->mqttPortSpinBox->setRange(1, 65535);
}

ConfigWindow::~ConfigWindow() {
    delete ui;
}

void ConfigWindow::on_startSimulationButton_clicked() {
    QString id = ui->idLineEdit->text();
    QString name = ui->nameLineEdit->text();
    QString wifiSSID = ui->wifiSSIDLineEdit->text();
    QString wifiPassword = ui->wifiPasswordLineEdit->text();
    QString mqttBroker = ui->mqttBrokerLineEdit->text();
    int mqttPort = ui->mqttPortSpinBox->value();
    QString mqttUsername = ui->mqttUsernameLineEdit->text();
    QString mqttPassword = ui->mqttPasswordLineEdit->text();
    QString topic = ui->topicLineEdit->text();

    if (id.isEmpty() || name.isEmpty() || wifiSSID.isEmpty() || mqttBroker.isEmpty() || topic.isEmpty()) {
        ui->outputTextEdit->append("Erreur : Veuillez remplir tous les champs requis.");
        return;
    }

    if (simulatedSonde) delete simulatedSonde;
    simulatedSonde = new SimulatedSonde(id, name, this);

    connect(simulatedSonde, &SimulatedSonde::dataReceived, this, &ConfigWindow::displayDataReceived);

    simulatedSonde->configureSonde(wifiSSID, wifiPassword, mqttBroker, mqttPort, mqttUsername, mqttPassword, topic);

    simulatedSonde->startSimulation();
}

void ConfigWindow::displayDataReceived(const QString &data) {
    ui->outputTextEdit->append(data);
}


