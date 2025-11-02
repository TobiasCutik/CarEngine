#include "Camera.h"
#include <utility>

namespace CE
{
    Camera::Camera(glm::vec3 position, glm::vec3 rotation, bool bIsActive, float fovY, float zFarPlane, std::string name) :
            Transformable(position, rotation),
            fovY(fovY),
            zFarPlane(zFarPlane),
            ActorComponent(bIsActive, std::move(name))
    {
        setComponentType(ActorComponentType::CAMERA);
    }

    void Camera::setFovY(float fov)
    {
        fovY = fov;
    }
}

