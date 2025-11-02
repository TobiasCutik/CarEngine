#pragma once
#include "UIComponent.h"
#include "Rendering/Mesh/Mesh.h"

namespace CE
{
    class UIImage : public UIComponent {

    public:
        explicit UIImage(World* world,
                         Material* material,
                         glm::vec2 scale = glm::vec2(100.f, 100.f),
                         glm::vec2 position = glm::vec2(0.f, 0.f),
                         float zRotation = 0.f,
                         bool bIsActive = true,
                         std::string name = "New UIImage");

        Mesh* getMesh() { return &uiImageMesh; }

        const static UIComponentType componentType = UIComponentType::IMAGE;

    protected:
        Mesh uiImageMesh;
    };
}