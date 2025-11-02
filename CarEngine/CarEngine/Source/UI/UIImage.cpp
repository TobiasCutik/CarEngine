#include "UIImage.h"
#include "Core/World.h"

namespace CE
{
    UIImage::UIImage(World* world, Material* material, glm::vec2 scale, glm::vec2 position, float zRotation, bool bIsActive, std::string name)
        : UIComponent(glm::vec3 {position.x, -position.y, 0.f}, glm::vec3 {-90.f, 0.f, zRotation}, glm::vec3 {scale.x, 1.f, scale.y},
                      bIsActive, std::move(name)),
        uiImageMesh(Mesh(world, world->getAssetManager()->getModel("CE_Plane")))
    {
        uiImageMesh.setMaterial(material);

        setComponentType(UIComponentType::IMAGE);
    }
}