//
// Created by keega on 6/5/2021.
//

#ifndef FORGE_SHADER_H
#define FORGE_SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    // program id
    unsigned int ID;

    //read in the shader and build it
    Shader(const char* vertexPath, const char* fragmentPath);
    // activate the shader
    void use();
    void destroy();
    //utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void set1DFloat(const std::string &name, float value) const;
    void set2DFloat(const std::string &name, float &x, float &y) const;
    // FIXME (materials): laziness from testing the learnOpenGL implementation
    void set3DFloat(const std::string &name, float x, float y, float z) const;
    void set4DFloat(const std::string &name, float &x, float &y, float &z, float &w) const;
    void setMat4(const std::string &name, glm::mat4 &matrix) const;
};


#endif //FORGE_SHADER_H
