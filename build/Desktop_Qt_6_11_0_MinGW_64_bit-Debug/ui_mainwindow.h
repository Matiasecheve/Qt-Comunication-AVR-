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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *v;
    QGroupBox *g1;
    QHBoxLayout *hboxLayout;
    QPushButton *btnConectar;
    QLabel *lblEstado;
    QGroupBox *g2;
    QHBoxLayout *hboxLayout1;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnReset;
    QTextEdit *txtLog;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(500, 450);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        v = new QVBoxLayout(centralwidget);
        v->setObjectName("v");
        g1 = new QGroupBox(centralwidget);
        g1->setObjectName("g1");
        hboxLayout = new QHBoxLayout(g1);
        hboxLayout->setObjectName("hboxLayout");
        btnConectar = new QPushButton(g1);
        btnConectar->setObjectName("btnConectar");

        hboxLayout->addWidget(btnConectar);

        lblEstado = new QLabel(g1);
        lblEstado->setObjectName("lblEstado");

        hboxLayout->addWidget(lblEstado);


        v->addWidget(g1);

        g2 = new QGroupBox(centralwidget);
        g2->setObjectName("g2");
        hboxLayout1 = new QHBoxLayout(g2);
        hboxLayout1->setObjectName("hboxLayout1");
        btnStart = new QPushButton(g2);
        btnStart->setObjectName("btnStart");

        hboxLayout1->addWidget(btnStart);

        btnStop = new QPushButton(g2);
        btnStop->setObjectName("btnStop");

        hboxLayout1->addWidget(btnStop);

        btnReset = new QPushButton(g2);
        btnReset->setObjectName("btnReset");

        hboxLayout1->addWidget(btnReset);


        v->addWidget(g2);

        txtLog = new QTextEdit(centralwidget);
        txtLog->setObjectName("txtLog");
        txtLog->setReadOnly(true);

        v->addWidget(txtLog);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        g1->setTitle(QCoreApplication::translate("MainWindow", "Conexi\303\263n", nullptr));
        btnConectar->setText(QCoreApplication::translate("MainWindow", "Conectar COM3", nullptr));
        lblEstado->setText(QCoreApplication::translate("MainWindow", "Desconectado", nullptr));
        g2->setTitle(QCoreApplication::translate("MainWindow", "Control", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        btnReset->setText(QCoreApplication::translate("MainWindow", "RESET", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
