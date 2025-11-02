#define TINYGLTF_IMPLEMENTATION

#include <iostream>
#include <utility>
#include "Mesh.h"
#include "Core/World.h"

namespace CE
{
    size_t componentTypeByteSize(int type)
    {
        switch (type)
        {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            case TINYGLTF_COMPONENT_TYPE_BYTE:
                return sizeof(char);
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            case TINYGLTF_COMPONENT_TYPE_SHORT:
                return sizeof(short);
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            case TINYGLTF_COMPONENT_TYPE_INT:
                return sizeof(int);
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                return sizeof(float);
            case TINYGLTF_COMPONENT_TYPE_DOUBLE:
                return sizeof(double);
            default:
                return 0;
        }
    }

    void Mesh::setupMesh(tinygltf::Model& model)
    {
        for (size_t i = 0; i < model.bufferViews.size(); i++)
        {
            const tinygltf::BufferView &bufferView = model.bufferViews[i];
            if (bufferView.target == 0)
            {
                std::cout << "WARN: bufferView.target is zero" << std::endl;
                continue;  // Unsupported bufferView.
            }

            int sparse_accessor = -1;
            for (size_t a_i = 0; a_i < model.accessors.size(); ++a_i)
            {
                const auto &accessor = model.accessors[a_i];
                if (accessor.bufferView == i)
                {
                    std::cout << i << " is used by accessor " << a_i << std::endl;
                    if (accessor.sparse.isSparse)
                    {
                        std::cout
                        << "WARN: this bufferView has at least one sparse accessor to it. We are going to load the data as patched by this sparse accessor, not the original data"
                        << std::endl;
                        sparse_accessor = a_i;
                        break;
                    }
                }
            }

            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
            MeshNode::BufferState state;
            glGenBuffers(1, &state.vertexBuffer);
            glBindBuffer(bufferView.target, state.vertexBuffer);
            std::cout << "buffer.size= " << buffer.data.size() << ", byteOffset = " << bufferView.byteOffset << std::endl;

            MeshNode::checkMeshErrors("bind buffer");

            if (sparse_accessor < 0)
            {
                glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
            }
            else
            {
                const auto accessor = model.accessors[sparse_accessor];

                // copy the buffer to a temporary one for sparse patching
                auto *tmp_buffer = new unsigned char[bufferView.byteLength];
                memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset,bufferView.byteLength);

                const size_t size_of_object_in_buffer = componentTypeByteSize(accessor.componentType);
                const size_t size_of_sparse_indices = componentTypeByteSize(accessor.sparse.indices.componentType);

                const auto &indices_buffer_view = model.bufferViews[accessor.sparse.indices.bufferView];
                const auto &indices_buffer = model.buffers[indices_buffer_view.buffer];

                const auto &values_buffer_view = model.bufferViews[accessor.sparse.values.bufferView];
                const auto &values_buffer = model.buffers[values_buffer_view.buffer];

                for (size_t sparse_index = 0; sparse_index < accessor.sparse.count; ++sparse_index)
                {
                    int index = 0;

                    switch (accessor.sparse.indices.componentType)
                    {
                        case TINYGLTF_COMPONENT_TYPE_BYTE:
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                            index = (int)*(unsigned char *)(indices_buffer.data.data() + indices_buffer_view.byteOffset +
                                                            accessor.sparse.indices.byteOffset + (sparse_index * size_of_sparse_indices));
                            break;
                        case TINYGLTF_COMPONENT_TYPE_SHORT:
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                            index = (int)*(unsigned short *)(indices_buffer.data.data() + indices_buffer_view.byteOffset +
                                                            accessor.sparse.indices.byteOffset + (sparse_index * size_of_sparse_indices));
                            break;
                        case TINYGLTF_COMPONENT_TYPE_INT:
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                            index = (int)*(unsigned int *)(indices_buffer.data.data() + indices_buffer_view.byteOffset +
                                                            accessor.sparse.indices.byteOffset + (sparse_index * size_of_sparse_indices));
                            break;
                    }

                    std::cout << "updating sparse data at index  : " << index << std::endl;
                    // index is now the target of the sparse index to patch in
                    const unsigned char *read_from = values_buffer.data.data() + (values_buffer_view.byteOffset +
                                                     accessor.sparse.values.byteOffset) + (sparse_index * (size_of_object_in_buffer * accessor.type));

                    unsigned char *write_to = tmp_buffer + index * (size_of_object_in_buffer * accessor.type);

                    memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);
                }

                glBufferData(bufferView.target, bufferView.byteLength, tmp_buffer, GL_STATIC_DRAW);
                delete[] tmp_buffer;
            }
            glBindBuffer(bufferView.target, 0);
            MeshNode::checkMeshErrors("bind buffer");

            bufferStates[i] = state;
        }
    }

    void Mesh::setupMeshNodesFromModel(tinygltf::Model& model)
    {
        // sort model nodes by name
        std::map<std::string, int> nodeMap;
        for (int i = 0; i < model.nodes.size(); ++i)
            nodeMap.insert({model.nodes[i].name, i});

        for (const auto& [name, index] : nodeMap)
            setupMeshNodesFromNode(model, model.nodes[index]);
    }

    // Hierarchically search for meshes and make MeshNodes with them
    void Mesh::setupMeshNodesFromNode(tinygltf::Model& model, tinygltf::Node& node)
    {
        if (node.mesh > -1)
        {
            // Make a MeshNode from the mesh and add it to meshNodes
            assert(node.mesh < model.meshes.size());
            auto meshNode = MeshNode(model, &model.meshes[node.mesh].primitives, &node, bufferStates, vertexAttributes, model.nodes[node.mesh].name);
            meshNodes.emplace_back(std::move(meshNode));
        }

        // Search for meshes in child nodes
        for (int nodeIndex : node.children)
        {
            assert(nodeIndex < model.nodes.size());
            setupMeshNodesFromNode(model, model.nodes[nodeIndex]);
        }
    }

    Mesh::Mesh(World* world, tinygltf::Model gltfModel) : vertexAttributes(world->getShaderVertexAttributes()), model(std::move(gltfModel))
    {
        setupMesh(model);
        MeshNode::checkMeshErrors("setup mesh");

        setupMeshNodesFromModel(model);
    }

    void Mesh::setMaterials(const std::vector<ColorMaterial*>& materials)
    {
        for (int i = 0; i < meshNodes.size() && i < materials.size(); i++)
        {
            meshNodes[i].material = materials[i];
        }
    }

    void Mesh::setMaterials(const std::vector<TextureMaterial*>& materials)
    {
        for (int i = 0; i < meshNodes.size() && i < materials.size(); i++)
        {
            meshNodes[i].material = materials[i];
        }
    }

    void Mesh::setMaterial(Material* material)
    {
        for (auto& meshNode : meshNodes)
        {
            meshNode.material = material;
        }
    }

    void Mesh::setMaterialOfMeshNode(unsigned int meshNodeIndex, Material* material)
    {
        if (meshNodeIndex < meshNodes.size())
            meshNodes.at(meshNodeIndex).material = material;
    }

    std::vector<Material*> Mesh::getMaterials()
    {
        std::vector<Material*> materials;

        for (auto& meshNode : meshNodes)
        {
            materials.emplace_back(meshNode.material);
        }

        return materials;
    }

    void Mesh::draw(Shader* shader)
    {
        for (auto& node : meshNodes)
            node.drawMeshNode(model, shader);
    }

    Mesh::~Mesh()
    {
        for (auto& bufferState : bufferStates)
            glDeleteBuffers(1, &bufferState.second.vertexBuffer);
    }
}