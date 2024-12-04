#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfigWindow;
}
QT_END_NAMESPACE

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private slots:
    void on_sendButton_clicked();

private:
    Ui::ConfigWindow *ui;
    QSerialPort serialPort;

    void loadSerialPorts(); // Pour charger les ports disponibles dans portComboBox
};

#endif // CONFIGWINDOW_H
