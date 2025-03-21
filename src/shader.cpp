#include <shader.hpp>
// shamelessly copied old project

Shader::Shader(const std::string vertexPath, const std::string fragmentPath) {
    ID = glCreateProgram();

    // Open file -> code
    std::string vertexCode, fragmentCode, line = "";
    std::ifstream vertexFile(vertexPath.c_str());
    if (vertexFile.is_open()) {
        while (std::getline(vertexFile, line)) {
            vertexCode += line + '\n';
        }
        vertexFile.close();
    }

    std::ifstream fragmentFile(fragmentPath.c_str());
    if (fragmentFile.is_open()) {
        while (std::getline(fragmentFile, line)) {
            fragmentCode += line + '\n';
        }
        fragmentFile.close();
    }

    // Compile shader code
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* v_src = vertexCode.c_str();
    const char* f_src = fragmentCode.c_str();

    glShaderSource(vertexShader, 1, &v_src, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    glShaderSource(fragmentShader, 1, &f_src, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Attach shaders to ID
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Validate Program
    glValidateProgram(ID);

    // Clean up
    glDetachShader(ID, vertexShader);
    glDetachShader(ID, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(ID); }
void Shader::destroy() { glDeleteProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}