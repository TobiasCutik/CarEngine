#include "Renderer.h"
#include <glm/gtx/transform.hpp>
#include "../Utils/Math.h"
#include "../ActorComponents/LightEntity.h"
#include "../ActorComponents/SkyboxEntity.h"
#include "UI/UIImage.h"
#include "UI/UITextField.h"

namespace CE
{
    Renderer::Renderer(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset,
                       const std::filesystem::path& skyboxVertexShaderAsset, const std::filesystem::path& skyboxFragmentShaderAsset,
                       const std::filesystem::path& depthVertexShaderAsset, const std::filesystem::path& depthFragmentShaderAsset,
                       const std::filesystem::path& uiVertexShaderAsset, const std::filesystem::path& uiFragmentShaderAsset,
                       const std::filesystem::path& textVertexShaderAsset, const std::filesystem::path& textFragmentShaderAsset,
                       const std::filesystem::path& tilingVertexShaderAsset, const std::filesystem::path& tilingFragmentShaderAsset, int depthMapWidth, int depthMapHeight, float shadowAreaSizeModifier,
                       bool bUseBackfaceCulling)
                       : depthMapWidth(depthMapWidth), depthMapHeight(depthMapHeight), shadowAreaSizeModifier(shadowAreaSizeModifier), windowWidth(0), windowHeight(0)
    {
        cameraShader = std::make_unique<CameraShader>(vertexShaderAsset, fragmentShaderAsset);
        skyboxShader = std::make_unique<SkyboxShader>(skyboxVertexShaderAsset, skyboxFragmentShaderAsset);
        depthShader = std::make_unique<DepthShader>(depthVertexShaderAsset, depthFragmentShaderAsset);
        uiShader = std::make_unique<UIShader>(uiVertexShaderAsset, uiFragmentShaderAsset);
        textShader = std::make_unique<TextShader>(textVertexShaderAsset, textFragmentShaderAsset);
        tilingShader = std::make_unique<TilingShader>(tilingVertexShaderAsset, tilingFragmentShaderAsset);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        setupDepthMap();

        setBackfaceCulling(bUseBackfaceCulling);
    }

    void Renderer::draw(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all entities of the current actor
            auto entities = actor->getComponentsRaw<Entity>();
            for (auto& entity : entities)
            {
                if (!entity->isActive())
                    continue;

                // calculate the global transform of the entity
                glm::mat4 entityGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), entity->getTransformationMatrix());

                if (entity->bUseTextureTiling)
                {
                    // render entity
                    tilingShader->setGlobalEntityMatrix(entityGlobalMatrix);
                    entity->getMesh()->draw(tilingShader.get());
                }
                else
                {
                    // render entity
                    cameraShader->setGlobalEntityMatrix(entityGlobalMatrix);
                    entity->getMesh()->draw(cameraShader.get());
                }
            }
        }
    }

    void Renderer::renderDepthMap(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        glViewport(0, 0, (int) depthMapWidth, (int) depthMapHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all entities of the current actor
            auto entities = actor->getComponentsRaw<Entity>();
            for (auto& entity : entities)
            {
                if (!entity->isActive())
                    continue;

                // calculate the global transform of the entity
                glm::mat4 entityGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), entity->getTransformationMatrix());

                // render entity
                depthShader->setGlobalEntityMatrix(entityGlobalMatrix);
                entity->getMesh()->draw(depthShader.get());
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::updateLight(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        // find active light
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all lightEntities of the current actor
            auto lightEntities = actor->getComponentsRaw<LightEntity>();
            for (auto& light : lightEntities)
            {
                if (!light->isLightActive())
                    continue;

                // calculate the global transform of the lightEntity
                glm::mat4 lightGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), light->getTransformationMatrix());

                shaderLightData.lightProjectionMatrix = glm::ortho(-10.f * shadowAreaSizeModifier, 10.f * shadowAreaSizeModifier,
                                                        -10.f * shadowAreaSizeModifier, 10.f * shadowAreaSizeModifier,0.1f, 100.f);

                shaderLightData.lightViewMatrix = glm::lookAt(glm::vec3(lightGlobalMatrix[3]), light->lookAtPosition, glm::vec3(0.f, 1.f, 0.f));

                shaderLightData.lightPosition = lightGlobalMatrix[3];
                shaderLightData.lightColor = light->color;
                shaderLightData.ambientLightStrength = light->ambientLightStrength;
                shaderLightData.diffuseLightStrength = light->diffuseLightStrength;
                shaderLightData.specularLightStrength = light->specularLightStrength;
                shaderLightData.shadowStrength = light->shadowStrength;

                return;
            }
        }
    }

    void Renderer::drawSkybox(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        // find active skybox
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all skyboxEntities of the current actor
            auto skyboxEntities = actor->getComponentsRaw<SkyboxEntity>();
            for (auto& skybox : skyboxEntities)
            {
                if (!skybox->isSkyboxActive())
                    continue;

                // calculate the global transform of the skyboxEntity
                glm::mat4 skyboxGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), skybox->getTransformationMatrix());
                // remove translations
                skyboxGlobalMatrix = glm::mat4(glm::mat3(skyboxGlobalMatrix));

                setBackfaceCulling(false);
                glDepthFunc(GL_LEQUAL);

                // render skybox
                skyboxShader->setGlobalEntityMatrix(skyboxGlobalMatrix);
                skybox->getMesh()->draw(skyboxShader.get());

                glDepthFunc(GL_LESS);
                setBackfaceCulling(true);

                return;
            }
        }
    }

    void Renderer::drawUI(const std::vector<std::shared_ptr<UIPanel>>& uiPanels)
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        setBackfaceCulling(false);

        for (auto& uiPanel : uiPanels)
        {
            if (!uiPanel)
                continue;

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // loop over all uiImages of the current uiPanel
            auto uiImages = uiPanel->getComponentsRaw<UIImage>();
            for (auto& uiImage : uiImages)
            {
                if (!uiImage->isActive())
                    continue;

                // apply anchors to uiPanel position (depends also on the current uiImages's scale)
                uiPanel->position.x = (((float) windowWidth - uiImage->scale.x) * (uiPanel->anchorPosition.x / 100)) + uiImage->scale.x / 2;
                // use z scale as uiImage is rotated
                uiPanel->position.y = (((float) windowHeight - uiImage->scale.z) * (uiPanel->anchorPosition.y / 100)) + uiImage->scale.z / 2;

                // calculate the global transform of the uiComponent
                glm::mat4 uiComponentGlobalMatrix = Math::calculateGlobalTransformationMatrix(uiPanel->getTransformationMatrix(), uiImage->getTransformationMatrix());

                // render uiComponent
                uiShader->setGlobalEntityMatrix(uiComponentGlobalMatrix);
                uiImage->getMesh()->draw(uiShader.get());
            }

            // loop over all textFields of the current uiPanel
            auto textFields = uiPanel->getComponentsRaw<UITextField>();
            for (auto& textField : textFields)
            {
                if (!textField->isActive())
                    continue;

                // apply anchors to uiPanel position (depends also on the current textField)
                // cast to int to ensure correctly rendered texts without visible textQuad edges
                uiPanel->position.x = (int) ((((float) windowWidth - textField->getTextQuads()->getWidth()) * (uiPanel->anchorPosition.x / 100)) - (float) textField->getTextQuads()->getOffsetLeftX());
                // cast to int to ensure correctly rendered texts without visible textQuad edges
                uiPanel->position.y = (int) ((((float) (windowHeight - textField->getTextQuads()->getMaxHeight())) * (uiPanel->anchorPosition.y / 100)) + (float) textField->getTextQuads()->getMaxHeightAboveBaseline());

                // calculate the global transform of the uiComponent
                glm::mat4 uiComponentGlobalMatrix = Math::calculateGlobalTransformationMatrix(uiPanel->getTransformationMatrix(), textField->getTransformationMatrix());

                // render uiComponent
                textShader->setGlobalEntityMatrix(uiComponentGlobalMatrix);
                textField->getTextQuads()->draw(textShader.get());
            }

            glDisable(GL_BLEND);
        }

        setBackfaceCulling(true);
    }

    void Renderer::passDataToShaders()
    {
        cameraShader->setShaderCameraData(&shaderCameraData);
        cameraShader->setShaderLightData(&shaderLightData);

        skyboxShader->setShaderCameraData(&shaderCameraData);

        depthShader->setShaderLightData(&shaderLightData);

        uiShader->setUiProjectionMatrix(uiProjectionMatrix);

        textShader->setUiProjectionMatrix(uiProjectionMatrix);

        tilingShader->setShaderCameraData(&shaderCameraData);
        tilingShader->setShaderLightData(&shaderLightData);
    }

    void Renderer::updateWindowSize(int width, int height)
    {
        this->windowWidth = width;
        this->windowHeight = height;
    }

    void Renderer::updateUiProjectionMatrix()
    {
        uiProjectionMatrix = glm::ortho(0.f, (float) windowWidth, (float) windowHeight, 0.f, -1.f, 1.f);
    }

    void Renderer::updateCamera(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        // find active camera; search in all actors
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all cameras of the current actor
            auto cameras = actor->getComponentsRaw<Camera>();
            for (auto& camera : cameras)
            {
                if (!camera->isActive())
                    continue;

                // calculate the global transform of the camera
                glm::mat4 cameraGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), camera->getTransformationMatrix());

                shaderCameraData.cameraPosition = cameraGlobalMatrix[3];

                shaderCameraData.viewMatrix = glm::inverse(cameraGlobalMatrix);

                if (windowWidth == 0 || windowHeight == 0)
                    return;

                shaderCameraData.projectionMatrix = glm::perspective(glm::radians(camera->getFovY()), (float) windowWidth / (float) windowHeight, 0.1f, camera->getZFarPlane());

                return;
            }
        }
    }

    void Renderer::setupDepthMap()
    {
        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (int) depthMapWidth, (int) depthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.f, 1.f, 1.f, 1.f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::setBackfaceCulling(bool bActive)
    {
        if (bActive == bUseBackfaceCulling)
            return;

        if (bActive)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        bUseBackfaceCulling = bActive;
    }
}