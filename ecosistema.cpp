#include "ecosistema.h"
#include <vector>
#include <cstdlib> // Para generar números aleatorios (rand)
#include <ctime>   // Para la semilla del tiempo
using namespace std;



// CONSTRUCTORES

EntidadViva::EntidadViva(int startX, int startY, int startEnergy) {
    pos.x = startX;
    pos.y = startY;
    energia = startEnergy;
    edad = 0; // Las entidades nacen con edad de 0
    Vitalidad = true; // Toda entidad nace viva
}

Plant::Plant(int startX, int startY, int startEnergy) : EntidadViva(startX,startY,startEnergy){}
Hervivoro::Hervivoro(int startX, int startY, int startEnergy) : EntidadViva(startX, startY, startEnergy){}
Carnivoro::Carnivoro(int startX, int startY, int startEnergy) : EntidadViva(startX,startY,startEnergy){}

Ecosystemboard::Ecosystemboard(int w, int h) : width(w), height(h), currentCycle(0) {       //Constructor de la grilla

    grid.resize(height, vector<EntidadViva*>(width, nullptr)); //Inicializo la matriz llena de nullptr(vacio)
}

Ecosystemboard::~Ecosystemboard() {
    //Limpiamos entidaddes que ya estaban
    for (EntidadViva* entity : entities) {
        delete entity;
    }

    for(EntidadViva* entity : newBorns) {
        delete entity;      //Limpiamos a los que nacen en el ciclo
    }

    entities.clear();
    newBorns.clear();


}

//Logica de act, osea el movimiento

void Plant::act(Ecosystemboard* board) {
    //En cada ciclo la planta se hace mas vieja
    edad++;
    // Al ser una planta puede generar energia de manera estatica
    energia += 5;

    //Muerte por vejez planta
    if (edad >= 80) {
        Vitalidad = false;
        return;
    }

    // Reproduccion de la planta en base a energia acumulada
    if (energia > 20) {
        int dx[] = {1,-1,0,0,1,1,-1,-1};
        int dy[] = {0,0,1,-1,1,-1,-1,1};


        for (int i = 0; i < 8; i++){
            int checkX = pos.x + dx[i];
            int checkY = pos.y + dy[i];

            if (board->isCellEmpty(checkX,checkY)) {
                board->addEntity(new Plant(checkX,checkY, 10)); //Esta nueva planta nace con 10 de energia
                energia -= 20; //A la planta le cuesta energia generar otra
                break;

            }

        }
        //Si encuentra un lugar vacio al lado, se crea una nueva planta

    }
}

void Hervivoro::act(Ecosystemboard* board) {
    if (!Vitalidad) return; //si esta muerto no hace nada

    edad++;
    energia -= 2; //el costo de moverse de casillas

    if (energia <= 0){
        Vitalidad = false;
        return; //Si la energia es menor a 0, muere el hervivoro
    }

    // Si esta muy viejo, muere
    if (edad > 50) {
        Vitalidad = false;
        return;
    }

    //Vectores con los movimientos posibles
    int dx[] = {1,-1,0,0,1,1,-1,-1};
    int dy[] = {0,0,1,-1,1,-1,-1,1};

    bool comio = false; //Bandera para saber si ya comio en ese turno

    //Busqueda de comida
    for (int i = 0; i < 8; i++){
        int checkX = pos.x + dx[i]; //chequeo de x
        int checkY = pos.y + dy[i]; //chequeo de y

        if (board->isPlant(checkX,checkY)) {
            //Encontro una planta
            board->consumePlant(checkX,checkY);
            energia += 15;
            comio = true;
            break;
        }

    }


    // Movimiento si es que no comio
    if (!comio) {
        // Elegimos un número al azar del 0 al 7 para empezar a mirar en una dirección aleatoria
        int direccionInicial = rand() % 8;

        for (int i = 0; i < 8; i++) {
            // Usamos el operador módulo (%) para dar la vuelta al arreglo sin salirnos
            int dir = (direccionInicial + i) % 8;

            int checkX = pos.x + dx[dir];
            int checkY = pos.y + dy[dir];

            if (board->isCellEmpty(checkX, checkY)) {
                board->moveEntity(this, checkX, checkY);
                pos.x = checkX;
                pos.y = checkY;
                break;
            }
        }
    }

}

void Carnivoro::act(Ecosystemboard* board) {
    if (!Vitalidad) return; //Si esta muerto no hace nada

    edad ++; //cada ciclo aumenta la edad
    energia -= 2; //moverse cuesta energia

    if (energia <= 0) {
        Vitalidad = false;
        return;             //Si la energia es menor a 0 muere el carnivoro
    }

    if (edad > 60){
        Vitalidad = false;
        return;
    }

    //Vectores de direcciones
    int dx[] = {1,-1,0,0,1,1,-1,-1};
    int dy[] = {0,0,1,-1,1,-1,-1,1};

    bool comio = false; //bandera para ver si ya comio

    //Busqueda de comida
    for(int i = 0; i < 8; i++){
        int checkX = pos.x + dx[i]; //chequeo en x
        int checkY = pos.y + dy[i]; //chequeo en y

        if (board->isHervivoro(checkX,checkY)) {
            //encontro un hervivoro
            board->consumeHervivoro(checkX,checkY);
            energia += 15;
            comio = true;
            break;
        }
    }

    // Movimiento si es que no comio
    if (!comio) {
        // Elegimos un número al azar del 0 al 7 para empezar a mirar en una dirección aleatoria
        int direccionInicial = rand() % 8;

        for (int i = 0; i < 8; i++) {
            // Usamos el operador módulo (%) para dar la vuelta al arreglo sin salirnos
            int dir = (direccionInicial + i) % 8;

            int checkX = pos.x + dx[dir];
            int checkY = pos.y + dy[dir];

            if (board->isCellEmpty(checkX, checkY)) {
                board->moveEntity(this, checkX, checkY);
                pos.x = checkX;
                pos.y = checkY;
                break;
            }
        }
    }
}


void Ecosystemboard::addEntity(EntidadViva* entity) {      //funcion para agregar entidades, que son punteros a la clase base de entidad viva
    Posicion p = entity->getPos(); //preguntamos las coordenadas de la entidad

    if(isCellEmpty(p.x,p.y)) {      //Si la celda esta vacia
        grid[p.y][p.x] = entity;
        newBorns.push_back((entity)); //Guardamos en newBorns
    } else {
        delete entity; //Si no hay lugar, liberamos memoria
    }
}

void Ecosystemboard::nextCycle() {
    currentCycle++; //Avanza el tiempo del ecosistema

    for (EntidadViva* entity : entities) {      //turno a cada entidad viva
        if (entity->getVitalidad()) {  //Verificamos que la entidad esta viva antes de darle el turno, porque pudo haber muerto
            entity->act(this);
        }
    }

    for (EntidadViva* b : newBorns){
        entities.push_back(b);
    }
    newBorns.clear(); //limpiamos newBorns

    cleanupDeadEntities(); //limpiamos los muertos
}

void Ecosystemboard::cleanupDeadEntities()  {    // Limpiamos a los muertos
    auto it = entities.begin();  //usamos un iterador para recorrer la lista de entidades
    while (it != entities.end()) {
        EntidadViva* entity = *it;  //Obtenemos el puntero a la entidad actual
        if (!entity->getVitalidad()){       //si encontramos un muerto
            Posicion p = entity->getPos();
            if(grid[p.y][p.x] == entity) {
                grid[p.y][p.x] = nullptr;        //vaciamos la casilla del grid
            }

            delete entity;
            it = entities.erase(it); //La funcion erase nos devuelve automaticamente el nuevo cursor valido
        } else {
            ++it; //Si esta vivo avanzamos el cursor al siguiente
        }
    }
}

bool Ecosystemboard::isCellEmpty(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return false; //revisamos que no nos salimos del mapa
    return grid[y][x] == nullptr; //Devolvemos true si hay un nullptt
}

bool Ecosystemboard::isPlant(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return false; //revisamos que no nos salimos del mapa
    if (grid[y][x] != nullptr) {
        Plant* posiblePlanta = dynamic_cast<Plant*>(grid[y][x]); //intento de convertir el puntero EntidadViva a puntero de planta
        if (posiblePlanta != nullptr) {
            return true;
        }
    }
    return false;
}

void Ecosystemboard::consumePlant(int x, int y){
    if (grid[y][x] != nullptr) {
        grid[y][x]->setMuerte();          //Hacemos que la planta muera
    }
}

bool Ecosystemboard::isHervivoro(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return false; //revisamos que no nos salimos del mapa
    if (grid[y][x] != nullptr) {
        Hervivoro* posibleHervivoro = dynamic_cast<Hervivoro*>(grid[y][x]);
        if (posibleHervivoro != nullptr){
            return true;
        }
    }
    return false;
}

void Ecosystemboard::consumeHervivoro(int x, int y){
    if (grid[y][x] != nullptr) {
        grid[y][x]->setMuerte();          //Hacemos que el hervivoro muera
    }
}

void Ecosystemboard::moveEntity(EntidadViva* entity, int newX, int newY) {
    Posicion p = entity->getPos();

    //Vaciamos la casilla vieja
    grid[p.y][p.x] = nullptr;

    //Ponemos el puntero en la nueva casilla
    grid[newY][newX] = entity;

    //Actualizamos las coordenadas
    entity->setPos(newX,newY);
}