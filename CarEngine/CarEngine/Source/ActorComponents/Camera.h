#pragma once
#include "../Core/Transformable.h"
#include "ActorComponent.h"

namespace CE
{
    class Camera : public Transformable, public ActorComponent {

    public:
        explicit Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                        bool bIsActive = false,
                        float fovY = 60.f,
                        float zFarPlane = 500.f,
                        std::string name = "New Camera");

        void setFovY(float fov);

        float getFovY() const { return fovY; }
        float getZFarPlane() const { return zFarPlane; }

        const static ActorComponentType componentType = ActorComponentType::CAMERA;

    private:
        float fovY = 60.f;
        float zFarPlane = 100.f;
    };
}