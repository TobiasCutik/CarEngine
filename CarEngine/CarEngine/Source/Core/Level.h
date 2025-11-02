#pragma once
#include <string>
#include "Rendering/Skybox.h"
#include "Rendering/Light.h"

namespace CE
{
    class World;

    class Level {

    friend class LevelManager;

    public:
        explicit Level(int index, std::string name = "New Level");
        virtual ~Level() = default;

    protected:
        virtual void setupLevel(World* world);

        std::weak_ptr<Skybox> skybox;
        std::weak_ptr<Light> light;
        std::weak_ptr<Actor> cameraActor;

    private:
        std::string name;
        bool bIsActive = false;
        bool bLoad = false;
        int index;
    };
}