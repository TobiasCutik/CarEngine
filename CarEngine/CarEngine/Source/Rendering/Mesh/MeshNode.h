#pragma once

#include "Utils/Loader.h"
#include <glad/glad.h>
#include "../Shader/Shader.h"
#include "../../Utils/Materials.h"
#include "../../Core/Transformable.h"

namespace CE
{
    class MeshNode {

    public:
        typedef struct {
            GLuint vertexBuffer;
        } BufferState;

        static void checkMeshErrors(const std::string& description);

        MeshNode(tinygltf::Model& model, std::vector<tinygltf::Primitive>* meshPrimitives, tinygltf::Node* node, std::map<int, BufferState>& bufferStates,
                 Shader::VertexAttributes& vertexAttributes, std::string name);

        // delete copy constructor
        MeshNode(const MeshNode &other) = delete;

        // explicit move constructor
        MeshNode(MeshNode &&other) noexcept {
            std::swap(this->meshPrimitivesPtr, other.meshPrimitivesPtr);
            std::swap(this->vao, other.vao);
            std::swap(this->drawMode, other.drawMode);
            std::swap(this->material, other.material);
            std::swap(this->transform, other.transform);
            std::swap(this->name, other.name);
        }

        void calculateLocalTransformation(tinygltf::Node* node);
        void drawMeshNode(tinygltf::Model& model, Shader* shader);

        bool isActive() const { return bIsActive; }
        void setActive(bool bActive) { bIsActive = bActive; }

        Transformable getTransform() const { return transform; }
        const std::string& getName() const { return name; }

        ~MeshNode();

        Material* material = nullptr;

    private:
        std::vector<tinygltf::Primitive>* meshPrimitivesPtr;
        unsigned int vao{};
        int drawMode{};
        bool bIsActive = true;
        Transformable transform;
        std::string name;
    };
}