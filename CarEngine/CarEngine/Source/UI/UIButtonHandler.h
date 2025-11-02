#pragma once
#include "UIPanel.h"
#include "UIButton.h"

namespace CE
{
    class UIButtonHandler {

    public:
        explicit UIButtonHandler(const std::weak_ptr<Input>& input);

        UIButtonHandler(const UIButtonHandler& other) = delete;

        UIButtonHandler(UIButtonHandler&& other) noexcept {
            std::swap(this->input, other.input);
            std::swap(this->uiButtonGlobalTransforms, other.uiButtonGlobalTransforms);
            std::swap(this->mouseMovedCallbackId, other.mouseMovedCallbackId);
            std::swap(this->mouseButtonPressedCallbackId, other.mouseButtonPressedCallbackId);
            std::swap(this->mouseButtonReleasedCallbackId, other.mouseButtonReleasedCallbackId);
        }

        ~UIButtonHandler();

        void calculateUiButtonGlobalTransforms(const std::vector<std::shared_ptr<UIPanel>>& uiPanels, glm::ivec2 windowSize);

    private:
        struct UIButtonTransform
        {
            glm::vec2 position = glm::vec2(1.f);
            float zRotation = 0.f;
            glm::vec2 scale = glm::vec2(1.f);
        };

        void mouseMoved(glm::vec2 cursorPosition);
        void mouseButtonPressed(int button, glm::vec2 cursorPosition);
        void mouseButtonReleased(int button, glm::vec2 cursorPosition);

        static bool isCursorOverlappingWithUIButton(glm::vec2 cursorPosition, UIButtonTransform transform);

        std::weak_ptr<Input> input;

        std::map<UIButton*, UIButtonTransform> uiButtonGlobalTransforms;

        std::list<std::function<void(glm::vec2)>>::iterator mouseMovedCallbackId;
        std::list<std::function<void(int, glm::vec2)>>::iterator mouseButtonPressedCallbackId;
        std::list<std::function<void(int, glm::vec2)>>::iterator mouseButtonReleasedCallbackId;
    };
}