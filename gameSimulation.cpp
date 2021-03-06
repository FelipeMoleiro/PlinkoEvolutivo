#include "gameSimulation.hpp"
#include "globals.hpp"
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 

#include <Box2D/Box2D.h>

//Recebe posições iniciais das bolas e plataformas, mais os seus angulos, e os scores e cria a simulação do jogo
gameSimulation::gameSimulation(std::vector<std::pair<float,float>> &posBolas,std::vector<std::pair<float,float>> &posPlataformas,
                                std::vector<float> &anglePlataformas,std::vector<float> &scores,Shader* program)
{
    this->posBolas = posBolas;
    this->posPlataformas = posPlataformas;
    this->anglePlataformas = anglePlataformas;
    this->scores = scores;
    this->programShader = program;

    
    if(DISCRETE){
        //posBarreiras
        float posTmp = left;
        posBarreiras.push_back(left);
        for(unsigned int i=0;i<scores.size();i++){
            posTmp = left + (right-left)*(i+1.0)/scores.size();
            posBarreiras.push_back(posTmp);
        }
    }

    //Criando objetos openGL para mostrar na tela
    for(unsigned int i = 0;i<posBolas.size();i++){
        bolas.push_back(circle(programShader,posBolas[i].first,posBolas[i].second,0,raioBola));
    }

    for(unsigned int i = 0;i<posPlataformas.size();i++){
        plataformas.push_back(quadrado(programShader,posPlataformas[i].first,posPlataformas[i].second,anglePlataformas[i],widthPlataforma,heightPlataforma));
    }

    if(DISCRETE){
        for(unsigned int i=0; i<posBarreiras.size();i++){
            barreiras.push_back(quadrado(programShader,posBarreiras[i],0.5,0,0.1,1));
        }
    }

    
}

//inserir mais bolas depois de criada a simulação
void gameSimulation::inserirBola(float posx,float posy){
    posBolas.push_back(std::make_pair(posx,posy));
    bolas.push_back(circle(programShader,posx,posy,0,raioBola));
}

//inserir mais plataformas depois de criada a simulação
void gameSimulation::inserirPlataforma(float posx,float posy,float angle){
    posPlataformas.push_back(std::make_pair(posx,posy));
    anglePlataformas.push_back(angle);
    plataformas.push_back(quadrado(programShader,posx,posy,0,widthPlataforma,heightPlataforma));
}

//mudar o vetor de scores e recalcula barreiras(cestas de score)
void gameSimulation::newScore(std::vector<float> &scores){
    this->scores = scores;
    if(!DISCRETE) return; //se estivermos no modo continuo nao tem pq atualizar score
    posBarreiras.clear();
    //posBarreiras
    float posTmp = left;
    posBarreiras.push_back(left);
    for(unsigned int i=0;i<scores.size();i++){
        posTmp = left + (right-left)*(i+1.0)/scores.size();
        posBarreiras.push_back(posTmp);
    }

    barreiras.clear();
    for(unsigned int i=0; i<posBarreiras.size();i++){
        barreiras.push_back(quadrado(programShader,posBarreiras[i],0.5,0,0.1,1));
    }
}

//função usada para a versão continua do jogo, que nao utiliza cestas de scores e sim uma func continua
float funcValue(float x){
    return -(x*x) + 2;
}

//desenha um frame da simulação
void gameSimulation::drawFrame(){
    for(unsigned int i = 0;i<bolas.size();i++){
        bolas[i].draw();
    }

    for(unsigned int i = 0;i<plataformas.size();i++){
        plataformas[i].draw();
    }

    for(unsigned int i = 0;i<barreiras.size();i++){
        barreiras[i].draw();
    }
}

//reseta a posição dos objetos bolas opengl
void gameSimulation::resetBolasGL(){
    for(unsigned int i = 0;i<posBolas.size();i++){
        bolas[i].set_pos(posBolas[i].first,posBolas[i].second);
    }
}

//simula o jogo mostrando na tela em tempo real ou nao(realTime != 0 é tempo real)
float gameSimulation::simulate(float *realTime){

//INICIANDO MUNDO DA SIMULAÇÃO
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    //criando as plataformas na simulação
    std::vector<b2Body*> plataformasBox;

    for(unsigned int i = 0;i<posPlataformas.size();i++){
        b2BodyDef bodyDef;
        //bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(posPlataformas[i].first, posPlataformas[i].second);
        bodyDef.angle = anglePlataformas[i]*b2_pi/180.0;;
        plataformasBox.push_back(world.CreateBody(&bodyDef));

        b2PolygonShape groundBox;
        groundBox.SetAsBox(widthPlataforma/2.0, heightPlataforma/2.0);

        plataformasBox[i]->CreateFixture(&groundBox,0.0f);

    }

    //criando as bolas na simulação
    std::vector<b2Body*> bolasBox;

    for(unsigned int i = 0;i<posBolas.size();i++){
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(posBolas[i].first, posBolas[i].second);

        bolasBox.push_back(world.CreateBody(&bodyDef));

        b2CircleShape circle;
        //circle.m_p.Set(posBolas[i].first, posBolas[i].second);
        circle.m_radius = raioBola;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        bolasBox[i]->CreateFixture(&fixtureDef);

    }

    //criando as barreiras das cestas na simulação
    std::vector<b2Body*> barreirasBox;

    for(unsigned int i = 0;i<posBarreiras.size();i++){
        b2BodyDef bodyDef;
        //bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(barreiras[i].x, barreiras[i].y);
        barreirasBox.push_back(world.CreateBody(&bodyDef));

        b2PolygonShape groundBox;
        groundBox.SetAsBox(barreiras[i].width/2.0, barreiras[i].height/2.0);

        barreirasBox[i]->CreateFixture(&groundBox,0.0f);

    }

    //inicia visitados com false
    bool visitados[bolas.size()];
    for(unsigned int i=0;i<bolas.size();i++){
        visitados[i] = false;
    }

    //parametros da simulação(60 frames por segundo, e numero de iterações de resolução da simulação fisica)
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    float score = 0;

    if(*realTime){
        // INICIALIZANDO AMBIENTE OPENGL
        GLFWwindow* window = glfwGetCurrentContext();
        
        //simula printando na tela
        for(int step=0;step<timeSimulation*60;step++){
            clock_t begin = clock();
            while (*realTime && window != NULL && !glfwWindowShouldClose(window) && (double)(clock() - begin) / CLOCKS_PER_SEC < 1.0/60.0){
                glfwPollEvents();
            }
            if(*realTime && window != NULL && !glfwWindowShouldClose(window)){
                glClearColor(0,1.0,1.0,1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }


            world.Step(timeStep, velocityIterations, positionIterations); //step smiluation
            for(unsigned int i = 0;i<bolasBox.size();i++){
                b2Vec2 position = bolasBox[i]->GetPosition();

                if(!visitados[i] && position.y < 0){

                    visitados[i] = true;
                    if(!DISCRETE){ //se versao continua usa funcValue, se não usa vetor de scores
                        score += funcValue(position.x);
                    }else{
                        float pos = ((position.x - left) / 8 )* scores.size();
                        if(pos >= 0 && pos < scores.size()){
                            score += scores[(int)pos];
                        }
                    }

                }
                bolas[i].set_pos(position.x,position.y);

            }

            int fim = 1;
            for(unsigned int i = 0;i<bolasBox.size();i++){ //verifica se todas as bolas ja cairam em cestas
                if(!visitados[i]) fim=0;
            }
            if(fim) break;

            

            if(*realTime && window != NULL && !glfwWindowShouldClose(window)){
                drawFrame();
                glfwSwapBuffers(window);
            }

        }
        if(window != NULL && glfwWindowShouldClose(window)){
            glfwDestroyWindow(window);
        }
        for(unsigned int i = 0;i<bolas.size();i++){
            bolas[i].set_pos(posBolas[i].first,posBolas[i].second);
        }

    }else{
        for(int step=0;step<timeSimulation*60;step++){
        
            world.Step(timeStep, velocityIterations, positionIterations); //step smiluation
            for(unsigned int i = 0;i<bolasBox.size();i++){
                b2Vec2 position = bolasBox[i]->GetPosition();

                if(!visitados[i] && position.y < 0){
                    visitados[i] = true;

                    if(!DISCRETE){ //se versao continua usa funcValue, se não usa vetor de scores
                        score += funcValue(position.x);
                    }else{
                        float pos = ((position.x - left) / 8 )* scores.size();
                        if(pos >= 0 && pos < scores.size()){
                            score += scores[(int)pos];
                        }
                    }
                }
                bolas[i].set_pos(position.x,position.y);

            }
            int fim = 1;
            for(unsigned int i = 0;i<bolasBox.size();i++){ //verifica se todas as bolas ja cairam em cestas
                if(!visitados[i]) fim=0;
            }
            if(fim) break;
        }
    }

    return score;
    
}

gameSimulation::~gameSimulation()
{
}

//seta os angulos da simulação
void gameSimulation::set_angles(std::vector<float> &anglePlataformas){
    this->anglePlataformas = anglePlataformas;
    for(unsigned int i = 0;i<plataformas.size();i++){
        plataformas[i].set_angle(anglePlataformas[i]);
    }
}
