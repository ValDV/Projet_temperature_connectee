#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>
#include "simulatedsonde.h"

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private slots:
    void on_startSimulationButton_clicked();

private:
    Ui::ConfigWindow *ui;
    SimulatedSonde *simulatedSonde;

    void displayDataReceived(const QString &data);
};

#endif
