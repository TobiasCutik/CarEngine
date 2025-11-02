#include "UITextField.h"

namespace CE
{
    UITextField::UITextField(const std::string& text, float fontSize, glm::vec4 color, glm::vec2 position, const std::filesystem::path& fontAssetPath, float zRotation, glm::vec2 scale,
                             bool bUseOnlyInitialTextWidth, bool bUseOnlyInitialTextHeight, bool bIsActive, std::string name)
        : UIComponent(glm::vec3 {position.x, -position.y, 0.f}, glm::vec3 {0.f, 0.f, zRotation}, glm::vec3 {scale.x, scale.y, 0.f},
                      bIsActive, std::move(name)),
        textQuads(text, fontSize, fontAssetPath, color, bUseOnlyInitialTextWidth, bUseOnlyInitialTextHeight)
    {
        setComponentType(UIComponentType::TEXT_FIELD);
    }
}