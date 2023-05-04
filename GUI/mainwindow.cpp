
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include "lib/Avl.h"
#include "lib/Hash.h"
#include "lib/Secuential.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    //Conectamos funciones con botones
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::handleButtonClick);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_buttonEnviar_clicked);
    connect(ui->pushButton_registrar, &QPushButton::clicked, this, &MainWindow::registrar);
    connect(ui->pushButton_eliminar, &QPushButton::clicked, this, &MainWindow::eliminar);
    connect(ui->pushButton_buscar, &QPushButton::clicked, this, &MainWindow::buscar);
}

MainWindow::~MainWindow()
{
    delete ui;
}


