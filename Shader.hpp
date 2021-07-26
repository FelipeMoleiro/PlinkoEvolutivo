/*Felipe Guilermmo Santuche Moleiro - 10724010*/
#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <GL/gl.h>  /* verifique no seu SO onde fica o gl.h */
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <glm/glm.hpp>

class Shader{
private:
    GLuint programID;
public:
    Shader(const char* verticeShaderPath,const char* fragmentShaderPath);
    //
    ~Shader();
    void useShader();
    GLuint getID();

    void setMat(const char* name, glm::mat4 matrix);
    void setMat(const char* name, float matrix[16]);
    void setVec3(const char* name, glm::vec3 values);
    void setVec4(const char* name, glm::vec3 values);
    GLuint getUniformLocation(const char* name);
};

#endif