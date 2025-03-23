#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

class Shader{
public:
    unsigned int ID;
    Shader(const std::string vertexPath, const std::string fragmentPath);
    
    void use();
    void destroy();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string&name, float value) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif