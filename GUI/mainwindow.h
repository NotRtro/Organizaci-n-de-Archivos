
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLine>
#include <QListWidget>
#include "ui_mainwindow.h"

#include <iostream>
#include "lib/Avl.h"
#include "lib/Hash.h"
#include "lib/Secuential.h"
#include "vector"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void buscar(){
        Hash temp("C:/Users/JEFF QUINTANA/Documents/untitled1/Data_from_Structurs/dataHash.dat");
        string key = ui->lineEdit_buscar->text().toStdString();
        QLabel* lerror = findChild<QLabel*>("label_buscar_error");
        try{

            if(temp.search(key)){
                qDebug() << "Llave encontrada";
                lerror->setText("Llave encontrada");
            }else{
                qDebug() << "Llave no encontrada";
                lerror->setText("Llave no encontrada");
            }



        }catch(const std::exception& e){
            qDebug() << "Ocurrió un error: " << e.what();
            lerror->setText("ERROR");
        }

    }

    void eliminar(){
        Hash temp("C:/Users/JEFF QUINTANA/Documents/untitled1/Data_from_Structurs/dataHash.dat");

        string key = ui->lineEdit_eliminar->text().toStdString();
        QLabel* lerror = findChild<QLabel*>("label_eliminar_error");
        try{
            temp.remove(key);
            lerror->setText("Llave eliminada correctamente");
        }catch(const std::exception& e){
            qDebug() << "Ocurrió un error: " << e.what();
            lerror->setText("Llave no encontrada");
        }

    }

    void registrar(){
        Hash temp("C:/Users/JEFF QUINTANA/Documents/untitled1/Data_from_Structurs/dataHash.dat");

        string codigo = ui->lineEdit_codigo->text().toStdString();
        string prenda = ui->lineEdit_prenda->text().toStdString();
        string genero = ui->lineEdit_genero->text().toStdString();
        int precio = ui->lineEdit_precio->text().toInt();
        int stock = ui->lineEdit_stock->text().toInt();
        string marca = ui->lineEdit_marca->text().toStdString();

        RecordHash record(codigo, prenda, genero, precio, stock, marca);
        temp.set(record);
        record.display();

    }

    void on_buttonEnviar_clicked(){
        QString contenido = ui->lineEdit->text();
        int key = contenido.toInt();

        Hash temp("C:/Users/JEFF QUINTANA/Documents/untitled1/Data_from_Structurs/dataHash.dat");

        QListWidget* lista = findChild<QListWidget*>("listWidget");

        try{
            vector<RecordHash> hash = temp.get(key);
            for (auto & i : hash) {
                i.display();
                lista->addItem(i.cod);
            }
        }catch(const std::exception& e){
            qDebug() << "Ocurrió un error: " << e.what();
            QLabel* lerror = findChild<QLabel*>("label_error");
            lerror->setText("No hay elementos en el bucked");
        }






        //QPixmap pixmap("C:/Users/JEFF QUINTANA/Desktop/Data_from_Structurs/vaca.jpg");
        //QCursor cursor(pixmap, pixmap.width() / 2, pixmap.height() / 2);
        //setCursor(cursor);
        //QApplication::setOverrideCursor(cursor);
    }

    void handleButtonClick() {
        // Aquí es donde se ejecutará la función cuando se presione el botón

        Hash temp("C:/Users/JEFF QUINTANA/Documents/untitled1/Data_from_Structurs/dataHash.dat");

        RecordHash jeff1("1111111", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff2("2222222", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff3("3333333", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff4("4444444", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff5("5555555", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff6("6666666", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff7("7777777", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff8("8888888", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff9("9999999", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff10("0000000", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff11("aaaaaaa", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff12("bbbbbbb", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff13("ccccccc", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff14("ddddddd", "Quintana", "m", 100.1, 6, "nikebaa");
        RecordHash jeff15("eeeeeee", "Quintana", "m", 100.1, 6, "nikebaa");
        //jeff.display();
        //temp.set(jeff1);
        //temp.set(jeff2);
        //temp.set(jeff3);
        //temp.set(jeff4);
        //temp.set(jeff5);
        //temp.set(jeff6);
        //temp.set(jeff7);
        //temp.set(jeff8);
        //temp.set(jeff9);
        //temp.set(jeff10);
        //temp.set(jeff11);
        //temp.set(jeff12);
        //temp.set(jeff13);
        //temp.set(jeff14);
        //temp.set(jeff15);

        qDebug() << temp.getHashCode("1111111")%15;
        vector<RecordHash> hash = temp.get(6);
        for (auto & i : hash) {
                    i.display();
                    cout<<endl;
                }

    }
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
