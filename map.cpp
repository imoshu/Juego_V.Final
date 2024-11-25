#include "map.h"
#include <iostream>
//#include <unistd.h> sleep()
#define exitZone 12
#define maxDoors 4
#define tileSize 64

Map::Map(int _tam, int typeMap, int data){
    tam = _tam;
    m = new Tile*[tam];
    for(int h=0;h<tam;h++){
        m[h] = new Tile[tam];
    }
    mapaEnCero();
    switch(typeMap){
        case 0:
            newMap(data);
            setWallsToDraw();
            setCollisions();
            spawnDoors();
            break;
        case 1:
            newDungeon(data);
            setWallsToDraw();
            setCollisions();
            break;
    }
    loadTextures();
}

Map::Map(FILE*p){
    if(!cargarDelDisco(p)) exit(1);
    loadTextures();
}

void Map::loadTextures(){
    f.loadFromFile("sprites/tTile.png");
    w.loadFromFile("sprites/wTile.png");
    c.loadFromFile("sprites/chest.png");
    dw.loadFromFile("sprites/gates.png");
    int i;
    for(i=0;i<2;i++){
        doorway[i].setTexture(dw);
        doorway[i].setTextureRect(sf::IntRect(64*i, 0, 64, 64));
    }
    for(i=0;i<3;i++){
        floor[i].setTexture(f);
        floor[i].setTextureRect(sf::IntRect(64*i, 0, 64, 64));
        floor[i].setColor(sf::Color(107, 142, 35)); //128, 128, 0
    }
    for(i=0;i<4;i++){
        swall[i].setTexture(w);
        swall[i].setTextureRect(sf::IntRect(64*i, 0, 64, 64)); //otros
        swall[i].setColor(sf::Color(128, 128, 0));
    }
}

Map::~Map(){
    delete m;
    delete wall;
}

int Map::cellsCount(int f, int c, int type){
    int i, t, r = 0;
    for(i=-1;i<2;i++){
        for(t=-1;t<2;t++){
            if(i == 0 && t == 0)
                continue;
            if(m[f+i][c+t].tipo == type)
                r++;
        }
    }
    return r;
}

void Map::mapaEnCero(){
    int i, f;
    sf::Vector2u pos;
    for(i=0;i<tam;i++){
        for(f=0;f<tam;f++){
            m[i][f].tipo = 0;
            m[i][f].id = 0;
            m[i][f].objeto = 0;
            m[i][f].visitado = 0;
            pos.y = i;
            pos.x = f;
            m[i][f].position = pos;
        }
    }
}

int Map::countWalls(){
    int i, h, c=0;
    for(i=0;i<tam;i++){
        for(h=0;h<tam;h++){
            if(isWall(i, h)){
                m[i][h].id = c;
                c++;
            }
        }
    }
    return c;
}

void Map::setCollisions(){
    cWalls = countWalls();
    wall = new sf::FloatRect[cWalls];
    int i, h, c=0;
    for(i=0;i<tam;i++){
        for(h=0;h<tam;h++){
            if(m[i][h].tipo==3){
                wall[c].height = 28;
                wall[c].width = 64;
                wall[c].left = h*64;
                wall[c].top = i*64;
                c++;
            }else if(m[i][h].tipo >= 4 && m[i][h].tipo <= 7){
                wall[c].height = 32;
                wall[c].width = 64;
                wall[c].left = h*64;
                wall[c].top = i*64+32;
                c++;
            }else if(isWall(i, h)){
                wall[c].height = 64;
                wall[c].width = 64;
                wall[c].left = h*64;
                wall[c].top = i*64;
                c++;
            }
        }
    }
}

void Map::decide(Room *room){
    int p = rand()%10;
    if(p>=6){
        room->alto = rand()%8+3;
        room->ancho = rand()%8+3;
    }else{
        if(rand()%2){
            room->alto = rand()%3+1;
            room->ancho = rand()%8+4;
        }else{
            room->alto = rand()%8+4;
            room->ancho = rand()%3+1;
        }
    }
}

int Map::checkSpace(Room room){
    int i, f, casOcup = 0;
    int casTotal = room.ancho*room.alto;
    for(i=room.posX;i<room.posX+room.ancho;i++){
        for(f=room.posY;f<room.posY+room.alto;f++){
            if(m[f][i].tipo == casAct){
                casOcup++;
            }
        }
    }
    return casOcup*100/casTotal;
}

void Map::makeRoom(Room *room){
    decide(room);
    room->posX = rand()%(tam-room->ancho+1);
    room->posY = rand()%(tam-room->alto+1);
}

void Map::buildRoom(Room room, int& tileCount){
    int i, f;
    for(i=room.posX;i<room.posX+room.ancho;i++){
        for(f=room.posY;f<room.posY+room.alto;f++){
            if(m[f][i].tipo == 0){
                m[f][i].tipo = casAct;
                tileCount++;
            }
        }
    }
}

void Map::buildWalls(){
    int f, c;
    for(f=0;f<tam;f++){
        for(c=0;c<tam;c++){
            if(f == 0 || c == 0 || c == tam-1 || f == tam-1){
                    m[f][c].tipo = casWall;
            }
            if(f!=0 && c!=0 && f!=tam-1 && c!=tam-1){
                if(m[f][c].tipo==0 && (m[f][c+1].tipo==casAct || m[f+1][c].tipo==casAct || m[f-1][c].tipo==casAct || m[f][c-1].tipo==casAct || m[f+1][c-1].tipo==casAct || m[f+1][c+1].tipo==casAct))
                    m[f][c].tipo = casWall;
            }
        }
    }
}

bool Map::checkVecinos(int f, int c){
    //si algunos delos vecinos en piso
    int i, k;
    for(i=-1;i<2;i++){
        for(k=-1;k<2;k++){
            if((i == -1 && k == 1) || (i == -1 && k == -1) || (i == 0 && k == 0) || (f+i < 0 || c+k < 0 || f+i > tam-1 || c+k > tam-1)){
                continue;
            }
            if(m[f+i][c+k].tipo == casAct){
                return true;
            }
        }
    }
    return false;
}

void Map::simpleConnect(int nPasadas){
int i, f, p;
    for(p=0;p<nPasadas;p++){
        for(i=1;i<tam-2;i++){
            for(f=1;f<tam-3;f++){
                if(m[i][f].tipo==casAct && (m[i][f+1].tipo!=casAct || m[i][f+2].tipo!=casAct) && m[i][f+3].tipo==casAct){
                    if(1){
                        m[i][f+1].tipo=casAct;
                        m[i][f+2].tipo=casAct;
                    }
                }
            }
        }
        for(i=1;i<tam-3;i++){
            for(f=1;f<tam-2;f++){
                if(m[i][f].tipo==casAct && (m[i+1][f].tipo!=casAct || m[i+2][f].tipo!=casAct) && m[i+3][f].tipo==casAct){
                    if(1){
                        m[i+1][f].tipo=casAct;
                        m[i+2][f].tipo=casAct;
                    }
                }
            }
        }
    }
}

void Map::newMap(int ocupacion){
    int actualOcupado = 0;
    ocupacion = ocupacion*(tam*tam)/100;
    Room actRoom;
    while(actualOcupado < ocupacion){
        do{
            makeRoom(&actRoom);
        }while(checkSpace(actRoom) > 10);
        buildRoom(actRoom, actualOcupado);
    }
    std::cout<<actualOcupado<<" : "<<ocupacion<<std::endl;
    exitSpawn();
    simpleConnect(2);
    buildWalls();
}

void Map::newDungeon(int nPasadas){
    int i, p;
    int center = tam/2;
    for(i=0;i<tam;i++){
        for(p=0;p<tam;p++){
            if(i == 0 || p == 0 || i == tam-1 || p == tam-1){
                m[i][p].tipo = 0;
                continue;
            }
            sf::Vector2u current(abs(center-i), abs(center-p));
            float prob = 100 - ((current.x+current.y)*100/(tam-2));
            if(rand()%100 < prob){
                m[i][p].tipo = 1;
            }
        }
    }
    for(int f=0;f<nPasadas;f++){
        for(i=1;i<tam-1;i++){
            for(p=1;p<tam-1;p++){
                if(m[i][p].tipo == 0){
                    int cells = cellsCount(i, p, 1);
                    if(cells > 4)
                        m[i][p].tipo = 1;
                }else{
                    int cells = cellsCount(i, p, 0);
                    if(cells > 4)
                        m[i][p].tipo = 0;
                }
            }
        }
    }
    buildWalls();
}

///////////////////////////////////////////////////////////// set get

sf::FloatRect Map::getWall(int c){
    return wall[c];
}

int Map::getTam(){
    return tam;
}

int Map::getWalls(){
    return cWalls;
}

void Map::setInMap(int f, int c, int dato){
    m[f][c].tipo = dato;
}

void Map::setObjectInMap(int f, int c, int dato){
    m[f][c].objeto = dato;
}

int Map::getObjectInMap(int f, int c){
    return m[f][c].objeto;
}

int Map::getIdInMap(int f, int c){
    return m[f][c].id;
}

int Map::setIdInMap(int f, int c, int dato){
    m[f][c].id = dato;
}

int Map::getPosInMap(int f, int c){
    return m[f][c].tipo;
}

void Map::setVisitadoMap(int f, int c, bool p){
    m[f][c].visitado = p;
}

bool Map::getVisitadoMap(int f, int c){
    return m[f][c].visitado;
}

Tile* Map::getTile(sf::Vector2u position){
    return &m[position.y][position.x];
}
//////////////////////////////////////////////////////////////
void Map::drawChests(int i, int h){
    chest.setTexture(c);
    chest.setPosition((h*64)+16, (i*64)+16);
    chest.setColor(sf::Color::Transparent);
    switch(m[i][h].objeto){
        case 4:
            chest.setTextureRect(sf::IntRect(0, 0, 32, 32));
            chest.setColor(sf::Color(128, 128, 0));
            break;
        case -4:
            chest.setTextureRect(sf::IntRect(32, 0, 32, 32));
            chest.setColor(sf::Color(128, 128, 0));
            break;
        case 5:
            chest.setTextureRect(sf::IntRect(0, 32, 32, 32));
            chest.setColor(sf::Color(128, 128, 0));
            break;
        case -5:
            chest.setTextureRect(sf::IntRect(32, 32, 32, 32));
            chest.setColor(sf::Color(128, 128, 0));
            break;
        case 6:
            chest.setTextureRect(sf::IntRect(0, 64, 32, 32));
            chest.setColor(sf::Color(128, 128, 128));
            break;
        case -6:
            chest.setTextureRect(sf::IntRect(32, 64, 32, 32));
            chest.setColor(sf::Color(128, 128, 128));
            break;
        case 7:
            chest.setTextureRect(sf::IntRect(0, 96, 32, 32));
            chest.setColor(sf::Color::White);
            break;
        case -7:
            chest.setTextureRect(sf::IntRect(32, 96, 32, 32));
            chest.setColor(sf::Color::White);
            break;
    }
}

void Map::drawObject(int i, int h, sf::RenderWindow* window){
    switch(m[i][h].tipo){
        case casAct:
            {
            int nF = m[i][h].id;
            this->floor[nF].setPosition(64*h, 64*i);
            window->draw(this->floor[nF]);
            break;
            }
        case casWall:
            this->swall[1].setPosition(64*h, 64*i);
            window->draw(this->swall[1]);
            break;
        case 3:
            this->swall[0].setPosition(64*h, 64*i);
            window->draw(this->swall[0]);
            break;
        case 4:
            swall[1].setPosition(64*h+64, 64*i+64);
            swall[1].setRotation(180);
            window->draw(this->swall[1]);
            break;
        case 5:
            swall[1].setPosition(64*h+64, 64*i);
            swall[1].setRotation(90);
            window->draw(this->swall[1]);
            break;
        case 6:
            this->swall[2].setPosition(64*h, 64*i);
            window->draw(this->swall[2]);
            break;
        case 7:
            swall[2].setPosition(64*h+64, 64*i);
            swall[2].setRotation(90);
            window->draw(this->swall[2]);
            break;
        case 8:
            swall[3].setPosition(64*h, 64*i);
            window->draw(this->swall[3]);
            break;
        case 9:
            this->doorway[0].setPosition(64*h, 64*i);
            window->draw(doorway[0]);
            break;
    }
    swall[1].setRotation(0);
    swall[2].setRotation(0);
    if((this->m[i][h].objeto>3 && this->m[i][h].objeto<8) || (this->m[i][h].objeto>-8 && this->m[i][h].objeto<-3)){
        drawChests(i, h);
        window->draw(chest);
    }else if(m[i][h].objeto == 8){
        doorway[1].setPosition(64*h, 64*i);
        window->draw(doorway[1]);
    }
}

int Map::setWall(int f, int c){

    bool left = false, up = false, right = false, down = false;
    if(f > 0){
        if(m[f-1][c].tipo == casAct || m[f-1][c].tipo == 3){
            up = true;
        }
    }
    if(f < tam-1){
        if(m[f+1][c].tipo == casAct || m[f+1][c].tipo == 3){
            down = true;
        }
    }
    if(c > 0){
        if(m[f][c-1].tipo == casAct || m[f][c-1].tipo == 3){
            left = true;
        }
    }
    if(c < tam-1){
        if(m[f][c+1].tipo == casAct || m[f][c+1].tipo == 3){
            right = true;
        }
    }

    if(left){
        if(up){
            if(right){
                if(down){
                    return 8;
                }else{
                    return 8;
                }
            }else{
                if(down){
                    return 8;
                }else{
                    return 6;
                }
            }
        }else{
            if(right){
                if(down){
                    return 3;
                }else{
                    return 8;
                }
            }else{
                if(down){
                    return 3;
                }else{
                    return 2;
                }
            }
        }
    }else{
        if(up){
            if(right){
                if(down){
                    return 8;
                }else{
                    return 7;
                }
            }else{
                if(down){
                    return 8;
                }else{
                    return 5;
                }
            }
        }else{
            if(right){
                if(down){
                    return 3;
                }else{
                    return 4;
                }
            }else{
                if(down){
                    return 3;
                }else{
                    if(m[f][c-1].tipo == 3)
                        return 2;
                    else return 4;
                }
            }
        }
    }

    return -1;
}

void Map::drawSecondLayer(sf::RenderWindow* window){
    for(unsigned i=0;i<secondLayer.size();i++){
        int c = secondLayer[i].x;
        int f = secondLayer[i].y;
        drawObject(c, f, window);
    }
    secondLayer.clear();
}

void Map::dibujarMapa(bool opc, sf::RenderWindow *window, sf::Vector2u j){
    if(opc){
        const int view = 10;
        int f = j.y;
        int c = j.x;
        if(c-view<0) c = view;
        if(c+view > tam-1) c = (tam-1)-view;
        if(f-view < 0) f = view;
        if(f+view > tam-1) f = (tam-1)-view;
        int i, h;
        for(i=f-view;i<=f+view;i++){
            for(h=c-view;h<=c+view;h++){
                if(m[i][h].tipo >= 4 && m[i][h].tipo <= 7){
                    secondLayer.push_back(sf::Vector2u(i, h));
                }
                else{
                    drawObject(i, h, window);
                }
            }
        }
    }else{
        int i, h;
        for(i=0;i<this->tam;i++){
            for(h=0;h<this->tam;h++){
                drawObject(i, h, window);
            }
        }
    }
}

bool Map::isWall(int f, int c){
    if(m[f][c].tipo >= 2 && m[f][c].tipo <= 8)
        return true;
    else return false;
}

void Map::setWallsToDraw(){
    int f, c;
    for(f=0;f<tam;f++){
        for(c=0;c<tam;c++){
            if(m[f][c].tipo != 0 && !checkVecinos(f, c)){
                m[f][c].tipo = 0;
                continue;
            }
            if(isWall(f, c)){
                int toDraw = setWall(f, c);
                m[f][c].tipo = toDraw;
            }
            if(m[f][c].tipo==casAct){
                m[f][c].id = rand()%3;
            }
        }
    }
}

void Map::mapConsole(){
    int f, c;
    for(f=0;f<tam;f++){
        std::cout<<std::endl;
        for(c=0;c<tam;c++){
            //if(m[f][c].tipo == 0) std::cout<<"-"<<" ";
            //else std::cout<<m[f][c].tipo<<" ";
            std::cout<<m[f][c].objeto<<" ";
        }
    }
}
void Map::spawnDoors(){
    int f, c, v;
    bool valido = false;
    do{
        if(rand()%2){
            f = rand()%(tam-1)+1;
            c = rand()%(tam-exitZone)+exitZone;
        }else{
            f = rand()%(tam-exitZone)+exitZone;
            c = rand()%(tam-1)+1;
        }
        for(v=-1;v<2;v++){
            if(m[f][c+v].tipo == 3){
                valido = true;
            }
            else{
                valido = false;
                break;
            }
        }

        if(valido){
            if(doors.size() == 0){
                doors.push_back(sf::Vector2u(f, c));
            }else{
                for(int d=0;d<doors.size();d++){
                    sf::Vector2u dist = Global::diffTo(doors[d], sf::Vector2u(f, c));
                    if(dist.x >(tam/6) || dist.y >(tam/6)){
                        for(int s=0;s<salida.size();s++){
                            sf::Vector2u distS = Global::diffTo(salida[s], sf::Vector2u(f, c));
                            if(distS.x > (tam/5) || distS.y > (tam/5)){
                                valido = true;
                            }else{
                                valido = false;
                                break;
                            }
                        }
                    }
                    else{
                        valido = false;
                        break;
                    }
                }
                if(valido)
                    doors.push_back(sf::Vector2u(f, c));
            }
        }
    }while(doors.size()<maxDoors);
    for(int i=0;i<doors.size();i++){
        printf("Salida x: %d y: %d \n", doors[i].y, doors[i].x);
        m[doors[i].x][doors[i].y].objeto = 8;
    }
}
void Map::exitSpawn(){
    int cantS, s1, s2;
    if(tam<=150){
        s1 = rand()%4;
        salida.push_back(exitDraw(s1));
        std::cout<<s1<<std::endl;
    }else{
        do{
            s1 = rand()%4;
            s2 = rand()%4;
        }while(s1 == s2);
        salida.push_back(exitDraw(s1));
        salida.push_back(exitDraw(s2));
        std::cout<<s1;
        std::cout<<s2;
    }
}

sf::Vector2u Map::exitDraw(int loc){
    int f, c;
    switch(loc){
        case 0:
            f=0;
            c=0;
            break;
        case 1:
            f=0;
            c=tam-exitZone;
            break;
        case 2:
            f=tam-exitZone;
            c=0;
            break;
        case 3:
            f=tam-exitZone;
            c=tam-exitZone;
            break;
    }
    sf::Vector2u exit(c+(exitZone/2), f+(exitZone/2));
    int i, j;
    for(i=f;i<exitZone+f;i++){
        for(j=c;j<exitZone+c;j++){
            m[i][j].tipo = casWall;
        }
    }
    drunkWalker(exit, 25, 1);
    drunkWalker(exit, sf::Vector2u((tam-1)/2, (tam-1)/2), 1);
    sf::Vector2u mapEnd;
    mapEnd.y = exit.y;
    mapEnd.x = exit.x;
    return mapEnd;
}

bool Map::guardarEnDisco(FILE*p){
    if(!p) return false;
    int i, j;
    fwrite(&tam, sizeof (int), 1, p);
    fwrite(&cWalls, sizeof (int), 1, p);
    for(i=0;i<salida.size();i++)
        fwrite(&salida[i], sizeof(sf::Vector2u), 1, p);
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
           fwrite(&m[i][j], sizeof(Tile), 1, p);
        }
    }
    for(i=0;i<cWalls;i++){
        fwrite(&wall[i], sizeof(sf::FloatRect), 1, p);
    }
    return true;
}

bool Map::cargarDelDisco(FILE*p){
    if(!p) return false;
    sf::Vector2u temp;
    fread(&tam, sizeof (int), 1, p);
    fread(&cWalls, sizeof (int), 1, p);
    if(tam<=150){
        fread(&temp, sizeof(sf::Vector2u), 1, p);
        salida.push_back(temp);
    }else{
        fread(&temp, sizeof(sf::Vector2u), 1, p);
        salida.push_back(temp);
        fread(&temp, sizeof(sf::Vector2u), 1, p);
        salida.push_back(temp);
    }

    m = new Tile*[tam];
    for(int h=0;h<tam;h++){
        m[h] = new Tile[tam];
    }
    wall = new sf::FloatRect[cWalls];
    int i, j;
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
           fread(&m[i][j], sizeof(Tile), 1, p);
           m[i][j].visitado = 0;
        }
    }
    for(i=0;i<cWalls;i++){
        fread(&wall[i], sizeof(sf::FloatRect), 1, p);
    }
    return true;
}
std::vector<sf::Vector2u> Map::getSalida(){
    return salida;
}

sf::Vector2u Map::getRandomNextStep(sf::Vector2u Pos){
    bool done = false;
    while(!done){
        int step = rand()%4;
        switch(step){
            case 0:
                if(Pos.x+1 < tam){
                    Pos.x++;
                    done = true;
                }
                break;
            case 1:
                if(Pos.x-1 >= 0){
                    Pos.x--;
                    done = true;
                }
                break;
            case 2:
                if(Pos.y+1 < tam){
                    Pos.y++;
                    done = true;
                }
                break;
            case 3:
                if(Pos.y-1 >= 0){
                    Pos.y--;
                    done = true;
                }
                break;
        }
    }
    return Pos;
}

void Map::drunkWalker(sf::Vector2u startPos, sf::Vector2u endPos, int tileType){
    m[startPos.y][startPos.x].tipo = tileType;
    while (startPos.x != endPos.x || startPos.y != endPos.y){
        int x = 0, y = 0;
        if (rand()%2)  //x
        {
            if (startPos.x == endPos.x)
            {
                x = 0;
            }else if (rand()%2)
            {
                if (startPos.x < endPos.x)
                {
                    x = 1;
                }
                else
                {
                    x = -1;
                }
            }
            else
            {
                if (startPos.x > endPos.x)
                {
                    x = -1;
                }
                else
                {
                    x = 1;
                }
            }

        }
        else                          //y
        {
            if (startPos.y == endPos.y)
            {
                y = 0;
            }else if(rand()%2)
            {
                if (startPos.y < endPos.y)
                {
                    y = 1;
                }
                else
                {
                    y = -1;
                }
            }
            else
            {
                if (startPos.y > endPos.y)
                {
                    y = -1;
                }
                else
                {
                    y = 1;
                }
            }
        }
        startPos.x += x;
        startPos.y += y;
        m[startPos.y][startPos.x].tipo = tileType;
    }
}
void Map::drunkWalker(sf::Vector2u sPos, int nMoves, int tileType){
    while(nMoves != 0){
        m[sPos.y][sPos.x].tipo = tileType;
        sPos = getRandomNextStep(sPos);
        nMoves--;
    }
}

Tile Map::getTileInGrid(int grid, sf::Vector2u pos){
    int gridSize = tileSize/grid;
    int c = pos.x/gridSize;
    int f = pos.y/gridSize;
    return m[f][c];
}
