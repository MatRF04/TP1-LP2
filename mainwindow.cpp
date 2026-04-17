#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Conectamos el codigo con el cuadro grafico
    escena = new QGraphicsScene(this);
    ui->vistaEcosistema->setScene(escena);

    //Creamos la matriz osea la grilla
    board = new Ecosystemboard(15,15);

    //Creamos el reloj y le decimos cuando hagas timeout, actualiza el tablero
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);

    //Ponemos algunos animlaes inciales
    poblarEcosistema();
    dibujarBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete board;
}

void MainWindow::on_btnIniciar_clicked()
{
    timer->start(500);

}


void MainWindow::on_btnPausar_clicked()
{
    timer->stop();

}


void MainWindow::on_btnReiniciar_clicked()
{
    timer->stop();

    ui->btnIniciar->setEnabled(true);
    ui->btnPausar->setEnabled(true);

    //borramos el tablero viejo y creamos uno nuevo vacio
    delete board;
    board = new Ecosystemboard(15,15);

    // volvemos a meter los animales
    poblarEcosistema();
    dibujarBoard();

    //Reinicio de label
    ui->lblCiclo->setText("Ciclo: 0");

}

void MainWindow::actualizarSimulacion() {
    // Le decimos a la logica que calcule los movimientos
    board->nextCycle();

    // Borramos la pantalla y dibujamos las nuevas posiciones
    dibujarBoard();
}

void MainWindow::poblarEcosistema() {
    board->addEntity(new Plant(5,5,20));
    board->addEntity(new Plant(10,10,20));
    board->addEntity(new Hervivoro(4,5,100));
    board->addEntity(new Carnivoro(14,14,150));
    board->addEntity(new Hervivoro(13,13,70));
    board->addEntity(new Carnivoro(4,3,100));
    board->addEntity(new Hervivoro(8,10,50));
    board->addEntity(new Plant(1,1,50));
    board->addEntity(new Plant(4,11,40));
}

void MainWindow::dibujarBoard() {
    escena->clear(); //Barramos el dibujo

    int pixel = 60; //cada cuadro medira 60 pixeles

    //Cargamos imagen de fondo para la celda

    brushPasto = QBrush(QPixmap(":/img/Pasto.png").scaled(pixel, pixel, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)); //La escalo al tamaño de cada cuadro, acemos que se adapte al pixel y que no pierda calidad

    //Cargamos imagen de planta
    brushPlanta = QBrush(QPixmap(":/img/Planta.png").scaled(pixel,pixel, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //Cargamos imagen de hervivoro

    brushHervivoro = QBrush(QPixmap(":/img/Hervioro.png").scaled(pixel,pixel, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //Cargamos imagen de carnivoro
    brushCarnivoro = QBrush(QPixmap(":/img/Carnivoro.png").scaled(pixel,pixel, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    auto grid = board->getGrid(); //Traemos la matriz

    int contPlantas = 0; //Contador de plantas
    int contHervivoro = 0; //Contador de Hervivoros
    int contCarnivoro = 0; //Contador de Carnivoros

    //Recorremos la matriz
    for (int y = 0; y < board->getHeight(); y++) {
        for(int x = 0; x < board->getWidth(); x++){

            //Calculamos los pixeles, si x=5 entonces se dibuja en el pixel 100
            int posX = x * pixel;
            int posY = y * pixel;

            //Dibujamos el fondo con una imagen osea el pasto
            escena->addRect(posX, posY, pixel, pixel, QPen(Qt::transparent), brushPasto);

            // Le preguntamos a la matriz, hay un puntero en esta celda?
            EntidadViva* entities = grid[y][x];

            if (entities != nullptr) {
                QBrush brushEntities;

                if (dynamic_cast<Plant*>(entities)) {
                    brushEntities = brushPlanta;
                    contPlantas++;
                } else if (dynamic_cast<Hervivoro*>(entities)) {
                    brushEntities = brushHervivoro;
                    contHervivoro++;
                } else if (dynamic_cast<Carnivoro*>(entities)) {
                    brushEntities = brushCarnivoro;
                    contCarnivoro++;
                }

                //Pintamos el cuadrito de color encima del pasto
                escena->addRect(posX,posY,pixel,pixel,QPen(Qt::black),brushEntities);


            }
        }
    }

    int totalEntidades = contCarnivoro + contPlantas + contHervivoro;
    int MaxEntidades = board->getWidth() * board->getHeight();




    //Al termino de dibujo del mapa, actualizamos los labels
    ui->lblCiclo->setText("Ciclo actual: " + QString::number(board->getCycle()));
    ui->lblPlanta->setText("Plantas: " + QString::number(contPlantas));
    ui->lblHervivoro->setText("Hervivoros: " + QString::number(contHervivoro));
    ui->lblCarnivoro->setText("Carnivoro: " + QString::number(contCarnivoro));

    if (totalEntidades >= MaxEntidades){
        //Si se lleno el tablero que no se pueda hacer nada mas que reiniciar
        timer->stop();
        ui->btnIniciar->setEnabled(false);
        ui->btnPausar->setEnabled(false);

        ui->lblCiclo->setText("Ecosistema Lleno ");
    }

    //Calculamos el tamaño exacto en píxeles )
    int anchoTotal = board->getWidth() * pixel;
    int altoTotal = board->getHeight() * pixel;

    //Le decimos al lienzo cuánto mide la simulación
    escena->setSceneRect(0, 0, anchoTotal, altoTotal);

    //Forzamos a la cámara (vistaEcosistema) a tener el tamaño EXACTO de la grilla
    ui->vistaEcosistema->setFixedSize(anchoTotal, altoTotal);

    //Hacemos que la caja sea transparente para que se vea tu fondo de pantalla
    ui->vistaEcosistema->setStyleSheet("background: transparent; border: none;");

}




