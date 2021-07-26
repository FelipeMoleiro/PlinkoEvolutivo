#ifndef OBJECT_HPP
#define OBJECT_HPP


#include "Shader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>  /* verifique no seu SO onde fica o gl.h */
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <glm/glm.hpp>


class object
{
public:
    Shader *program;
    GLuint VBO,VAO;
    float x,y;
    float angle;
public:
    virtual void draw()=0;
    object(Shader *program);
    ~object();
    void set_pos(float x,float y);
    void set_angle(float angle);
};

class quadrado : public object
{
public:
    float width;
    float height;
public:
    quadrado(Shader *program,float x,float y,float angle,float width,float height);
    ~quadrado();
    void draw(); 
};

class circle : public object
{
public:
    float radius;
    
public:
    circle(Shader *program,float x,float y,float angle, float radius);
    ~circle();
    void draw(); 
};



#endif