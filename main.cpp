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

int numPlataformas;

float mutacaoMax = 5; //em graus
float porcentagemMutacao = 0.1;

#define NUM_INDIVIDUOS 10
std::vector<float> anglePlataformas[NUM_INDIVIDUOS];
float scoresIndividuos[NUM_INDIVIDUOS];

float bestPastInd[5];

int geracao = 0;

int popInit = 0;

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

void randomize_gene(std::vector<float> &gene){
    for(unsigned int i=0;i<gene.size();i++){
        gene[i] = ((double)rand())/RAND_MAX * 360;
    }
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

void alteraTaxaMutacao(){
    if(bestPastInd[4]-bestPastInd[0]<0.00001)
		mutacaoMax = mutacaoMax*1.2;
	else
		mutacaoMax = 5;

    std::cout << "Mutação Max:" << mutacaoMax << std::endl;

	if(mutacaoMax>10*360){
        genocidio();
		mutacaoMax = 5;
    }

}

bool readingInput = true;
gameSimulation* simuPtr;
GLFWwindow* window;


void show_melhor(gameSimulation* simu){
    float maxScore = scoresIndividuos[0];
    int maxi = 0;

    for (int i=1;i<NUM_INDIVIDUOS;i++){ 
        if (scoresIndividuos[i]>maxScore){
            maxScore = scoresIndividuos[i];
            maxi = i;
        }
    }

    // Guarda ultimos melhores individuos
	for (int i=1;i<5;i++)
	{
		bestPastInd[i-1] = bestPastInd[i];
	}
	bestPastInd[4] = maxScore;

    std::cout << "Geração " << geracao << ": " << maxScore << std::endl;

    if(bestPastInd[4]-bestPastInd[3]>0.00001 || bestPastInd[4] < bestPastInd[3]){
        simu->set_angles(anglePlataformas[maxi]);
        simu->simulate(SHOW_SCREEN);
    }else{
        simu->set_angles(anglePlataformas[maxi]);
        glfwPollEvents();
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        simuPtr->resetBolasGL();
        simuPtr->drawFrame();

        glfwSwapBuffers(window);

    }

}




std::vector<std::pair<float,float>> posBolas;
std::vector<std::pair<float,float>> posPlataformas;
std::vector<float> anglePlataformasSet;
std::vector<float> scores;





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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        readingInput = false;
}

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
    //
}

int main(){
    srand(4241);
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, false);

    window = glfwCreateWindow(width, height, "Cenario", NULL, NULL);

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

// Setando parametros da simulação
    
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

    glfwShowWindow(window);

    initpop();
    while(!glfwWindowShouldClose(window)){
        //printf("oi\n");
        glfwPollEvents();
        avalia(&simu);
        show_melhor(&simu);
        elitismo();
        alteraTaxaMutacao();
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