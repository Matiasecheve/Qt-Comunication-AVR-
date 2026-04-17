#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Estados del sistema (Igual que en el AVR)
    enum State { Desconectado, Listo, Corriendo, Detenido, Error };

private slots:
    // Eventos de la Interfaz
    void on_btnConectar_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnReset_clicked();
    void readSerial(); // Recepción de datos
    void on_btnSimIr0_clicked();
    void on_btnSimIr1_clicked();
    void on_btnSimIr2_clicked();
    void on_SendConfigOut_clicked();
    void on_btnSimServo1_clicked();
    void on_btnSimServo0_clicked();
    void on_btnSimServo2_clicked();
    QString formatearAscii(const QByteArray &datos);
    void procesarComandoMicro(uint8_t cmd, QByteArray payload);

    void on_sendVel_clicked();

    void on_sendTimeout_clicked();

    void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_sendWaitCenter_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    State m_estadoActual;
    void setEstado(State nuevoEstado);
    void enviarComando(uint8_t cmd, QByteArray payload = QByteArray());
    void procesarComandoMicro(uint8_t cmd);
};
#endif