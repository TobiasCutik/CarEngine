#include "UIButtonHandler.h"
#include "UIButton.h"
#include "Utils/Math.h"

namespace CE
{
    UIButtonHandler::UIButtonHandler(const std::weak_ptr<Input>& input) : input(input)
    {
        mouseMovedCallbackId = input.lock()->getMouseMovedEvent().addCallback(BIND_FUNC(UIButtonHandler::mouseMoved));
        mouseButtonPressedCallbackId = input.lock()->getMouseButtonPressedEvent().addCallback(BIND_FUNC(UIButtonHandler::mouseButtonPressed));
        mouseButtonReleasedCallbackId = input.lock()->getMouseButtonReleasedEvent().addCallback(BIND_FUNC(UIButtonHandler::mouseButtonReleased));
    }

    void UIButtonHandler::mouseMoved(glm::vec2 cursorPosition)
    {
        for (const auto& [uiButton, transform] : uiButtonGlobalTransforms)
        {
            if (!uiButton->isActive())
                continue;

            if (isCursorOverlappingWithUIButton(cursorPosition, transform))
                uiButton->buttonIsHovered();
            else
                uiButton->buttonIsNotHovered();
        }
    }

    void UIButtonHandler::mouseButtonPressed(int button, glm::vec2 cursorPosition)
    {
        for (const auto& [uiButton, transform] : uiButtonGlobalTransforms)
        {
            if (!uiButton->isActive())
                continue;

            if (isCursorOverlappingWithUIButton(cursorPosition, transform))
                uiButton->buttonPressed(button);
        }
    }

    void UIButtonHandler::mouseButtonReleased(int button, glm::vec2 cursorPosition)
    {
        for (const auto& [uiButton, transform] : uiButtonGlobalTransforms)
        {
            if (!uiButton->isActive())
                continue;

            uiButton->mouseButtonReleased(button);
        }
    }

    bool UIButtonHandler::isCursorOverlappingWithUIButton(glm::vec2 cursorPosition, UIButtonTransform transform)
    {
        // rotate cursorPosition to account for rotated UIButtons
        glm::vec2 rotatedCursorPosition = Math::rotatePointAroundPivotPoint(cursorPosition, transform.position, transform.zRotation);

        float buttonX1 = transform.position.x - (0.5f * transform.scale.x);
        float buttonX2 = transform.position.x + (0.5f * transform.scale.x);
        if (rotatedCursorPosition.x < buttonX1 || rotatedCursorPosition.x > buttonX2)
            return false; // rotatedCursorPosition.x is not within UIButton's x range

        float buttonY1 = transform.position.y - (0.5f * transform.scale.y);
        float buttonY2 = transform.position.y + (0.5f * transform.scale.y);
        if (rotatedCursorPosition.y < buttonY1 || rotatedCursorPosition.y > buttonY2)
            return false; // rotatedCursorPosition.y is not within UIButton's y range

        return true;
    }

    void UIButtonHandler::calculateUiButtonGlobalTransforms(const std::vector<std::shared_ptr<UIPanel>>& uiPanels, glm::ivec2 windowSize)
    {
        for (auto& uiPanel : uiPanels)
        {
            if (!uiPanel)
                continue;

            // loop over all uiButtons of the current uiPanel
            auto uiButtons = uiPanel->getComponentsRaw<UIButton>();
            for (auto& uiButton : uiButtons)
            {
                if (uiButton->bDelete)
                {
                    uiButtonGlobalTransforms.erase(uiButton);
                    continue;
                }

                if (!uiButton->isActive())
                    continue;

                // apply anchors to uiPanel position (depends also on the current uiButton's scale)
                uiPanel->position.x = (((float) windowSize.x - uiButton->scale.x) * (uiPanel->anchorPosition.x / 100)) + uiButton->scale.x / 2;
                // use z scale as uiButton is rotated
                uiPanel->position.y = (((float) windowSize.y - uiButton->scale.z) * (uiPanel->anchorPosition.y / 100)) + uiButton->scale.z / 2;

                // calculate the global transform of the uiButton
                glm::mat4 uiButtonGlobalMatrix = Math::calculateGlobalTransformationMatrix(uiPanel->getTransformationMatrix(), uiButton->getTransformationMatrix());

                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(uiButtonGlobalMatrix, position, rotation, scale);

                UIButtonTransform uiButtonTransform;
                uiButtonTransform.position = position;
                uiButtonTransform.zRotation = glm::degrees(glm::eulerAngles(rotation)).z;
                uiButtonTransform.scale = {scale.x, scale.z};

                uiButtonGlobalTransforms.insert_or_assign(uiButton, uiButtonTransform);
            }
        }
    }

    UIButtonHandler::~UIButtonHandler()
    {
        if (auto inputShared = input.lock())
        {
            inputShared->getMouseMovedEvent().removeCallback(mouseMovedCallbackId);
            inputShared->getMouseButtonPressedEvent().removeCallback(mouseButtonPressedCallbackId);
            inputShared->getMouseButtonReleasedEvent().removeCallback(mouseButtonReleasedCallbackId);
        }
    }
}