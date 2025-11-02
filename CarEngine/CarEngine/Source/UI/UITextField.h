#pragma once
#include "UIComponent.h"
#include "Rendering/Mesh/TextQuads.h"
#include "Utils/Loader.h"

namespace CE
{
    class UITextField : public UIComponent {

    public:
        explicit UITextField(const std::string& text,
                             float fontSize = 15.f,
                             glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f),
                             glm::vec2 position = glm::vec2(0.f, 0.f),
                             const std::filesystem::path& fontAssetPath = Loader::resolve("Assets/Fonts/Vera/Vera.ttf"),
                             float zRotation = 0.f,
                             glm::vec2 scale = glm::vec2(1.f, 1.f),
                             bool bUseOnlyInitialTextWidth = false,
                             bool bUseOnlyInitialTextHeight = false,
                             bool bIsActive = true,
                             std::string name = "New UITextField");

        TextQuads* getTextQuads() { return &textQuads; }

        const static UIComponentType componentType = UIComponentType::TEXT_FIELD;

    private:
        TextQuads textQuads;
    };
}