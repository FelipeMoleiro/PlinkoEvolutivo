#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexShaderPath,const char* fragmentShaderPath){
    //read files
    std::ifstream vertexShaderif;
	vertexShaderif.open(vertexShaderPath);
    std::stringstream bufferVertex;
    bufferVertex << vertexShaderif.rdbuf();

    std::string vertexShader = bufferVertex.str();


    std::ifstream fragmentShaderif;
    fragmentShaderif.open(fragmentShaderPath);
    std::stringstream bufferFragment;
    bufferFragment << fragmentShaderif.rdbuf();

    std::string fragmentShader = bufferFragment.str();

    // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertex_code = vertexShader.c_str();
    const char* fragment_code = fragmentShader.c_str();

    //std::cout << vertex_code << std::endl;

    //std::cout << fragment_code << std::endl;

    // Associando nosso código-fonte GLSL aos slots solicitados
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compilando o Vertex Shader e verificando erros
    glCompileShader(vertex);

    GLint isCompiled = 0;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(vertex, infoLength, NULL, info);

        printf("Erro de compilacao no Vertex Shader.\n");
        printf("--> %s\n",info);

    }

    
    // Compilando o Fragment Shader e verificando erros
    glCompileShader(fragment);

    isCompiled = 0;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(fragment, infoLength, NULL, info);

        printf("Erro de compilacao no Fragment Shader.\n");
        printf("--> %s\n",info);

    }

    // Associando os programas compilado ao programa principal
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linkagem do programa e definindo como default
    glLinkProgram(program);
    glUseProgram(program);
 
    //delete shaders after linking to program
    glDeleteShader(vertex);
    glDeleteShader(fragment); 

    this->programID = program;
}

Shader::~Shader(){
}



void Shader::useShader(){
    glUseProgram(this->programID); 
}

GLuint Shader::getID(){
    return programID;
}

void Shader::setMat(const char* name, glm::mat4 matrix){ 
    //glUniform1f(glGetUniformLocation(ID, name), value);
    glUseProgram(this->programID);
    int loc_matrix = glGetUniformLocation(this->programID, name);
    glUniformMatrix4fv(loc_matrix, 1, GL_TRUE,glm::value_ptr(matrix));
}

void Shader::setMat(const char* name,float matrix[16]){ 
    //glUniform1f(glGetUniformLocation(ID, name), value);
    glUseProgram(this->programID);
    int loc_matrix = glGetUniformLocation(this->programID, name);
    glUniformMatrix4fv(loc_matrix, 1, GL_TRUE,matrix);
}

void Shader::setVec3(const char* name, glm::vec3 values){ 
    //glUniform1f(glGetUniformLocation(ID, name), value);
    glUseProgram(this->programID);
    int loc_vec = glGetUniformLocation(this->programID, name);
    glUniform3f(loc_vec,values.x,values.y,values.z);
}

void Shader::setVec4(const char* name, glm::vec3 values){ 
    //glUniform1f(glGetUniformLocation(ID, name), value);
    glUseProgram(this->programID);
    int loc_vec = glGetUniformLocation(this->programID, name);
    glUniform4f(loc_vec,values.x,values.y,values.z,1.0f);
}


GLuint Shader::getUniformLocation(const char* name){
    glUseProgram(this->programID);
    return glGetUniformLocation(this->programID, name);
}

