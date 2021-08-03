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

//algumas variaveis globais do programa
int numPlataformas;

#define MutMaxBase 1
float mutacaoMax = MutMaxBase; //em graus
float porcentagemMutacao = 0.1;

#define NUM_INDIVIDUOS 10
std::vector<float> anglePlataformas[NUM_INDIVIDUOS];
float scoresIndividuos[NUM_INDIVIDUOS];

float bestPastInd[5];

int geracao = 0;

int popInit = 0; //se a população ja foi iniciada ou nao

//func que inicia os valores da população
void initpop(){
    //setting inicial values
    for(int i=0;i<NUM_INDIVIDUOS;i++){
        for(int j=0;j<numPlataformas;j++){
            anglePlataformas[i].push_back(((double)rand())/RAND_MAX * 360);
        }
    }

    for(int i=0;i<5;i++){
        bestPastInd[i] = 0;
    }
    popInit = 1;
}

float falseFloat = 0;

//avalia todos os individuos
void avalia(gameSimulation* simu){
    for(int i=0;i<NUM_INDIVIDUOS;i++){
        simu->set_angles(anglePlataformas[i]);
        scoresIndividuos[i] = simu->simulate(&falseFloat);
    }
}

//mistura dois genes pegando a media
std::vector<float> mistura_gene(std::vector<float> &gene1,std::vector<float> &gene2){
    std::vector<float> geneFinal;
    for(unsigned int i=0;i<gene1.size();i++){
        geneFinal.push_back((gene1[i] + gene2[i])/2.0);
    }
    return geneFinal;
}

//randomiza um gene
void randomize_gene(std::vector<float> &gene){
    for(unsigned int i=0;i<gene.size();i++){
        gene[i] = ((double)rand())/RAND_MAX * 360;
    }
}

// Melhor compartilha gene com todos
void elitismo() {
    //acha melhor
    float maxScore = scoresIndividuos[0];
    int maxi = 0;

    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }

    //compartilha gene e muta eles
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

//mata todos os individuos que não são o melhor
void genocidio(void)
{
    float maxScore = scoresIndividuos[0];
    int maxi = 0;

    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }

	for(int i=0;i<NUM_INDIVIDUOS; i++){
		if(i!= maxi){
			randomize_gene(anglePlataformas[i]);
		}
	}
}

//se não estamos tendo resultado com um valor de mutação por muito tempo, nos aumentamos a mutação
void alteraTaxaMutacao(){
    if(bestPastInd[4]-bestPastInd[0]<0.00001)
		mutacaoMax = mutacaoMax*1.2;
	else
		mutacaoMax = MutMaxBase;

    std::cout << "Mutação Max:" << mutacaoMax << std::endl;

	if(mutacaoMax>2*360){
        genocidio();
		mutacaoMax = MutMaxBase;
    }

}

//variaveis para controlar a leitura de input e janela
bool readingInput = true;
gameSimulation* simuPtr;
GLFWwindow* window;

float showScreen = SHOW_SCREEN;

float lastShown = 0;

FILE* outGrafico;

//função que desenha na tela a simulação do melhor indivuo de cada geração
void show_melhor(gameSimulation* simu){
    //acha melhor individuo
    float maxScore = scoresIndividuos[0];
    int maxi = 0;
    float meanScore = 0;
    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        meanScore += scoresIndividuos[i];
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }
    meanScore /= (float)NUM_INDIVIDUOS;

    // Guarda ultimos melhores individuos
	for (int i=1;i<5;i++)
	{
		bestPastInd[i-1] = bestPastInd[i];
	}
	bestPastInd[4] = maxScore;

    std::cout << "Geração " << geracao << ": " << maxScore << std::endl;
    fprintf(outGrafico,"%d,%f,%f\n",geracao,maxScore,meanScore);
    
    //std::cout << "lastShownVar: " << lastShown << std::endl;
    //std::cout << "showScreenVar: " << showScreen << std::endl;
    //se o score não é igual(aumentou ou abaixou) printa na tela
    if((!lastShown && showScreen) || bestPastInd[4]-bestPastInd[3]>0.00001 || bestPastInd[4] < bestPastInd[3]){
        lastShown = showScreen;
        simu->set_angles(anglePlataformas[maxi]);
        simu->simulate(&showScreen);
    }else{
        lastShown = showScreen;
        simu->set_angles(anglePlataformas[maxi]);
        glfwPollEvents();
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        simuPtr->resetBolasGL();
        simuPtr->drawFrame();

        glfwSwapBuffers(window);

    }
    
    

}

//variaveis das posições iniciais da simulação
std::vector<std::pair<float,float>> posBolas;
std::vector<std::pair<float,float>> posPlataformas;
std::vector<float> anglePlataformasSet;
std::vector<float> scores;

//faz a leitura das posições iniciais da posição em forma de texto
void readTextIn(){
    printf("Digite o numero de scores que teremos: \n");
    int nScores;
    scanf("%d", &nScores);
    printf("Digite os scores separados por espaço ou \\n agora: \n");
    for(int i=0;i<nScores;i++){
        float score;
        scanf("%f", &score);
        scores.push_back(score);
    }

    printf("Digite o numero de bolas que teremos: \n");
    int nBolas;
    scanf("%d", &nBolas);
    printf("Digite as posições x(-4 a 4) e y(0 a 16) de cada bola separados por espaço e cada bola separada por \\n agora:\n");
    for(int i=0;i<nBolas;i++){
        float posBolaX,posBolaY;
        scanf("%f %f", &posBolaX,&posBolaY);
        posBolas.push_back(std::make_pair(posBolaX,posBolaY));
    }

    printf("Digite o numero de plataformas que teremos: \n");
    int nPlataformas;
    scanf("%d", &nPlataformas);
    printf("Digite as posições x(-4 a 4) e y(0 a 16) de cada plataforma separados por espaço e cada plataforma separada por \\n agora:\n");
    for(int i=0;i<nPlataformas;i++){
        float posPlataformaX,posPlataformaY;
        scanf("%f %f", &posPlataformaX,&posPlataformaY);
        posPlataformas.push_back(std::make_pair(posPlataformaX,posPlataformaY));
        anglePlataformasSet.push_back(0);
    }
    numPlataformas = posPlataformas.size();

    printf("\n\n\nIniciando Evolução:\n");
}

//funções de callback para inputs na tela glfw no ambiente opengl

//se aperta enter inicia simulação
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        readingInput = false;
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        showScreen = !showScreen;
        std::cout << "showScreen: " << showScreen << std::endl;
    }
        
}

//se clicar com o botão esquedo do mouse adiciona bola na simulação
//se clicar com o botão direito do mouse adiciona plataforma na simulação
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos = xpos / width;
    xpos = left + xpos * (right-left);

    ypos = ypos / height;
    ypos = top - ypos * (top-bottom);
    //std::cout << xpos << " , " << ypos << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        posBolas.push_back(std::make_pair(xpos,ypos));
        simuPtr->inserirBola(xpos,ypos);
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        posPlataformas.push_back(std::make_pair(xpos,ypos));
        
        simuPtr->inserirPlataforma(xpos,ypos,0);
        if(popInit){
            for(int i=0;i<NUM_INDIVIDUOS;i++){
                anglePlataformas[i].push_back(0);
            }
        }else{
            anglePlataformasSet.push_back(0);
        }
    }
        
}

//faz a leitura do input com o usuario clicando onde colocar as bolas e plataformas na tela
void clickIn(){
    printf("Digite o numero de scores que teremos: ");
    int nScores;
    scanf("%d", &nScores);
    printf("Digite os scores separados por espaço ou \\n agora: ");
    for(int i=0;i<nScores;i++){
        float score;
        scanf("%f", &score);
        scores.push_back(score);
    }
    printf("Agora clique com o botão esquerdo onde ficarão as bolas, e com o botão direito onde ficarão as plataformas\n");
    printf("E digite Enter com a janela OpenGL em foco para terminar de adicionar novos itens e iniciar a simulação\n\n");
    
    simuPtr->newScore(scores);

    glfwShowWindow(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    while(!glfwWindowShouldClose(window) && readingInput){
        glfwPollEvents();
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        simuPtr->drawFrame();

        glfwSwapBuffers(window);
    }
    numPlataformas = posPlataformas.size();

}



int main(){
    srand(4241);

    //inicia ambiente de janelas
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, false);

    window = glfwCreateWindow(width, height, "Cenario", NULL, NULL);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s\n", glewGetErrorString(GlewInitResult));

    //inicia shader openGL
    Shader programShader = Shader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");

    //matriz de projeção para arrumar tela baseado nos parametros de tamanho
    float mat[16] = {
        2/(right-left),0,0,-(right+left)/(right-left),
        0,2/(top-bottom),0,-(top+bottom)/(top-bottom),
        0,0,1,0,
        0,0,0,1
    };
    programShader.setMat("projection",mat);


    //chamando funções de leitura baseado no input

    printf("Digite 1 para entrada com texto ou 2 para entrada com mouse e teclado: \n");
    int inputType;
    scanf("%d", &inputType);
    if(inputType == 1){
        readTextIn();
    }

    gameSimulation simu(posBolas,posPlataformas,anglePlataformasSet,scores,&programShader);
    simuPtr = &simu;

    if(inputType == 2){
        clickIn();
    }

    outGrafico = fopen("evolucao.csv","w");
    fprintf(outGrafico,"Geração,Score Melhor Individuo,Score Medio Individuos\n");
    glfwShowWindow(window);

    //loop principal
    initpop(); //inicia população
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents(); //verifica eventos de mouse e teclado
        avalia(&simu); //avalia individuos
        show_melhor(&simu); //mostra na tela o melhor
        elitismo(); //pega o melhor e mistura gene com todos alem de mutar eles
        alteraTaxaMutacao(); //muda a taxa de mutação se necessaio
        geracao++;
    }

    fclose(outGrafico);

    return 0;
}