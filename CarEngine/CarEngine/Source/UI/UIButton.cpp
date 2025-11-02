#include "UIButton.h"
#include <utility>

namespace CE
{
    UIButton::UIButton(World* world, Material* buttonDefaultMaterial, Material* buttonHoveredMaterial, Material* buttonPressedMaterial, glm::vec2 scale, glm::vec2 position, float zRotation, bool bIsActive, std::string name)
        : UIImage(world, buttonDefaultMaterial, scale, position, zRotation, bIsActive, std::move(name)),
        buttonDefaultMaterial(buttonDefaultMaterial), buttonHoveredMaterial(buttonHoveredMaterial), buttonPressedMaterial(buttonPressedMaterial)
    {
        setComponentType(UIComponentType::BUTTON);
    }

    void UIButton::buttonIsHovered()
    {
        if (!bIsPressed)
            uiImageMesh.setMaterial(buttonHoveredMaterial);

        if (!bIsHovered)
        {
            onButtonHoveredEnter.broadcast();
            bIsHovered = true;
        }
    }

    void UIButton::buttonIsNotHovered()
    {
        if (bIsHovered)
        {
            uiImageMesh.setMaterial(buttonDefaultMaterial);
            onButtonHoveredExit.broadcast();
            bIsHovered = false;
            bIsPressed = false;
        }
    }

    void UIButton::buttonPressed(int button)
    {
        if (bIsPressed)
            return;

        uiImageMesh.setMaterial(buttonPressedMaterial);
        onButtonPressed.broadcast(button);
        bIsPressed = true;
        pressedMouseButton = button;
    }

    void UIButton::mouseButtonReleased(int button)
    {
        if (bIsPressed && button == pressedMouseButton)
        {
            if (bIsHovered)
                uiImageMesh.setMaterial(buttonHoveredMaterial);
            else
                uiImageMesh.setMaterial(buttonDefaultMaterial);

            onButtonReleased.broadcast(button);
            bIsPressed = false;
        }
    }
}