#include "simulatedsonde.h"
#include <QRandomGenerator>

SimulatedSonde::SimulatedSonde(const QString &id, const QString &name, QObject *parent)
    : QObject(parent), id(id), name(name) {
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &SimulatedSonde::sendTemperatureReading);
}

void SimulatedSonde::configureSonde(const QString &wifiSSID, const QString &wifiPassword,
                                    const QString &mqttBroker, int mqttPort,
                                    const QString &mqttUsername, const QString &mqttPassword,
                                    const QString &topic) {
    this->topic = topic;
    emit dataReceived("Configuration réussie pour la sonde simulée.");
}

void SimulatedSonde::startSimulation() {
    simulationTimer->start(10000);
}

void SimulatedSonde::sendTemperatureReading() {
    QJsonObject temperatureReading {
        {"id", id},
        {"name", name},
        {"type", "TEMPERATURE"},
        {"value", 18 + QRandomGenerator::global()->bounded(10)},
        {"unit", "°C"}
    };
    QJsonDocument doc(temperatureReading);
    emit dataReceived(doc.toJson(QJsonDocument::Compact));
}
