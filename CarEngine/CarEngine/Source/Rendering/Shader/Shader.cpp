#include "Shader.h"
#include "Utils/Loader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <utility>

namespace CE
{
    struct GlStatusData
    {
        int success;
        const char* typeName;
        const char* shaderName;
        char* infoLog;
    };

    unsigned int Shader::loadAndCompileShader(unsigned int shaderType, const std::filesystem::path& shaderAsset)
    {
        unsigned int shader = glCreateShader(shaderType);

        auto shaderSrc = Loader::loadShader(shaderAsset);
        const char* shaderSource = shaderSrc.c_str();

        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        checkShaderErrors(shader, shaderType);

        return shader;
    }

    Shader::Shader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name) : name(std::move(name))
    {
        vertexShader = loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderAsset);
        fragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderAsset);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        checkShaderErrors(shaderProgram, GL_PROGRAM);

        glDetachShader(shaderProgram, fragmentShader);
        glDetachShader(shaderProgram, vertexShader);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        vertexAttributes.attributes["POSITION"] = getAttributeLocationFromName("inPosition");
        vertexAttributes.attributes["NORMAL"] = getAttributeLocationFromName("inNormal");
        vertexAttributes.attributes["TEXCOORD_0"] = getAttributeLocationFromName("inTexCoord");
    }

    void Shader::use() const
    {
        glUseProgram(shaderProgram);
    }

    void Shader::setUniformMatrix(const std::string& uniform, glm::mat4 matrix) const
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::setUniformVector4(const std::string& uniform, glm::vec4 vector) const
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void Shader::setUniformVector3(const std::string& uniform, glm::vec3 vector) const
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform3fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void Shader::setUniformFloat(const std::string& uniform, float value) const
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform1f(uniformLocation, value);
    }

    void Shader::setUniformInteger(const std::string& uniform, int value) const
    {
        int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform1i(uniformLocation, value);
    }

    GLint Shader::getAttributeLocationFromName(const std::string& attribute) const
    {
        return glGetAttribLocation(shaderProgram, attribute.c_str());
    }

    void Shader::checkShaderErrors(unsigned int object, unsigned int type) const
    {
        GlStatusData status{};

        status.shaderName = name.c_str();

        if (type == GL_VERTEX_SHADER)
            status.typeName = "VERTEX_SHADER";
        else if (type == GL_FRAGMENT_SHADER)
            status.typeName = "FRAGMENT_SHADER";
        else if (type == GL_PROGRAM)
            status.typeName = "SHADER_PROGRAM";
        else
        {
            std::cout << status.shaderName << ": " << "Invalid type to check shader errors for" << std::endl;
            return;
        }

        if (type == GL_PROGRAM)
        {
            glGetProgramiv(object, GL_LINK_STATUS, &status.success);
            if (!status.success)
            {
                int infoLogLength;
                glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
                status.infoLog = (char*) alloca(infoLogLength * sizeof (char));

                glGetProgramInfoLog(object, infoLogLength, &infoLogLength, status.infoLog);
                glDeleteProgram(object);

                throw std::runtime_error(std::string(status.shaderName) + ": " + "Linking of " + std::string(status.typeName) + " failed:\n" + std::string(status.infoLog));
            }
        }
        else
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &status.success);
            if (!status.success)
            {
                int infoLogLength;
                glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
                status.infoLog = (char*) alloca(infoLogLength * sizeof (char));

                glGetShaderInfoLog(object, infoLogLength, &infoLogLength, status.infoLog);
                glDeleteShader(object);

                throw std::runtime_error(std::string(status.shaderName) + ": " + "Compilation of " + std::string(status.typeName) + " failed:\n" + std::string(status.infoLog));
            }
        }
    }

    Shader::~Shader()
    {
        glDeleteProgram(shaderProgram);
    }
}