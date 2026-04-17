#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> //Importamos el reloj
#include <QGraphicsScene> //Importamos el lienzo
#include "ecosistema.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_btnIniciar_clicked();

    void on_btnPausar_clicked();

    void on_btnReiniciar_clicked();

    void actualizarSimulacion();


private:
    Ui::MainWindow *ui;

    //Toolbox
    QTimer *timer; //un puntero a reloj
    Ecosystemboard *board; //puntero a la grilla
    QGraphicsScene *escena; // puntero al linzo de dibuji
    QBrush brushPasto;
    QBrush brushPlanta;
    QBrush brushHervivoro;
    QBrush brushCarnivoro;


    void poblarEcosistema(); //Para poner los animales
    void dibujarBoard(); //Para traducir la matriz a imagenes
};
#endif // MAINWINDOW_H
