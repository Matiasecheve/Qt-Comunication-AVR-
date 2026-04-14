/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_main;
    QTabWidget *tabWidget;
    QWidget *tabPrincipal;
    QVBoxLayout *verticalLayout_tab1;
    QGroupBox *g1;
    QHBoxLayout *horizontalLayout_g1;
    QPushButton *btnConectar;
    QLabel *lblEstado;
    QSpacerItem *horizontalSpacer_g1;
    QGroupBox *g2;
    QHBoxLayout *horizontalLayout_g2;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnReset;
    QSpacerItem *horizontalSpacer_g2;
    QTextEdit *txtLog;
    QWidget *tabSimulacion;
    QVBoxLayout *verticalLayout_tab2;
    QHBoxLayout *horizontalLayout_entry;
    QLabel *lblCaja;
    QLineEdit *txtCajaNum;
    QPushButton *btnSimEntry;
    QSpacerItem *horizontalSpacer_entry;
    QGroupBox *gSensores_3;
    QHBoxLayout *horizontalLayout_cfg;
    QLineEdit *Out0Config;
    QLineEdit *Out1Config;
    QLineEdit *Out2Config;
    QSpacerItem *horizontalSpacer_cfg;
    QPushButton *SendConfigOut;
    QGroupBox *gVelocidad_indep;
    QHBoxLayout *horizontalLayout_vel_indep;
    QLabel *lblTextVel;
    QLineEdit *textvel;
    QPushButton *sendVel;
    QSpacerItem *horizontalSpacer_vel_indep;
    QGroupBox *gTimeout_servo;
    QHBoxLayout *horizontalLayout_timeout;
    QLabel *lblTextTimeout;
    QLineEdit *textTimeout;
    QPushButton *sendTimeout;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer_timeout;
    QHBoxLayout *horizontalLayout_hardware;
    QGroupBox *gServos;
    QHBoxLayout *horizontalLayout_servos;
    QPushButton *btnSimServo0;
    QPushButton *btnSimServo1;
    QPushButton *btnSimServo2;
    QGroupBox *gSensores;
    QHBoxLayout *horizontalLayout_irs;
    QPushButton *btnSimIr0;
    QPushButton *btnSimIr1;
    QPushButton *btnSimIr2;
    QTextEdit *txtLogMicro;
    QWidget *tabAvanzado;
    QVBoxLayout *verticalLayout_tab3;
    QLabel *lblProximamente;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(507, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("/* Dise\303\261o SCADA / Industrial Dark Mode */\n"
"QWidget { background-color: #2b2b2b; color: #e0e0e0; font-family: \"Segoe UI\", sans-serif; }\n"
"\n"
"/* Estilo para las Pesta\303\261as */\n"
"QTabWidget::pane { border: 1px solid #444444; border-radius: 4px; }\n"
"QTabBar::tab { background: #3a3a3a; color: #aaaaaa; padding: 8px 20px; border: 1px solid #444; border-bottom: none; border-top-left-radius: 6px; border-top-right-radius: 6px; margin-right: 2px; }\n"
"QTabBar::tab:selected { background: #0275d8; color: white; font-weight: bold; }\n"
"QTabBar::tab:hover:!selected { background: #4a4a4a; }\n"
"\n"
"/* Cajas de Agrupaci\303\263n */\n"
"QGroupBox { border: 1px solid #444444; border-radius: 6px; margin-top: 15px; font-weight: bold; }\n"
"QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 5px; color: #5bc0de; }\n"
"\n"
"/* Botones Generales */\n"
"QPushButton { border-radius: 4px; padding: 6px 12px; font-weight: bold; color: white; border: none; background-color: #55"
                        "5555;}\n"
"QPushButton:hover { background-color: #777777; }\n"
"QPushButton:pressed { background-color: #333333; }\n"
"\n"
"/* Botones Espec\303\255ficos */\n"
"QPushButton#btnConectar { background-color: #0275d8; }\n"
"QPushButton#btnConectar:hover { background-color: #33b5e5; }\n"
"QPushButton#btnStart { background-color: #5cb85c; }\n"
"QPushButton#btnStart:hover { background-color: #449d44; }\n"
"QPushButton#btnStop { background-color: #d9534f; }\n"
"QPushButton#btnStop:hover { background-color: #c9302c; }\n"
"QPushButton#btnReset { background-color: #f0ad4e; }\n"
"QPushButton#btnReset:hover { background-color: #ec971f; }\n"
"\n"
"/* Cajas de Texto y Entradas */\n"
"QTextEdit, QLineEdit { background-color: #1e1e1e; color: #00ff00; border: 1px solid #444; border-radius: 4px; font-family: \"Consolas\", monospace; font-size: 12px; }\n"
"QLineEdit { color: #ffffff; font-size: 14px; font-weight: bold; padding-left: 5px;}\n"
"\n"
"QLabel#lblEstado { font-weight: bold; color: #f0ad4e; font-size: 13px; }\n"
"QLabel"
                        " { font-weight: bold; }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_main = new QVBoxLayout(centralwidget);
        verticalLayout_main->setObjectName("verticalLayout_main");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabPrincipal = new QWidget();
        tabPrincipal->setObjectName("tabPrincipal");
        verticalLayout_tab1 = new QVBoxLayout(tabPrincipal);
        verticalLayout_tab1->setObjectName("verticalLayout_tab1");
        g1 = new QGroupBox(tabPrincipal);
        g1->setObjectName("g1");
        horizontalLayout_g1 = new QHBoxLayout(g1);
        horizontalLayout_g1->setObjectName("horizontalLayout_g1");
        btnConectar = new QPushButton(g1);
        btnConectar->setObjectName("btnConectar");

        horizontalLayout_g1->addWidget(btnConectar);

        lblEstado = new QLabel(g1);
        lblEstado->setObjectName("lblEstado");

        horizontalLayout_g1->addWidget(lblEstado);

        horizontalSpacer_g1 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_g1->addItem(horizontalSpacer_g1);


        verticalLayout_tab1->addWidget(g1);

        g2 = new QGroupBox(tabPrincipal);
        g2->setObjectName("g2");
        horizontalLayout_g2 = new QHBoxLayout(g2);
        horizontalLayout_g2->setObjectName("horizontalLayout_g2");
        btnStart = new QPushButton(g2);
        btnStart->setObjectName("btnStart");

        horizontalLayout_g2->addWidget(btnStart);

        btnStop = new QPushButton(g2);
        btnStop->setObjectName("btnStop");

        horizontalLayout_g2->addWidget(btnStop);

        btnReset = new QPushButton(g2);
        btnReset->setObjectName("btnReset");

        horizontalLayout_g2->addWidget(btnReset);

        horizontalSpacer_g2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_g2->addItem(horizontalSpacer_g2);


        verticalLayout_tab1->addWidget(g2);

        txtLog = new QTextEdit(tabPrincipal);
        txtLog->setObjectName("txtLog");
        txtLog->setReadOnly(true);

        verticalLayout_tab1->addWidget(txtLog);

        tabWidget->addTab(tabPrincipal, QString());
        tabSimulacion = new QWidget();
        tabSimulacion->setObjectName("tabSimulacion");
        verticalLayout_tab2 = new QVBoxLayout(tabSimulacion);
        verticalLayout_tab2->setObjectName("verticalLayout_tab2");
        horizontalLayout_entry = new QHBoxLayout();
        horizontalLayout_entry->setObjectName("horizontalLayout_entry");
        lblCaja = new QLabel(tabSimulacion);
        lblCaja->setObjectName("lblCaja");

        horizontalLayout_entry->addWidget(lblCaja);

        txtCajaNum = new QLineEdit(tabSimulacion);
        txtCajaNum->setObjectName("txtCajaNum");
        txtCajaNum->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_entry->addWidget(txtCajaNum);

        btnSimEntry = new QPushButton(tabSimulacion);
        btnSimEntry->setObjectName("btnSimEntry");

        horizontalLayout_entry->addWidget(btnSimEntry);

        horizontalSpacer_entry = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_entry->addItem(horizontalSpacer_entry);


        verticalLayout_tab2->addLayout(horizontalLayout_entry);

        gSensores_3 = new QGroupBox(tabSimulacion);
        gSensores_3->setObjectName("gSensores_3");
        horizontalLayout_cfg = new QHBoxLayout(gSensores_3);
        horizontalLayout_cfg->setObjectName("horizontalLayout_cfg");
        Out0Config = new QLineEdit(gSensores_3);
        Out0Config->setObjectName("Out0Config");
        Out0Config->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_cfg->addWidget(Out0Config);

        Out1Config = new QLineEdit(gSensores_3);
        Out1Config->setObjectName("Out1Config");
        Out1Config->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_cfg->addWidget(Out1Config);

        Out2Config = new QLineEdit(gSensores_3);
        Out2Config->setObjectName("Out2Config");
        Out2Config->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_cfg->addWidget(Out2Config);

        horizontalSpacer_cfg = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_cfg->addItem(horizontalSpacer_cfg);

        SendConfigOut = new QPushButton(gSensores_3);
        SendConfigOut->setObjectName("SendConfigOut");

        horizontalLayout_cfg->addWidget(SendConfigOut);


        verticalLayout_tab2->addWidget(gSensores_3);

        gVelocidad_indep = new QGroupBox(tabSimulacion);
        gVelocidad_indep->setObjectName("gVelocidad_indep");
        horizontalLayout_vel_indep = new QHBoxLayout(gVelocidad_indep);
        horizontalLayout_vel_indep->setObjectName("horizontalLayout_vel_indep");
        lblTextVel = new QLabel(gVelocidad_indep);
        lblTextVel->setObjectName("lblTextVel");

        horizontalLayout_vel_indep->addWidget(lblTextVel);

        textvel = new QLineEdit(gVelocidad_indep);
        textvel->setObjectName("textvel");
        textvel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_vel_indep->addWidget(textvel);

        sendVel = new QPushButton(gVelocidad_indep);
        sendVel->setObjectName("sendVel");

        horizontalLayout_vel_indep->addWidget(sendVel);

        horizontalSpacer_vel_indep = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_vel_indep->addItem(horizontalSpacer_vel_indep);


        verticalLayout_tab2->addWidget(gVelocidad_indep);

        gTimeout_servo = new QGroupBox(tabSimulacion);
        gTimeout_servo->setObjectName("gTimeout_servo");
        horizontalLayout_timeout = new QHBoxLayout(gTimeout_servo);
        horizontalLayout_timeout->setObjectName("horizontalLayout_timeout");
        lblTextTimeout = new QLabel(gTimeout_servo);
        lblTextTimeout->setObjectName("lblTextTimeout");

        horizontalLayout_timeout->addWidget(lblTextTimeout);

        textTimeout = new QLineEdit(gTimeout_servo);
        textTimeout->setObjectName("textTimeout");
        textTimeout->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_timeout->addWidget(textTimeout);

        sendTimeout = new QPushButton(gTimeout_servo);
        sendTimeout->setObjectName("sendTimeout");

        horizontalLayout_timeout->addWidget(sendTimeout);

        checkBox = new QCheckBox(gTimeout_servo);
        checkBox->setObjectName("checkBox");

        horizontalLayout_timeout->addWidget(checkBox);

        horizontalSpacer_timeout = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_timeout->addItem(horizontalSpacer_timeout);


        verticalLayout_tab2->addWidget(gTimeout_servo);

        horizontalLayout_hardware = new QHBoxLayout();
        horizontalLayout_hardware->setObjectName("horizontalLayout_hardware");
        gServos = new QGroupBox(tabSimulacion);
        gServos->setObjectName("gServos");
        horizontalLayout_servos = new QHBoxLayout(gServos);
        horizontalLayout_servos->setObjectName("horizontalLayout_servos");
        btnSimServo0 = new QPushButton(gServos);
        btnSimServo0->setObjectName("btnSimServo0");

        horizontalLayout_servos->addWidget(btnSimServo0);

        btnSimServo1 = new QPushButton(gServos);
        btnSimServo1->setObjectName("btnSimServo1");

        horizontalLayout_servos->addWidget(btnSimServo1);

        btnSimServo2 = new QPushButton(gServos);
        btnSimServo2->setObjectName("btnSimServo2");

        horizontalLayout_servos->addWidget(btnSimServo2);


        horizontalLayout_hardware->addWidget(gServos);

        gSensores = new QGroupBox(tabSimulacion);
        gSensores->setObjectName("gSensores");
        horizontalLayout_irs = new QHBoxLayout(gSensores);
        horizontalLayout_irs->setObjectName("horizontalLayout_irs");
        btnSimIr0 = new QPushButton(gSensores);
        btnSimIr0->setObjectName("btnSimIr0");

        horizontalLayout_irs->addWidget(btnSimIr0);

        btnSimIr1 = new QPushButton(gSensores);
        btnSimIr1->setObjectName("btnSimIr1");

        horizontalLayout_irs->addWidget(btnSimIr1);

        btnSimIr2 = new QPushButton(gSensores);
        btnSimIr2->setObjectName("btnSimIr2");

        horizontalLayout_irs->addWidget(btnSimIr2);


        horizontalLayout_hardware->addWidget(gSensores);


        verticalLayout_tab2->addLayout(horizontalLayout_hardware);

        txtLogMicro = new QTextEdit(tabSimulacion);
        txtLogMicro->setObjectName("txtLogMicro");
        txtLogMicro->setReadOnly(true);

        verticalLayout_tab2->addWidget(txtLogMicro);

        tabWidget->addTab(tabSimulacion, QString());
        tabAvanzado = new QWidget();
        tabAvanzado->setObjectName("tabAvanzado");
        verticalLayout_tab3 = new QVBoxLayout(tabAvanzado);
        verticalLayout_tab3->setObjectName("verticalLayout_tab3");
        lblProximamente = new QLabel(tabAvanzado);
        lblProximamente->setObjectName("lblProximamente");
        lblProximamente->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_tab3->addWidget(lblProximamente);

        tabWidget->addTab(tabAvanzado, QString());

        verticalLayout_main->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simulador HMI - Cinta Clasificadora", nullptr));
        g1->setTitle(QCoreApplication::translate("MainWindow", "Conexi\303\263n UART", nullptr));
        btnConectar->setText(QCoreApplication::translate("MainWindow", "Conectar COM3", nullptr));
        lblEstado->setText(QCoreApplication::translate("MainWindow", "Desconectado", nullptr));
        g2->setTitle(QCoreApplication::translate("MainWindow", "Control de Hardware", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        btnReset->setText(QCoreApplication::translate("MainWindow", "RESET", nullptr));
        txtLog->setPlaceholderText(QCoreApplication::translate("MainWindow", ">_ Monitor General UART...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabPrincipal), QCoreApplication::translate("MainWindow", "Principal", nullptr));
        lblCaja->setText(QCoreApplication::translate("MainWindow", "Caja (cm):", nullptr));
        txtCajaNum->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        btnSimEntry->setText(QCoreApplication::translate("MainWindow", "Simular EntryBox", nullptr));
        gSensores_3->setTitle(QCoreApplication::translate("MainWindow", "Configurar Salidas", nullptr));
        Out0Config->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        Out1Config->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        Out2Config->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        SendConfigOut->setText(QCoreApplication::translate("MainWindow", "Enviar", nullptr));
        gVelocidad_indep->setTitle(QCoreApplication::translate("MainWindow", "Cambiar Velocidad (CMD 0x54)", nullptr));
        lblTextVel->setText(QCoreApplication::translate("MainWindow", "Velocidad:", nullptr));
        textvel->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        sendVel->setText(QCoreApplication::translate("MainWindow", "Enviar Vel", nullptr));
        gTimeout_servo->setTitle(QCoreApplication::translate("MainWindow", "Configurar Patada Servo (CMD 0x55)", nullptr));
        lblTextTimeout->setText(QCoreApplication::translate("MainWindow", "Tiempo (ms):", nullptr));
        textTimeout->setText(QCoreApplication::translate("MainWindow", "160", nullptr));
        sendTimeout->setText(QCoreApplication::translate("MainWindow", "Enviar Timeout", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "Forzar Timeout", nullptr));
        gServos->setTitle(QCoreApplication::translate("MainWindow", "Pateadores (Servos)", nullptr));
        btnSimServo0->setText(QCoreApplication::translate("MainWindow", "Patear S0", nullptr));
        btnSimServo1->setText(QCoreApplication::translate("MainWindow", "Patear S1", nullptr));
        btnSimServo2->setText(QCoreApplication::translate("MainWindow", "Patear S2", nullptr));
        gSensores->setTitle(QCoreApplication::translate("MainWindow", "Sensores IR F\303\255sicos", nullptr));
        btnSimIr0->setText(QCoreApplication::translate("MainWindow", "Activar IR0", nullptr));
        btnSimIr1->setText(QCoreApplication::translate("MainWindow", "Activar IR1", nullptr));
        btnSimIr2->setText(QCoreApplication::translate("MainWindow", "Activar IR2", nullptr));
        txtLogMicro->setPlaceholderText(QCoreApplication::translate("MainWindow", ">_ Monitor de Eventos Simulados y Retornos del Micro...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSimulacion), QCoreApplication::translate("MainWindow", "Inyecci\303\263n Manual", nullptr));
        lblProximamente->setText(QCoreApplication::translate("MainWindow", "Espacio para futuras configuraciones...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAvanzado), QCoreApplication::translate("MainWindow", "Avanzado", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
