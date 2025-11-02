#pragma once

#include <xstring>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <filesystem>
#include <glad/glad.h>
#include <map>
#include "../Material/Material.h"
#include "Core/Transformable.h"

namespace CE
{
    struct ShaderCameraData
    {
        glm::mat4 viewMatrix = glm::mat4(1.f);
        glm::mat4 projectionMatrix = glm::mat4(1.f);
        glm::vec4 cameraPosition = glm::vec4(1.f);
    };

    struct ShaderLightData
    {
        glm::mat4 lightViewMatrix = glm::mat4(1.f);
        glm::mat4 lightProjectionMatrix = glm::mat4(1.f);
        glm::vec4 lightPosition = glm::vec4(1.f);
        glm::vec4 lightColor = glm::vec4(1.f);
        float ambientLightStrength;
        float diffuseLightStrength;
        float specularLightStrength;
        float shadowStrength;
    };

    class Shader {

    public:
        typedef struct {
            std::map<std::string, GLint> attributes;
        } VertexAttributes;

        Shader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name);

        // delete copy constructor
        Shader(const Shader& shader) = delete;

        // explicit move constructor
        Shader(Shader &&other) noexcept{
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }

        void use() const;
        void setUniformMatrix(const std::string& uniform, glm::mat4 matrix) const;
        void setUniformVector4(const std::string& uniform, glm::vec4 vector) const;
        void setUniformVector3(const std::string& uniform, glm::vec3 vector) const;
        void setUniformFloat(const std::string& uniform, float value) const;
        void setUniformInteger(const std::string& uniform, int value) const;

        virtual void setShaderCameraData(ShaderCameraData* shaderCameraData) { }
        virtual void setShaderLightData(ShaderLightData* shaderLightData) { }
        virtual void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) = 0;
        virtual void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) = 0;
        virtual void setUiProjectionMatrix(glm::mat4 matrix) { }

        std::string name;
        VertexAttributes vertexAttributes;

        ~Shader();

    private:
        unsigned int loadAndCompileShader(unsigned int shaderType, const std::filesystem::path& shaderAsset);

        GLint getAttributeLocationFromName(const std::string& attribute) const;
        void checkShaderErrors(unsigned int object, unsigned int type) const;

        unsigned int shaderProgram = 0;
        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;
    };
}