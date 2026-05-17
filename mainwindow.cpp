#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lblTiempoS0->hide();
    ui->lblTiempoS1->hide();
    ui->lblTiempoS2->hide();


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

void MainWindow::procesarComandoMicro(uint8_t cmd, QByteArray payload) {
    switch (cmd) {
    case 0xF0: // Handshake / Alive
        if (m_estadoActual != Corriendo) {
            setEstado(Listo);
        }
        ui->txtLog->append(">> Micro: ACK (0xF0) - Sistema en línea y conectado.");
        break;

    case 0x40: // Configuración OK
        if (payload.size() >= 3) {
            // Desenmascarado seguro forzando a entero de 8 bits (quint8)
            ui->txtLogMicro->append(">> Micro: ACK (0x40) - Salidas seteadas -> S0: " +
                                    QString::number(static_cast<quint8>(payload[0])) + "cm | S1: " +
                                    QString::number(static_cast<quint8>(payload[1])) + "cm | S2: " +
                                    QString::number(static_cast<quint8>(payload[2])) + "cm.");
        }
        break;
    case 0x50: // Start / Confirmación de que arrancó
        setEstado(Corriendo);
        ui->txtLog->append(">> Micro: ACK (0x50) - cinta en movimiento.");
        break;

    case 0x51: // Stop
        setEstado(Detenido);
        ui->txtLog->append(">> Micro: ACK (0x51) -  cinta detenida.");
        break;

    case 0x53: // Reset
        setEstado(Listo); // CAMBIO CLAVE: Antes decía Desconectado
        ui->txtLog->append(">> Micro: ACK (0x53) - Memoria borrada y hardware reiniciado.");
        break;

    case 0x5E: // Evento Sensor Físico
        ui->txtLog->append(">> Micro: Evento (0x5E) - Sensor Infrarrojo activado.");
        break;

    case 0x5A: // Error: Arranque sin configuración
        setEstado(Listo);
        ui->txtLog->append("!! Micro: ERROR (0x5A) - Intento de arranque rechazado. No se ha asignado configuración a las salidas.");
        break;

    case 0x70: {
        QString texto = QString::fromLatin1(payload);

        ui->txtLogMicro->append("INFO: " + texto);

        actualizarQueues(texto);

        break;
    }

    case 0x58: // Cinemática del Modo Tiempo (Velocidad + Tipo de Caja + FIFOs)
        if(payload.size() >= 5) {
            quint8 velocidad = static_cast<quint8>(payload[0]);
            quint8 tipoCaja  = static_cast<quint8>(payload[1]);
            quint8 countS0   = static_cast<quint8>(payload[2]);
            quint8 countS1   = static_cast<quint8>(payload[3]);
            quint8 countS2   = static_cast<quint8>(payload[4]);

            // 1. Loggeamos el evento (Mensaje genérico porque llega al entrar y al patear)
            ui->txtLogMicro->append(">> Micro: Cinemática (0x58) - Actualizando FIFOs y Velocidad.");

            // 2. Actualizamos la etiqueta de Velocidad
            ui->lblVelocidadCinta->setText("Velocidad de la cinta: " + QString::number(velocidad) + " cm/s");

            // 3. Generador rápido para dibujar la cola de patadas (Máximo 5 elementos)
            auto generarVistaCola = [](quint8 count) -> QString {
                QString vista = "[ ";
                for(int i = 0; i < 5; i++) {
                    if(i < count) vista += "X "; // Caja en vuelo agendada
                    else vista += "- ";          // Espacio libre en la FIFO
                }
                vista += "]";
                return vista;
            };

            // 4. Actualizamos visualmente los labels de las Zonas (que en Modo Tiempo son las FIFOs)
            ui->lblQueue0->setText("S0: " + generarVistaCola(countS0));
            ui->lblQueue1->setText("S1: " + generarVistaCola(countS1));
            ui->lblQueue2->setText("S2: " + generarVistaCola(countS2));
        }
        break;
    case 0x55:
        if(payload.size() >= 2) {
            // Desenmascarado seguro usando quint8
            uint16_t tiempoConfirmado = static_cast<quint8>(payload[0]) | (static_cast<quint8>(payload[1]) << 8);
            ui->txtLogMicro->append(">> Micro: ACK (0x55) - Tiempo de reacción configurado en " + QString::number(tiempoConfirmado) + " ms.");
        }
    break;
    case 0x57:
        if(payload.size() >= 2) {
            // Desenmascarado seguro usando quint8
            uint16_t tiempoWaitConfirmado = static_cast<quint8>(payload[0]) | (static_cast<quint8>(payload[1]) << 8);
            ui->txtLogMicro->append(">> Micro: ACK (0x57) - Tiempo de espera de medición configurado en " + QString::number(tiempoWaitConfirmado) + " ms.");
        }
        break;
    case 0x59: // ACK de Cambio de Modo
        if (payload.size() >= 1) {
            if (payload[0] == 0x00) {
                ui->txtLogMicro->append(">> Micro: ACK (0x59) - Cinta operando en MODO TIEMPO (Ignorando IR de zonas).");
            } else if (payload[0] == 0x01) {
                ui->txtLogMicro->append(">> Micro: ACK (0x59) - Cinta operando en MODO INFRARROJO (Usando IR de zonas).");
            }
        }
        break;
    case 0x52: // Confirmación de estado de Actuadores
        if(payload.size() >= 2) {
            quint8 mask = static_cast<quint8>(payload[0]);
            quint8 pos  = static_cast<quint8>(payload[1]);

            for(int i=0; i < 3; i++) {
                if(mask & (1 << i)) {
                    if(pos & (1 << i)) {
                        ui->txtLogMicro->append(">> Micro: Evento (0x52) - Servo S" + QString::number(i) + " PATEANDO caja.");
                    }
                }
            }
        }
        break;
    }
}
// --- SLOTS (INTERFAZ) ---

void MainWindow::on_btnConectar_clicked(){
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

    // RESTAURADO: La librería espera que LEN sea 2 + el tamaño del payload
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

    // 1. Mostrar la trama binaria cruda (Hexadecimal) en el Log Principal
    ui->txtLog->append("RX <- " + bufferIn.toHex(' ').toUpper());

    // 2. Traducir a ASCII y mostrarlo en el Log del Micro
    QString textoTraducido = formatearAscii(bufferIn);

    // Solo imprimimos si el texto tiene algo más que puros puntos vacíos
    if (!textoTraducido.trimmed().isEmpty() && textoTraducido.contains(QRegularExpression("[A-Za-z0-9]"))) {
        ui->txtLogMicro->append("DEBUG: " + textoTraducido.trimmed());
    }

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

        // RESTAURADO: El tamaño total es 6 (Header) + LEN
        int tramaTotalEsperada = 6 + lenField;

        if (tramaBuffer.size() < tramaTotalEsperada) break;

        uint8_t cksCalculado = 0;
        for (int j = 0; j < tramaTotalEsperada - 1; ++j) {
            cksCalculado ^= (uint8_t)tramaBuffer.at(j);
        }

        uint8_t cksRecibido = (uint8_t)tramaBuffer.at(tramaTotalEsperada - 1);

        if (cksCalculado == cksRecibido) {
            uint8_t cmd = (uint8_t)tramaBuffer.at(6);

            // LA CORRECCIÓN DEFINITIVA:
            // Si LEN = 2 + payload, entonces el payload mide LEN - 2.
            QByteArray payload = tramaBuffer.mid(7, lenField - 2);

            // Le pasamos el comando y el contenido a la función
            procesarComandoMicro(cmd, payload);
        }

        tramaBuffer.remove(0, tramaTotalEsperada);
    }
}


void MainWindow::on_SendConfigOut_clicked()
{
    bool ok0, ok1, ok2;

    int val0 = ui->Out0Config->text().toInt(&ok0);
    int val1 = ui->Out1Config->text().toInt(&ok1);
    int val2 = ui->Out2Config->text().toInt(&ok2);

    if (ok0 && ok1 && ok2) {
        QByteArray payload;
        payload.append((char)val0);
        payload.append((char)val1);
        payload.append((char)val2);

        enviarComando(0x40, payload);
        // NO cambiamos el estado acá, esperamos el ACK 0x40
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio de configuración de salidas...");
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

void MainWindow::on_btnSimServo0_clicked()
{
    QByteArray payload;
    payload.append((char)0x01); // Máscara: Bit 0 activado (Servo 0)
    payload.append((char)0x01); // Posición: Bit 0 en '1' (Extender)
    enviarComando(0x52, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Pateando Servo 0...");
}

void MainWindow::on_btnSimServo1_clicked()
{
    QByteArray payload;
    payload.append((char)0x02); // Máscara: Bit 1 activado (Servo 1)
    payload.append((char)0x02); // Posición: Bit 1 en '1' (Extender)
    enviarComando(0x52, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Pateando Servo 1...");
}

void MainWindow::on_btnSimServo2_clicked()
{
    QByteArray payload;
    payload.append((char)0x04); // Máscara: Bit 2 activado (Servo 2)
    payload.append((char)0x04); // Posición: Bit 2 en '1' (Extender)
    enviarComando(0x52, payload);
    ui->txtLogMicro->append("SIMULACIÓN: Pateando Servo 2...");
}

QString MainWindow::formatearAscii(const QByteArray &datos) {
    QString resultado;
    for (char c : datos) {
        // Dejamos pasar solo el rango ASCII imprimible (letras, números, símbolos)
        // y los saltos de línea (\n).
        if ((c >= 32 && c <= 126) || c == '\n') {
            resultado.append(c);
        } else {
            // Si es un comando del protocolo (como 0xF0 o 0x50)
            // o un retorno de carro (\r), ponemos un punto.
            resultado.append('.');
        }
    }
    return resultado;
}

void MainWindow::on_sendVel_clicked()
{

    bool ok;

    int vel = ui->textvel->text().toInt(&ok);

    if (ok) {
        QByteArray payload;
        payload.append((char)vel);
        enviarComando(0x54, payload);
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio de configuración en la velocidad ...");
    } else {
        ui->txtLogMicro->append("!! ERROR: Ingrese números válidos en la configuración de salidas.");
    }
}

void MainWindow::on_sendTimeout_clicked()
{
    bool ok;
    int timeout = ui->textTimeout->text().toInt(&ok);

    if(ok){
        QByteArray payload;

        // Empaquetado seguro forzando enteros sin signo (evita valores negativos)
        quint8 byteLow = timeout & 0xFF;
        quint8 byteHigh = (timeout >> 8) & 0xFF;

        payload.append(static_cast<char>(byteLow));
        payload.append(static_cast<char>(byteHigh));

        enviarComando(0x55, payload);
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio de configuración en el Tiempo de reacción...");
    } else {
        ui->txtLogMicro->append("!! ERROR: Ingrese números válidos en la configuración de Tiempo de reacción.");
    }
}

void MainWindow::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    QByteArray payload;

    if(arg1 == Qt::Checked){
        ui->txtLogMicro->append("CUIDADO: Estas forzando el tiempo de patada. Esto puede dar lugar a comportamiento no deseado.");
        payload.append((char)0x01); // 1 = Modo Manual
    } else {
        ui->txtLogMicro->append("Timeout automático seteado.");
        payload.append((char)0x00); // 0 = Modo Automático
    }

    enviarComando(0x56, payload); // CMD 0x56 para toggle manual/auto
}
void MainWindow::on_sendWaitCenter_clicked()
{
    bool ok;
    int timeWait = ui->textWaitCenter->text().toInt(&ok);

    if(ok){
        QByteArray payload;

        // Empaquetado seguro forzando enteros sin signo (evita valores negativos)
        quint8 byteLow = timeWait & 0xFF;
        quint8 byteHigh = (timeWait >> 8) & 0xFF;

        payload.append(static_cast<char>(byteLow));
        payload.append(static_cast<char>(byteHigh));

        enviarComando(0x57, payload);
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio de configuración en el Retardo del HCSR04...");
    } else {
        ui->txtLogMicro->append("!! ERROR: Ingrese números válidos en la configuración de Retardo.");
    }
}

void MainWindow::actualizarQueues(const QString &texto)
{
    QString linea = texto.trimmed();

    if(linea.startsWith("Q0:")) {
        ui->lblQueue0->setText(linea);
    }
    else if(linea.startsWith("Q1:")) {
        ui->lblQueue1->setText(linea);
    }
    else if(linea.startsWith("Q2:")) {
        ui->lblQueue2->setText(linea);
    }
}

void MainWindow::on_chkTimeMode_checkStateChanged(const Qt::CheckState &arg1)
{
    QByteArray payload;

    if(arg1 == Qt::Checked){
        // Si se marca, pedimos Modo Tiempo (0x00)
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio a Modo Tiempo (Lazo Abierto)...");
        payload.append(static_cast<char>(0x00));

        // --- CAMBIO VISUAL PARA MODO TIEMPO ---
        ui->lblTitleQ0->setText("Queue S0 (Modo Tiempo)");
        ui->lblTitleQ1->setText("Queue S1 (Modo Tiempo)");
        ui->lblTitleQ2->setText("Queue S2 (Modo Tiempo)");

        // CONTENIDO DEFAULT: Inicializamos las FIFOs de patadas vacías [ - - - - - ]
        ui->lblQueue0->setText("S0: [ - - - - - ]");
        ui->lblQueue1->setText("S1: [ - - - - - ]");
        ui->lblQueue2->setText("S2: [ - - - - - ]");

        // Mostramos las etiquetas de tiempo de los servos
        ui->lblTiempoS0->show();
        ui->lblTiempoS1->show();
        ui->lblTiempoS2->show();

    } else {
        // Si se desmarca, pedimos Modo Infrarrojo (0x01)
        ui->txtLogMicro->append("SISTEMA: Solicitando cambio a Modo Infrarrojo (Lazo Cerrado)...");
        payload.append(static_cast<char>(0x01));

        // --- RESTAURAR VISUAL PARA MODO INFRARROJO ---
        ui->lblTitleQ0->setText("Ir0 - Ir1");
        ui->lblTitleQ1->setText("Ir1 - Ir2");
        ui->lblTitleQ2->setText("Ir2 - Ir3");

        // CONTENIDO DEFAULT: Restauramos el formato de matriz de 10 ceros para Modo IR
        ui->lblQueue0->setText("Q0: [0 0 0 0 0 0 0 0 0 0]");
        ui->lblQueue1->setText("Q1: [0 0 0 0 0 0 0 0 0 0]");
        ui->lblQueue2->setText("Q2: [0 0 0 0 0 0 0 0 0 0]");

        // Ocultamos las etiquetas de tiempo de los servos
        ui->lblTiempoS0->hide();
        ui->lblTiempoS1->hide();
        ui->lblTiempoS2->hide();
    }

    // Usamos el comando 0x59 para el cambio de modo
    enviarComando(0x59, payload);
}