#pragma once
#include "UIImage.h"
#include "Input/Input.h"

namespace CE
{
    class UIButton : public UIImage {

    friend class UIButtonHandler;

    public:
        explicit UIButton(World* world,
                          Material* buttonDefaultMaterial,
                          Material* buttonHoveredMaterial,
                          Material* buttonPressedMaterial,
                          glm::vec2 scale = glm::vec2(100.f, 100.f),
                          glm::vec2 position = glm::vec2(0.f, 0.f),
                          float zRotation = 0.f,
                          bool bIsActive = true,
                          std::string name = "New UIButton");

        bool isHovered() const { return bIsHovered; }
        bool isPressed() const { return bIsPressed; }

        Event<UIButton> onButtonHoveredEnter;
        Event<UIButton> onButtonHoveredExit;
        Event<UIButton, int> onButtonPressed;
        Event<UIButton, int> onButtonReleased;

        const static UIComponentType componentType = UIComponentType::BUTTON;

    private:
        void buttonIsHovered();
        void buttonIsNotHovered();
        void buttonPressed(int button);
        void mouseButtonReleased(int button);

        Material* buttonDefaultMaterial;
        Material* buttonHoveredMaterial;
        Material* buttonPressedMaterial;

        bool bIsHovered = false;
        bool bIsPressed = false;

        int pressedMouseButton = -1;
    };
}