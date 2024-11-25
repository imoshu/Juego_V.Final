#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED
const char file[] = "scores.sk";

class Score{
    public:
        int seed;
        int tamMap;
        int puntaje;
        bool leerDelDisco(int);
        bool guardarEnDisco();
        void setScore(int, int, int);
};

bool Score::leerDelDisco(int pos){
    FILE*p = fopen(file, "rb");
    if(!p) return false;
    fseek(p, pos*sizeof*this, 0);
    fread(this, sizeof *this, 1, p);
    fclose(p);
    return true;
}
bool Score::guardarEnDisco(){
    FILE*p = fopen(file, "ab");
    if(!p) return false;
    fwrite(this, sizeof *this, 1, p);
    fclose(p);
    return true;
}

void Score::setScore(int _seed, int tam, int punt){
    seed = _seed;
    tamMap = tam;
    puntaje = punt;
}
#endif // SCORE_H_INCLUDED
