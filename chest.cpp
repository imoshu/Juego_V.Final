#include "chest.h"

Chest::Chest(int f, int c){
    estado = 1;
    setChest();
    pos.y = f;
    pos.x = c;
}

Chest::Chest(int f, int c, int t){
    estado = 1;
    setChest(t);
    pos.y = f;
    pos.x = c;
}

int Chest::chestDrop(){
    return drop;
}

void Chest::disable(){
    estado = false;
    tipo = tipo*-1;
}

int Chest::getTipo(){
    return tipo;
}

void Chest::setChest(int t){
    int p;
    if(t==0){
        p = rand()%10;
        if(p<=3){
            tipo = 4;
        }else if(p<=6){
            tipo = 5;
        }else if(p<=8){
            tipo = 6;
        }else{
            tipo = 7;
        }
    }else
        tipo = t;


    p = rand()%10;
    switch(tipo){
        case 4:
            drop = rand()%30 + 15;
            break;
        case 5:
            if(p>6)
                drop = 1;
            else
                drop = rand()%35 + 40;
            break;
        case 6:
            if(p>=5)
                drop = 1;
            else
                drop = rand()%35 + 40;
            break;
        case 7:
            drop = 1;
            break;
    }
}
