#ifndef SAVEGAME_H_INCLUDED
#define SAVEGAME_H_INCLUDED

class gameData{
    private:
        Map mapa;
    public:
        gameData(Map*m);
        int guardarEnDisco();
        int leerDelDisco();
};

gameData::gameData(Map*m):mapa(m){
    std::cout<<"listo"<<std::endl;
}

int gameData::guardarEnDisco(){
    FILE*p = fopen("savedata.dat", "wb");
    if(!p)
        return 0;

    int g = fwrite(this, sizeof*this, 1, p);
    return g;
}

int gameData::leerDelDisco(){
}

#endif // SAVEGAME_H_INCLUDED
