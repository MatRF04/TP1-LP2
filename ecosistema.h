#ifndef ECOSISTEMA_H
#define ECOSISTEMA_H
using namespace std;


#include <vector>

struct Posicion         //Posicion del elemento en la grilla
{
    int x;
    int y;
};

//Aviso de que existira mas abajo
class Ecosystemboard;

class EntidadViva {
protected:  //Protected para que las hijas puedan ver estas funciones
    Posicion pos;
    int energia;
    int edad;
    bool Vitalidad;

public:
    EntidadViva(int startX, int startY, int startEnergia);
    virtual ~EntidadViva() {}  //virtual porque usamos herencia

    virtual void act(Ecosystemboard* board) = 0; //Virtual pura para que cada hijo elija su version

    void setMuerte() { Vitalidad = false; }
    bool getVitalidad() const { return Vitalidad; }
    Posicion getPos() const { return pos; }
    void setPos(int newX, int newY) { pos.x = newX; pos.y = newY; }
};

class Plant : public EntidadViva {
public:
    Plant(int startX, int startY, int startEnergia);
    void act(Ecosystemboard* board) override;
};

class Hervivoro : public EntidadViva {   //clase hija hervivoro
public:
    Hervivoro(int startX, int startY, int startEnergia);
    void act(Ecosystemboard* board) override;
};

class Carnivoro : public EntidadViva {   //clase hija carnivoro
public:
    Carnivoro(int startX, int startY, int startEnergia);
    void act(Ecosystemboard* board) override;
};

class Ecosystemboard {
private:
    int width;
    int height;
    int currentCycle; //Esto para llevar la cuenta de turnos

    vector<vector<EntidadViva*>> grid; //vector de vectores, una matriz llamada grid(grilla)
    vector<EntidadViva*> entities; //una lista con las entidades vivvas, para no tener que recorrer toda la matriz vacia cuando queramos que actue

    vector<EntidadViva*> newBorns; //vector de recien nacidos


public:
    Ecosystemboard(int w, int h);
    ~Ecosystemboard();

    void addEntity(EntidadViva* entity);
    void nextCycle();
    void cleanupDeadEntities();

    bool isCellEmpty(int x, int y);
    bool isPlant(int x, int y);
    void consumePlant(int x, int y);
    bool isHervivoro(int x, int y);
    void consumeHervivoro(int x, int y);
    void moveEntity(EntidadViva* entity, int newX, int newY);

    //Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getCycle() const { return currentCycle; }
    const vector<vector<EntidadViva*>>& getGrid() const { return grid; }

};

#endif // ECOSISTEMA_H
