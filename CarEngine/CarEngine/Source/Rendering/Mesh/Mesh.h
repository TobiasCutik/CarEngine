#pragma once

#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include "MeshNode.h"
#include "Rendering/Material/TextureMaterial.h"

namespace CE
{
    class Mesh {

    public:
        Mesh(World* world, tinygltf::Model gltfModel);

        // delete copy constructor
        Mesh(const Mesh &other) = delete;

        // explicit move constructor
        Mesh(Mesh &&other) noexcept {
            std::swap(this->model, other.model);
            std::swap(this->meshNodes, other.meshNodes);
            std::swap(this->vertexAttributes, other.vertexAttributes);
            std::swap(this->bufferStates, other.bufferStates);
        }

        void setMaterials(const std::vector<ColorMaterial*>& materials);
        void setMaterials(const std::vector<TextureMaterial*>& materials);
        void setMaterial(Material* material);
        void setMaterialOfMeshNode(unsigned int meshNodeIndex, Material* material);
        std::vector<Material*> getMaterials();

        void draw(Shader* shader);

        ~Mesh();

        std::vector<MeshNode> meshNodes;

    private:
        void setupMesh(tinygltf::Model& model);
        void setupMeshNodesFromModel(tinygltf::Model& model);
        void setupMeshNodesFromNode(tinygltf::Model& model, tinygltf::Node& node);

        tinygltf::Model model;
        Shader::VertexAttributes vertexAttributes;
        std::map<int, MeshNode::BufferState> bufferStates;
    };
}