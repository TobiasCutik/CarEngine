#include "CarSimulationHandler.h"
#include "chrono/physics/ChSystemNSC.h"
#include "chrono_vehicle/ChWorldFrame.h"
#include "chrono/physics/ChSystemSMC.h"
#include "Core/ActorContainer.h"
#include "CarSimulation/Cars/Car.h"

namespace CE
{
    CarSimulationHandler::CarSimulationHandler(std::weak_ptr<ActorContainer> actorContainer, int maxSimulationIterations, float carSimulationStepSize)
        : maxSimulationIterations(maxSimulationIterations), carSimulationStepSize(carSimulationStepSize), actorContainer(actorContainer)
    {
        registerCarCallbackId = actorContainer.lock()->onActorAdded.addCallback(BIND_FUNC(CarSimulationHandler::registerCar));
        unregisterCarCallbackId = actorContainer.lock()->onActorDeleted.addCallback(BIND_FUNC(CarSimulationHandler::unregisterCar));
    }

    void CarSimulationHandler::setMaxSimulationIterations(int maxIterations)
    {
        maxSimulationIterations = maxIterations;

        if (simulationSystem)
            simulationSystem->SetSolverMaxIterations(maxSimulationIterations);
    }

    double CarSimulationHandler::getSimulationTime()
    {
        if (simulationSystem)
            return simulationSystem->GetChTime();
        else
            return 0;
    }

    void CarSimulationHandler::setContactMethod(ContactMethod contactMethod)
    {
        if (contactMethod == ContactMethod::RIGID)
            simulationSystem = std::make_unique<chrono::ChSystemNSC>();
        else if (contactMethod == ContactMethod::SOFT)
            simulationSystem = std::make_unique<chrono::ChSystemSMC>();

        simulationSystem->Set_G_acc(-9.81 * chrono::vehicle::ChWorldFrame::Vertical());
        simulationSystem->SetSolverMaxIterations(maxSimulationIterations);
        simulationSystem->SetMaxPenetrationRecoverySpeed(4.0);

        auto callback = std::make_shared<CeCollisionCallback>(this);
        simulationSystem->GetContactContainer()->RegisterAddContactCallback(callback);
    }

    void CarSimulationHandler::registerCar(Actor* actor)
    {
        if (auto car = dynamic_cast<Car*>(actor))
            carList.push_back(car);
    }

    void CarSimulationHandler::unregisterCar(Actor* actor)
    {
        if (auto car = dynamic_cast<Car*>(actor))
            carList.erase(std::remove(carList.begin(), carList.end(), car), carList.end());
    }

    void CarSimulationHandler::onCollision(const chrono::collision::ChCollisionInfo& contactInfo)
    {
        Car* carCollisionA = nullptr;
        Car* carCollisionB = nullptr;
        CollisionEntity* collisionEntityA = nullptr;
        CollisionEntity* collisionEntityB = nullptr;

        auto collisionPhysicsItemA = contactInfo.modelA->GetPhysicsItem();
        auto collisionPhysicsItemB = contactInfo.modelB->GetPhysicsItem();

        for (const auto car : carList)
        {
            if (!carCollisionA)
            {
                if (car->chronoVehicle->GetChassisBody()->GetPhysicsItem() == collisionPhysicsItemA)
                {
                    carCollisionA = car;
                    continue;
                }
            }
            if (!carCollisionB)
            {
                if (car->chronoVehicle->GetChassisBody()->GetPhysicsItem() == collisionPhysicsItemB)
                {
                    carCollisionB = car;
                    continue;
                }
            }
            if (carCollisionA && carCollisionB)
                break;
        }

        if (!carCollisionA || !carCollisionB)
        {
            for (const auto collisionEntity : collisionEntityList)
            {
                if (!collisionEntityA && !carCollisionA)
                {
                    if (collisionEntity->chronoCollision->GetPhysicsItem() == collisionPhysicsItemA)
                    {
                        collisionEntityA = collisionEntity;

                        if (collisionEntityB || carCollisionB)
                            break;

                        continue;
                    }
                }
                if (!collisionEntityB && !carCollisionB)
                {
                    if (collisionEntity->chronoCollision->GetPhysicsItem() == collisionPhysicsItemB)
                    {
                        collisionEntityB = collisionEntity;

                        if (carCollisionA || collisionEntityA)
                            break;

                        continue;
                    }
                }
            }
        }

        if (collisionEntityA && collisionEntityB)
        {
            collisionEntityA->onCollision.broadcast(collisionEntityB);
            collisionEntityB->onCollision.broadcast(collisionEntityA);
        }
        else if (collisionEntityA && carCollisionB)
        {
            collisionEntityA->onCollisionWithCar.broadcast(carCollisionB);
            carCollisionB->onCollision.broadcast(collisionEntityA);
        }
        else if (collisionEntityB && carCollisionA)
        {
            collisionEntityB->onCollisionWithCar.broadcast(carCollisionA);
            carCollisionA->onCollision.broadcast(collisionEntityB);
        }
        else if (carCollisionA && carCollisionB)
        {
            carCollisionA->onCollisionWithCar.broadcast(carCollisionB);
            carCollisionB->onCollisionWithCar.broadcast(carCollisionA);
        }
    }

    void CarSimulationHandler::addCollisionToSimulation(CollisionEntity* collisionEntity)
    {
        if (simulationSystem)
        {
            simulationSystem->Add(collisionEntity->chronoCollision);
            collisionEntityList.push_back(collisionEntity);
        }
    }

    void CarSimulationHandler::removeCollisionFromSimulation(CollisionEntity* collisionEntity)
    {
        if (simulationSystem)
        {
            simulationSystem->Remove(collisionEntity->chronoCollision);
            collisionEntityList.erase(std::remove(collisionEntityList.begin(), collisionEntityList.end(), collisionEntity), collisionEntityList.end());
        }
    }

    Terrain* CarSimulationHandler::getActiveTerrain()
    {
        for (const auto& terrain : terrainList)
        {
            if (!terrain)
                continue;

            if (!terrain->isTerrainActive())
                continue;

            return terrain;
        }

        return nullptr;
    }

    void CarSimulationHandler::updateCollisionEntityTriggers()
    {
        for (auto collisionEntity : collisionEntityList)
        {
            if (collisionEntity->isTrigger() && collisionEntity->wasTriggered())
                collisionEntity->setCollisionActive(false);
        }
    }

    void CarSimulationHandler::addTerrain(Terrain* terrain)
    {
        terrainList.push_back(terrain);
    }

    void CarSimulationHandler::removeTerrain(Terrain* terrain)
    {
        terrainList.erase(std::remove(terrainList.begin(), terrainList.end(), terrain), terrainList.end());
    }

    void CarSimulationHandler::updateTerrains()
    {
        for (const auto& terrain : terrainList)
        {
            if (!terrain || !terrain->isTerrainActive())
                continue;

            terrain->updateChronoTerrain(getSimulationTime());
        }
    }

    void CarSimulationHandler::removeAllChronoCollisions()
    {
        simulationSystem->RemoveAllBodies();
    }

    CarSimulationHandler::~CarSimulationHandler()
    {
        if (auto actorContainerShared = actorContainer.lock())
        {
            actorContainerShared->onActorAdded.removeCallback(registerCarCallbackId);
            actorContainerShared->onActorDeleted.removeCallback(unregisterCarCallbackId);
        }
    }

    void CarSimulationHandler::CeCollisionCallback::OnAddContact(const chrono::collision::ChCollisionInfo& contactinfo, chrono::ChMaterialComposite* const material)
    {
        simulationHandler->onCollision(contactinfo);
    }
}