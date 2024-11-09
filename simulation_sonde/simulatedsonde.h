#ifndef SIMULATEDSONDE_H
#define SIMULATEDSONDE_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

class SimulatedSonde : public QObject {
    Q_OBJECT

public:
    explicit SimulatedSonde(const QString &id, const QString &name, QObject *parent = nullptr);
    void configureSonde(const QString &wifiSSID, const QString &wifiPassword,
                        const QString &mqttBroker, int mqttPort,
                        const QString &mqttUsername, const QString &mqttPassword,
                        const QString &topic);
    void startSimulation();

signals:
    void dataReceived(const QString &data);

private slots:
    void sendTemperatureReading();

private:
    QString id;
    QString name;
    QString topic;
    QTimer *simulationTimer;
};

#endif
