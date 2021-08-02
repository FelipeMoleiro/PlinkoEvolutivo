#include <vector>
#include <box2d/box2d.h>
#include "object.hpp"
#include "Shader.hpp"
#include <utility>

//Objeto que cria uma simulação do jogo

class gameSimulation
{
private:
    //posições gerais
    std::vector<std::pair<float,float>> posBolas;
    std::vector<std::pair<float,float>> posPlataformas;
    std::vector<float> anglePlataformas;
    std::vector<float> scores;
    std::vector<float> posBarreiras;
    //objetos openGL para mostrar na tela se preciso
    Shader* programShader;
    std::vector<circle> bolas;
    std::vector<quadrado> plataformas;
    std::vector<quadrado> barreiras;
public:
    gameSimulation(std::vector<std::pair<float,float>> &posBolas,std::vector<std::pair<float,float>> &posPlataformas,
                            std::vector<float> &anglePlataformas,std::vector<float> &scores,Shader* program);
    float simulate(float *realTime);
    void drawFrame();
    void newScore(std::vector<float> &scores);
    void inserirBola(float posx,float posy);
    void inserirPlataforma(float posx,float posy,float angle);
    void resetBolasGL();
    void set_angles(std::vector<float> &anglePlataformas);
    ~gameSimulation();
};
