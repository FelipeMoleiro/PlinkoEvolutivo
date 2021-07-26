#include "object.hpp"

#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

typedef struct{
    float x, y;
} coordenadas;

object::object(Shader *program)
{
    this->program = program;
}

object::~object()
{
}

void object::set_pos(float x,float y){
    this->x = x;
    this-> y = y;
}

void object::set_angle(float angle){
    this->angle = angle;
}

quadrado::quadrado(Shader* program,float x,float y,float angle,float width,float height) : object(program)
{
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    this->angle = angle;

    this->program->useShader();
    
    //generate buffers
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1,&this->VBO);

    //bind buffers
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    coordenadas vertices[4] = {
        {-width/2.0f,-height/2.0f},
        {-width/2.0f,height/2.0f},
        {width/2.0f,-height/2.0f},
        {width/2.0f,height/2.0f}
    };


    //Send data to GPU buffers
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(coordenadas), vertices, GL_STATIC_DRAW);

    GLuint loc_position = glGetAttribLocation(this->program->getID(), "position");

    // vertex positions
    glEnableVertexAttribArray(loc_position);	
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //unbind vertex array
    glBindVertexArray(0);
    
}

quadrado::~quadrado()
{
}

void quadrado::draw(){
    this->program->useShader();
    this->program->setVec4("color",glm::vec3(1,0,0));
    float c = cos(this->angle*M_PI/180);
    float s = sin(this->angle*M_PI/180);
    float mat[16] = {
        c,-s,0,x,
        s,c,0,y,
        0,0,1,0,
        0,0,0,1
    };
    this->program->setMat("model",mat);
    //printf("%d\n",  this->program->getID());
    //printf("%f %f\n", x, y);
    //printf("%f %f\n",  width, height);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

circle::circle(Shader* program,float x,float y,float angle,float radius) : object(program)
{
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->program->useShader();
    
    //generate buffers
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1,&this->VBO);

    //bind buffers
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    coordenadas vertices[30];

    float step = 360/30;
    float angleCircleCreation = 0;
    for(int i=0;i<30;i++){
        vertices[i].x = radius * cos(angleCircleCreation*M_PI/180);
        vertices[i].y = radius * sin(angleCircleCreation*M_PI/180);
        angleCircleCreation += step;
    }

    //Send data to GPU buffers
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(coordenadas), vertices, GL_STATIC_DRAW);

    GLuint loc_position = glGetAttribLocation(this->program->getID(), "position");

    // vertex positions
    glEnableVertexAttribArray(loc_position);	
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //unbind vertex array
    glBindVertexArray(0);
    
}

circle::~circle()
{
}

void circle::draw(){
    this->program->useShader();
    this->program->setVec4("color",glm::vec3(1,0,0));
    float c = cos(this->angle*M_PI/180);
    float s = sin(this->angle*M_PI/180);
    float mat[16] = {
        c,-s,0,x,
        s,c,0,y,
        0,0,1,0,
        0,0,0,1
    };
    this->program->setMat("model",mat);
    //printf("%d\n",  this->program->getID());
    //printf("%f %f\n", x, y);
    //printf("%f %f\n",  width, height);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0,30);
}