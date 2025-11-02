#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "MeshNode.h"
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>

namespace CE
{
    void MeshNode::checkMeshErrors(const std::string& description)
    {
        GLenum e = glGetError();
        if (e != GL_NO_ERROR)
        {
            fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", description.c_str(), e, e);
            exit(20);
        }
    }

    MeshNode::MeshNode(tinygltf::Model& model, std::vector<tinygltf::Primitive>* meshPrimitives, tinygltf::Node* node, std::map<int, BufferState>& bufferStates, Shader::VertexAttributes& vertexAttributes,
        std::string name)
        : meshPrimitivesPtr(meshPrimitives), name(std::move(name))
    {
        calculateLocalTransformation(node);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (const auto& primitive : *meshPrimitivesPtr)
        {
            if (primitive.indices < 0)
                return;

            auto it(primitive.attributes.begin());
            auto itEnd(primitive.attributes.end());

            for (; it != itEnd; it++)
            {
                assert(it->second >= 0);
                const tinygltf::Accessor &accessor = model.accessors[it->second];
                checkMeshErrors("bind buffer");
                glBindBuffer(GL_ARRAY_BUFFER, bufferStates[accessor.bufferView].vertexBuffer);
                checkMeshErrors("bind buffer");
                int size = 1;
                if (accessor.type == TINYGLTF_TYPE_SCALAR)
                    size = 1;
                else if (accessor.type == TINYGLTF_TYPE_VEC2)
                    size = 2;
                else if (accessor.type == TINYGLTF_TYPE_VEC3)
                    size = 3;
                else if (accessor.type == TINYGLTF_TYPE_VEC4)
                    size = 4;
                else
                    assert(0);

                // it->first would be "POSITION", "NORMAL", "TEXCOORD_0", ...
                if ((it->first == "POSITION") || (it->first == "NORMAL") || (it->first == "TEXCOORD_0"))
                {
                    if (vertexAttributes.attributes[it->first] >= 0)
                    {
                        // Compute byteStride from Accessor + BufferView combination.
                        int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                        assert(byteStride != -1);
                        glVertexAttribPointer(vertexAttributes.attributes[it->first], size, accessor.componentType,
                                              accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
                        checkMeshErrors("vertex attrib pointer");
                        glEnableVertexAttribArray(vertexAttributes.attributes[it->first]);
                        checkMeshErrors("enable vertex attrib array");
                    }
                }
            }

            const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferStates[indexAccessor.bufferView].vertexBuffer);
            checkMeshErrors("bind buffer");

            if (primitive.mode == TINYGLTF_MODE_TRIANGLES)
                drawMode = GL_TRIANGLES;
            else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP)
                drawMode = GL_TRIANGLE_STRIP;
            else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN)
                drawMode = GL_TRIANGLE_FAN;
            else if (primitive.mode == TINYGLTF_MODE_POINTS)
                drawMode = GL_POINTS;
            else if (primitive.mode == TINYGLTF_MODE_LINE)
                drawMode = GL_LINES;
            else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP)
                drawMode = GL_LINE_LOOP;
            else
                assert(0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void MeshNode::calculateLocalTransformation(tinygltf::Node* node)
    {
        if (node->translation.size() == 3)
            transform.position = {node->translation[0], node->translation[1], node->translation[2]};

        if (node->rotation.size() == 4)
        {
            auto rotationQuaternion = glm::quat((float) node->rotation[3], (float) node->rotation[0], (float) node->rotation[1], (float) node->rotation[2]);
            transform.setRotationQ(rotationQuaternion);
        }

        if (node->scale.size() == 3)
            transform.scale = {node->scale[0], node->scale[1], node->scale[2]};
    }

    void MeshNode::drawMeshNode(tinygltf::Model& model, Shader* shader)
    {
        if (!bIsActive)
            return;

        shader->setMeshNodeSpecificUniforms(&transform, material);

        glBindVertexArray(vao);

        for (const auto& primitive : *meshPrimitivesPtr)
        {
            if (primitive.indices < 0)
                return;

            const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

            glDrawElements(drawMode, (int) indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
            checkMeshErrors("draw elements");
        }

        glBindVertexArray(0);
    }

    MeshNode::~MeshNode()
    {
        glDeleteVertexArrays(1, &vao);
    }
}