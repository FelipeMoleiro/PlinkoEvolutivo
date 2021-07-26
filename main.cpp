#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 

#include <box2d/box2d.h>
#include <stdio.h>
#include "Shader.hpp"
#include "object.hpp"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "time.h"
#include <iostream>

#include <glm/gtx/string_cast.hpp>
#include <utility>
#include <vector>
#include <stdlib.h>

#include "globals.hpp"
#include "gameSimulation.hpp"

const int numPlataformas = 4;

float mutacaoMax = 10; //em graus
float porcentagemMutacao = 0.1;

#define NUM_INDIVIDUOS 10
std::vector<float> anglePlataformas[NUM_INDIVIDUOS];
float scoresIndividuos[NUM_INDIVIDUOS];

int geracao = 0;

void initpop(){
    //setting inicial values
    for(int i=0;i<NUM_INDIVIDUOS;i++){
        for(unsigned int j=0;j<numPlataformas;j++){
            anglePlataformas[i].push_back(rand()*360);
        }
    }
}

void avalia(gameSimulation* simu){
    for(int i=0;i<NUM_INDIVIDUOS;i++){
        simu->set_angles(anglePlataformas[i]);
        scoresIndividuos[i] = simu->simulate(false);
    }
}

std::vector<float> mistura_gene(std::vector<float> &gene1,std::vector<float> &gene2){
    std::vector<float> geneFinal;
    for(unsigned int i=0;i<gene1.size();i++){
        geneFinal.push_back((gene1[i] + gene2[i])/2.0);
    }
    return geneFinal;
}

// Melhor compartilha gene com todos
void elitismo() {
    float maxScore = scoresIndividuos[0];
    int maxi = 0;

    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }

    //std::cout<< maxScore << std::endl;

    for (int i=0;i<NUM_INDIVIDUOS;i++){
        if (i==maxi)        // Protege o melhor individuo
            continue;

        // Crossover
        anglePlataformas[i] = mistura_gene(anglePlataformas[maxi],anglePlataformas[i]);
        // Mutacao
        for(unsigned int j=0;j<anglePlataformas[i].size();j++){
            if( (((double)rand())/RAND_MAX) < porcentagemMutacao){// porcentagemMutacao % das vezes muta
                float taxa = ((((double)rand())/RAND_MAX) - 0.5)*2; //intervalo de [-1,1]                    
                anglePlataformas[i][j] = fmod(anglePlataformas[i][j] + taxa*mutacaoMax,360.0);
            }

        }
    }
}

void show_melhor(gameSimulation* simu){
    float maxScore = scoresIndividuos[1];
    int maxi = 0;

    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }

    std::cout << "Geração " << geracao << ": " << maxScore << std::endl;;
    simu->set_angles(anglePlataformas[maxi]);
    simu->simulate(true);
}

int main(){
    srand(462);
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, false);

    GLFWwindow* window = glfwCreateWindow(width, height, "Cenario", NULL, NULL);

    glfwMakeContextCurrent(window);
    
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s\n", glewGetErrorString(GlewInitResult));


    Shader programShader = Shader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");

    
    float mat[16] = {
        2/(right-left),0,0,-(right+left)/(right-left),
        0,2/(top-bottom),0,-(top+bottom)/(top-bottom),
        0,0,1,0,
        0,0,0,1
    };
    programShader.setMat("projection",mat);

    printf("oi\n");
// Setando parametros da simulação
    std::vector<std::pair<float,float>> posBolas;

    posBolas.push_back(std::make_pair(-3,16));
    posBolas.push_back(std::make_pair(-2,16));
    posBolas.push_back(std::make_pair(1,16));
    posBolas.push_back(std::make_pair(2,16));

    std::vector<std::pair<float,float>> posPlataformas;
    std::vector<float> anglePlataformasSet;

    posPlataformas.push_back(std::make_pair(-3,15));
    anglePlataformasSet.push_back(0);

    posPlataformas.push_back(std::make_pair(-2,12));
    anglePlataformasSet.push_back(0);

    posPlataformas.push_back(std::make_pair(1,5));
    anglePlataformasSet.push_back(0);

    posPlataformas.push_back(std::make_pair(2,7));
    anglePlataformasSet.push_back(0);

    std::vector<float> scores;
    scores.push_back(23);
    scores.push_back(1);
    scores.push_back(100);
    scores.push_back(1);
    scores.push_back(1);
    scores.push_back(1);

    gameSimulation simu(posBolas,posPlataformas,anglePlataformasSet,scores,&programShader);

    glfwShowWindow(window);

    initpop();
    while(!glfwWindowShouldClose(window)){
        //printf("oi\n");
        avalia(&simu);
        show_melhor(&simu);
        elitismo();
        geracao++;
    }


    
    

    
    /*
    // Exibindo nossa janela
    glfwShowWindow(window);


    while (!glfwWindowShouldClose(window))
    {   
        //clock_t begin = clock();

        //while((double)(clock() - begin) / CLOCKS_PER_SEC < 1.0/60.0){
        //    glfwPollEvents();
       // }
        
        glClearColor(1.0,1.0,1.0,1.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);







        glfwSwapBuffers(window);
    }
*/

    return 0;
}