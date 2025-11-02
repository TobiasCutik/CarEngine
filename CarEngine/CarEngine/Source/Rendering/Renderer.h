#pragma once

#include "Shader/CameraShader.h"
#include "Shader/SkyboxShader.h"
#include "Shader/DepthShader.h"
#include "../ActorComponents/Camera.h"
#include "../Core/Actor.h"
#include "UI/UIPanel.h"
#include "Rendering/Shader/UIShader.h"
#include "Rendering/Shader/TextShader.h"
#include "Rendering/Shader/TilingShader.h"

namespace CE
{
    class Renderer {

    public:
        Renderer(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset,
                 const std::filesystem::path& skyboxVertexShaderAsset, const std::filesystem::path& skyboxFragmentShaderAsset,
                 const std::filesystem::path& depthVertexShaderAsset, const std::filesystem::path& depthFragmentShaderAsset,
                 const std::filesystem::path& uiVertexShaderAsset, const std::filesystem::path& uiFragmentShaderAsset,
                 const std::filesystem::path& textVertexShaderAsset, const std::filesystem::path& textFragmentShaderAsset,
                 const std::filesystem::path& tilingVertexShaderAsset, const std::filesystem::path& tilingFragmentShaderAsset,
                 int depthMapWidth = 6144, int depthMapHeight = 6144, float shadowAreaSizeModifier = 6.f,
                 bool bUseBackfaceCulling = true);
        void draw(const std::vector<std::shared_ptr<Actor>>& actors);
        void renderDepthMap(const std::vector<std::shared_ptr<Actor>>& actors);
        void updateLight(const std::vector<std::shared_ptr<Actor>>& actors);
        void updateCamera(const std::vector<std::shared_ptr<Actor>>& actors);
        void drawSkybox(const std::vector<std::shared_ptr<Actor>>& actors);
        void drawUI(const std::vector<std::shared_ptr<UIPanel>>& uiPanels);
        void passDataToShaders();
        void updateWindowSize(int width, int height);
        void updateUiProjectionMatrix();
        Shader::VertexAttributes& getShaderVertexAttributes() { return cameraShader->vertexAttributes; }
        glm::ivec2 getWindowSize() const { return {windowWidth, windowHeight}; }

        void setBackfaceCulling(bool bActive);
        bool isBackfaceCullingActive() const { return bUseBackfaceCulling; }

    private:
        void setupDepthMap();

        std::unique_ptr<CameraShader> cameraShader;
        std::unique_ptr<SkyboxShader> skyboxShader;
        std::unique_ptr<DepthShader> depthShader;
        std::unique_ptr<UIShader> uiShader;
        std::unique_ptr<TextShader> textShader;
        std::unique_ptr<TilingShader> tilingShader;

        ShaderCameraData shaderCameraData;
        ShaderLightData shaderLightData;

        glm::mat4 uiProjectionMatrix = glm::mat4(1.f);

        unsigned int windowWidth = 0;
        unsigned int windowHeight = 0;

        unsigned int depthMapFBO = 0;
        unsigned int depthMap = 0;
        unsigned int depthMapWidth = 0;
        unsigned int depthMapHeight = 0;
        float shadowAreaSizeModifier = 0.f;

        bool bUseBackfaceCulling = false;
    };
}