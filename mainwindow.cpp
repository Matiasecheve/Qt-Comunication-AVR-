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

void MainWindow::procesarComandoMicro(uint8_t cmd) {
    switch (cmd) {
    case 0xF0: // Handshake / Alive
        // SOLO cambiamos a 'Listo' si NO estamos ya corriendo.
        // Esto evita que un mensaje de "estoy vivo" del micro nos resetee la UI.
        if (m_estadoActual != Corriendo) {
            setEstado(Listo);
        }
        ui->txtLog->append(">> Micro: Handshake/Alive recibido.");
    break;

    case 0x50: // Start / Config (Confirmación de que arrancó)
        setEstado(Corriendo);
        ui->txtLog->append(">> Micro: Confirmación de inicio recibida.");
        break;
    case 0x51: // Stop
        setEstado(Detenido);
        ui->txtLog->append(">> Micro: Parada solicitada.");
        break;
    case 0x53: // Reset
        setEstado(Desconectado);
        ui->btnConectar->setEnabled(true);
        ui->txtLog->append(">> Micro: Reset integral ejecutado.");
        break;
    case 0x5F: // Nueva Caja
        ui->txtLog->append(">> Micro: Caja detectada en cinta.");
        break;
    }
}

// --- SLOTS (INTERFAZ) ---

void MainWindow::on_btnConectar_clicked() {
    serial->setPortName("COM3"); // Asegurate de que sea el puerto correcto
    if (serial->open(QIODevice::ReadWrite)) {
        ui->btnConectar->setEnabled(false);
        ui->txtLog->append("SISTEMA: Puerto abierto. Esperando señal del Micro...");
        // No llamamos a setEstado(Listo) acá, esperamos el 0xF0 del AVR
    }
}

void MainWindow::on_btnStart_clicked() {
    if((m_estadoActual == Listo || m_estadoActual == Detenido) && m_estadoActual != Corriendo) {
        enviarComando(0x50);
    }
}

void MainWindow::on_btnStop_clicked() {
    if(m_estadoActual == Corriendo) {
        enviarComando(0x51);
    }
}

void MainWindow::on_btnReset_clicked() {
    enviarComando(0x53);
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

    // El checksum XOR se calcula sobre la cabecera y el comando
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
    tramaBuffer.append(bufferIn); // Agregamos lo nuevo al remanente

    while (tramaBuffer.size() >= 8) { // Una trama mínima (sin payload) tiene 8 bytes
        // Buscamos la cabecera "UNER"
        int index = tramaBuffer.indexOf("UNER");

        if (index == -1) {
            tramaBuffer.clear(); // No hay cabecera, limpiamos
            break;
        }

        if (index > 0) {
            tramaBuffer.remove(0, index); // Descartamos basura antes de "UNER"
        }

        if (tramaBuffer.size() < 6) break; // Esperamos a tener al menos el campo LEN

        // Según tu AVR: LEN = 2 (CMD + Token) + payload_len
        // Tamaño total = 4 (UNER) + 1 (LEN) + 1 (:) + (LEN) + 1 (CKS) = 7 + LEN
        // Pero como LEN ya incluye el CMD, la cuenta real es:
        uint8_t lenField = (uint8_t)tramaBuffer.at(4);
        int tramaTotalEsperada = 6 + lenField;

        if (tramaBuffer.size() < tramaTotalEsperada) break; // Trama incompleta, esperamos

        // Validación de Checksum
        uint8_t cksCalculado = 0;
        for (int j = 0; j < tramaTotalEsperada - 1; ++j) {
            cksCalculado ^= (uint8_t)tramaBuffer.at(j);
        }

        uint8_t cksRecibido = (uint8_t)tramaBuffer.at(tramaTotalEsperada - 1);

        if (cksCalculado == cksRecibido) {
            uint8_t cmd = (uint8_t)tramaBuffer.at(6);
            procesarComandoMicro(cmd);
        } else {
            ui->txtLog->append("!! Error Checksum: Recibido " + QString::number(cksRecibido, 16) +
                               " vs Calc " + QString::number(cksCalculado, 16));
        }

        tramaBuffer.remove(0, tramaTotalEsperada); // Limpiamos la trama procesada
    }
}