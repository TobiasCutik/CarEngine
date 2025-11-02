#include "PlayerCar.h"
#include "Rendering/Material/TextureMaterial.h"
#include "Utils/Loader.h"
#include "Core/World.h"

namespace CD
{
    PlayerCar::PlayerCar(World* world, glm::vec3 position, glm::vec3 rotation) : SedanCarActor(world, position, rotation)
    {
        addComponent<Entity>(Mesh(world, world->getAssetManager()->getModel("Car")),
                                    std::vector {world->getAssetManager()->getMaterial<TextureMaterial>("Car_RedPaint"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_WhitePaint"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Headlight"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Headlight"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Grill"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_BlackPaint"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_WhitePaint"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Grill"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_DarkGrey"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_VeryDarkGrey"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_VeryDarkGrey"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Black"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Black"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Lights"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Lights"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Wheel"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminium"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Wheel"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminium"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Wheel"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminium"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Wheel"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminium"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt"),
                                                 world->getAssetManager()->getMaterial<TextureMaterial>("Car_Black")},
                                    glm::vec3 {0.f, -0.25f, 0.f},
                                    glm::vec3 {0.f, 0.f, 0.f},
                                    glm::vec3 {0.38f, 0.38f, 0.38f},
                                    false,
                                    true,
                                    "car");

        addComponent<Entity>(Mesh(world, world->getAssetManager()->getModel("CE_Box")),
                                    Materials::pink(),
                                    glm::vec3 {0.f, 0.1f, 0.f},
                                    glm::vec3 {0.f, 0.f, 0.f},
                                    glm::vec3 {1.f, 0.5f, 2.f},
                                    false,
                                    false,
                                    "carCollisionVisualization");

        addComponent<Camera>(glm::vec3 {0.f, 1.25f, 2.40f}, glm::vec3 {-21.f, 0.f, 0.f}, true);
    }
}