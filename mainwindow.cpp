#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);

    // Configuración inicial UART
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerial);

    setEstado(Desconectado);
}

MainWindow::~MainWindow() {
    if(serial->isOpen()) serial->close();
    delete ui;
}

// --- FUNCIONES DE LÓGICA ---

void MainWindow::setEstado(State nuevoEstado) {
    m_estadoActual = nuevoEstado;
    if(!ui->lblEstado) return;

    switch (m_estadoActual) {
    case Desconectado:
        ui->lblEstado->setText("ESTADO: IDLE / RESET");
        ui->lblEstado->setStyleSheet("color: gray; font-weight: bold;");
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(false);
        break;

    case Listo:
        ui->lblEstado->setText("ESTADO: LISTO (READY)");
        ui->lblEstado->setStyleSheet("color: blue; font-weight: bold;");
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        break;

    case Corriendo:
        ui->lblEstado->setText("ESTADO: CLASIFICANDO");
        ui->lblEstado->setStyleSheet("color: green; font-weight: bold;");
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        break;

    case Detenido:
        ui->lblEstado->setText("ESTADO: DETENIDO (STOP)");
        ui->lblEstado->setStyleSheet("color: orange; font-weight: bold;");
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        break;

    case Error:
        ui->lblEstado->setText("ESTADO: ERROR");
        ui->lblEstado->setStyleSheet("color: red; font-weight: bold;");
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(false);
        break;
    }
}

// ESTA FUNCIÓN AHORA ES LA ÚNICA QUE TIENE PERMISO DE CAMBIAR EL ESTADO
void MainWindow::procesarComandoMicro(uint8_t cmd) {
    switch (cmd) {
    case 0xF0: // Handshake / Alive
        if (m_estadoActual != Corriendo) {
            setEstado(Listo); // El micro avisa que despertó y pasamos a Azul
        }
        ui->txtLog->append(">> Micro: ACK (0xF0) - Sistema en línea y conectado.");
        break;

    case 0x40: // Configuración OK
        ui->txtLog->append(">> Micro: ACK (0x40) - Configuración guardada en la RAM.");
        break;

    case 0x50: // Start / Confirmación de que arrancó
        setEstado(Corriendo); // <--- ACÁ LA INTERFAZ PASA A VERDE
        ui->txtLog->append(">> Micro: ACK (0x50) - cinta en movimiento.");
        break;

    case 0x51: // Stop
        setEstado(Detenido); // <--- ACÁ LA INTERFAZ PASA A NARANJA
        ui->txtLog->append(">> Micro: ACK (0x51) -  cinta detenida.");
        break;

    case 0x53: // Reset
        setEstado(Desconectado); // <--- ACÁ LA INTERFAZ PASA A GRIS
        ui->btnConectar->setEnabled(true);
        ui->txtLog->append(">> Micro: ACK (0x53) - Memoria borrada y hardware reiniciado.");
        break;

    case 0x5F: // Nueva Caja
        ui->txtLog->append(">> Micro: Evento (0x5F) - Nueva caja física detectada en cinta.");
        break;

    case 0x5E: // Evento Sensor Físico
        ui->txtLog->append(">> Micro: Evento (0x5E) - Sensor Infrarrojo activado.");
        break;
    case 0x5A: // Error: Arranque sin configuración
        // Aseguramos que el estado vuelva/siga en LISTO
        setEstado(Listo);
        ui->txtLog->append("!! Micro: ERROR (0x5A) - Intento de arranque rechazado. No se ha asignado configuración a las salidas.");
        break;
    }
}

// --- SLOTS (INTERFAZ) ---

void MainWindow::on_btnConectar_clicked() {
    // Si el puerto YA está abierto, lo cerramos (Desconectar)
    if (serial->isOpen()) {
        serial->close();
        ui->btnConectar->setText("Conectar COM3");
        setEstado(Desconectado); // Volvemos la interfaz a gris
        ui->txtLog->append("SISTEMA: Puerto COM3 cerrado. Sistema desconectado.");
    }
    // Si el puerto está cerrado, intentamos abrirlo (Conectar)
    else {
        serial->setPortName("COM3"); // Asegurate de que sea el puerto correcto

        if (serial->open(QIODevice::ReadWrite)) {
            // ¡No deshabilitamos el botón! Solo le cambiamos el texto
            ui->btnConectar->setText("Desconectar");
            ui->txtLog->append("SISTEMA: Puerto COM3 abierto. Esperando señal del Micro...");
            // Nota: El estado cambiará a 'Listo' (Azul) recién cuando llegue el 0xF0
        } else {
            ui->txtLog->append("!! ERROR: No se pudo abrir el puerto COM3. ¿Está conectado?");
        }
    }
}


void MainWindow::enviarComando(uint8_t cmd, QByteArray payload) {
    if(!serial->isOpen()) return;

    QByteArray frame;
    uint8_t cks = 0;
    uint8_t len = 2 + payload.size();

    frame.append('U'); frame.append('N'); frame.append('E'); frame.append('R');
    frame.append(len);
    frame.append(':');
    frame.append(cmd);

    for(int i=0; i<frame.size(); i++) cks ^= (uint8_t)frame[i];

    if(!payload.isEmpty()) {
        frame.append(payload);
        for(int i=0; i<payload.size(); i++) cks ^= (uint8_t)payload[i];
    }

    frame.append(cks);
    serial->write(frame);
    ui->txtLog->append("TX -> " + frame.toHex(' ').toUpper());
}

void MainWindow::readSerial() {
    QByteArray bufferIn = serial->readAll();
    ui->txtLog->append("RX <- " + bufferIn.toHex(' ').toUpper());

    static QByteArray tramaBuffer;
    tramaBuffer.append(bufferIn);

    while (tramaBuffer.size() >= 8) {
        int index = tramaBuffer.indexOf("UNER");

        if (index == -1) {
            tramaBuffer.clear();
            break;
        }

        if (index > 0) {
            tramaBuffer.remove(0, index);
        }

        if (tramaBuffer.size() < 6) break;

        uint8_t lenField = (uint8_t)tramaBuffer.at(4);
        int tramaTotalEsperada = 6 + lenField;

        if (tramaBuffer.size() < tramaTotalEsperada) break;

        uint8_t cksCalculado = 0;
        for (int j = 0; j < tramaTotalEsperada - 1; ++j) {
            cksCalculado ^= (uint8_t)tramaBuffer.at(j);
        }

        uint8_t cksRecibido = (uint8_t)tramaBuffer.at(tramaTotalEsperada - 1);

        if (cksCalculado == cksRecibido) {
            uint8_t cmd = (uint8_t)tramaBuffer.at(6);
            procesarComandoMicro(cmd);
        } else {
            if(ui->tabWidget->currentIndex() == 0) ui->txtLog->append("!! Error Checksum: Recibido " + QString::number(cksRecibido, 16) +" vs Calc " + QString::number(cksCalculado, 16));
            if(ui->tabWidget->currentIndex() == 1) ui->txtLogMicro->append("!! Error Checksum: Recibido " + QString::number(cksRecibido, 16) +" vs Calc " + QString::number(cksCalculado, 16));
        }

        tramaBuffer.remove(0, tramaTotalEsperada);
    }
}


void MainWindow::on_SendConfigOut_clicked()
{
    bool okvel, ok0, ok1, ok2;

    int vel = ui->Vel->text().toInt(&okvel);
    int val0 = ui->Out0Config->text().toInt(&ok0);
    int val1 = ui->Out1Config->text().toInt(&ok1);
    int val2 = ui->Out2Config->text().toInt(&ok2);

    if (ok0 && ok1 && ok2 && vel) {
        QByteArray payload;
        payload.append((char)vel);
        payload.append((char)val0);
        payload.append((char)val1);
        payload.append((char)val2);

        enviarComando(0x40, payload);
        // NO cambiamos el estado acá, esperamos el ACK 0x40
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio de configuración...");
    } else {
        ui->txtLogMicro->append("!! ERROR: Ingrese números válidos en la configuración de salidas.");
    }
}


void MainWindow::on_btnSimIr0_clicked()
{
    QByteArray payload;
    payload.append((char)0x00);
    payload.append((char)0x01);

    enviarComando(0x5E, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Activando IR0 manual...");
}

void MainWindow::on_btnSimIr1_clicked()
{
    QByteArray payload;
    payload.append((char)0x01);
    payload.append((char)0x01);

    enviarComando(0x5E, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Activando IR1 manual...");
}

void MainWindow::on_btnSimIr2_clicked()
{
    QByteArray payload;
    payload.append((char)0x02);
    payload.append((char)0x01);

    enviarComando(0x5E, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Activando IR2 manual...");
}

void MainWindow::on_btnStart_clicked() {
    if((m_estadoActual == Listo || m_estadoActual == Detenido) && m_estadoActual != Corriendo) {
        enviarComando(0x50, QByteArray());
        // NO cambiamos a Verde acá, esperamos el ACK 0x50
        ui->txtLog->append("SISTEMA: Solicitando Inicio de Cinta...");
    }
}

void MainWindow::on_btnStop_clicked() {
    if(m_estadoActual == Corriendo) {
        enviarComando(0x51, QByteArray());
        // NO cambiamos a Naranja acá, esperamos el ACK 0x51
        ui->txtLog->append("SISTEMA: Solicitando Parada...");
    }
}

void MainWindow::on_btnReset_clicked() {
    enviarComando(0x53, QByteArray());
    // NO cambiamos a Gris acá, esperamos el ACK 0x53
    ui->txtLog->append("SISTEMA: Solicitando Reset Integral...");
}
void MainWindow::on_btnSimServo1_clicked()
{
    QByteArray payload;
    payload.append((char)0x02); // Bit 1 (Servo 1)
    payload.append((char)0x02); // Extender
    enviarComando(0x52, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Pateando Servo 1...");
}

