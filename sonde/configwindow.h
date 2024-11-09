#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QMap>
#include <QSerialPort>

struct Sonde {
    QString id;
    QString name;
    QString ip;
    int port;
    QString mqttUser;
    QString mqttPassword;
    QString mqttTopic;
    bool isConfigured;
};

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QWidget {
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private slots:
    void onAddSondeClicked();
    void onRemoveSondeClicked();
    void onConfigureSondeClicked();
    void onSendConfiguration(Sonde sonde);

private:
    Ui::ConfigWindow *ui;
    QMap<QString, Sonde> sondes;
    QSerialPort serialPort;

    void updateSondeList();
    void configureSerialPort();
};

#endif
