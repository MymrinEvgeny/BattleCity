#include"ShaderProgram.h"

#include<iostream>
#include<glm/gtc/type_ptr.hpp>

namespace Renderer {

    bool ShaderProgram::createShader(const std::string& source,
        const GLenum shaderType,
        GLuint& shaderID) {

        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
        programID = shaderProgram.programID;
        programIsCompiled = shaderProgram.programIsCompiled;

        shaderProgram.programID = NULL;
        shaderProgram.programIsCompiled = false;
    }
	ShaderProgram::ShaderProgram(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource) {


        GLuint vertexShaderID;
        if (!createShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShaderID)) {
            std::cerr << "VERTEX SHADER compile time error" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShaderID)) {
            std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            //glGetShaderInfoLog(programID, 1024, nullptr, infoLog);
            glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link time error:\n" << infoLog << std::endl;
        }
        else {
            programIsCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
	}
    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(programID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
        
        glDeleteProgram(programID);
        programID = shaderProgram.programID;
        programIsCompiled = shaderProgram.programIsCompiled;

        shaderProgram.programID = NULL;
        shaderProgram.programIsCompiled = false;
        return *this;
    }

    void ShaderProgram::use() const {
        glUseProgram(programID);
    }
    void ShaderProgram::setInt(const std::string& name, const GLint value) {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    }
    void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()),
            1, GL_FALSE, glm::value_ptr(matrix));
    }
}