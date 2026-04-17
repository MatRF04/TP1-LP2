#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Esto inicializa el sistema gráfico de Qt
    QApplication a(argc, argv);

    // Esto crea tu ventana (donde están tus botones y la grilla)
    MainWindow w;
    w.show();

    // Esto mantiene el programa abierto hasta que cierres la ventana
    return a.exec();
}