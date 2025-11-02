#include "UITextInputPanel.h"
#include "Core/World.h"
#include <utility>
#include "Input/KeyCodes.h"

namespace CE
{
    UITextInputPanel::UITextInputPanel(World* world, Material* panelDefaultMaterial, Material* panelSelectedMaterial, glm::vec2 anchorPositionInPercent, glm::vec2 textOffset,
                                       glm::vec2 scale, int characterLimit, float fontSize, glm::vec4 textColor, const std::filesystem::path& fontAssetPath, float zRotation, bool bIsActive, std::string name)
        : UIPanel(world, anchorPositionInPercent, zRotation, glm::vec2 (1.f, 1.f), std::move(name)),
        characterLimit(characterLimit), panelDefaultMaterial(panelDefaultMaterial), panelSelectedMaterial(panelSelectedMaterial)
    {
        uiButton = UIPanel::addComponent<UIButton>(world, panelDefaultMaterial, panelSelectedMaterial, panelSelectedMaterial, scale, glm::vec2(0.f, 0.f), 0.f, bIsActive);
        uiTextField = UIPanel::addComponent<UITextField>("", fontSize, textColor, textOffset, fontAssetPath, 0.f, glm::vec2(1.f, 1.f), false, true, bIsActive);

        uiButton.lock()->onButtonReleased.addCallback(BIND_FUNC(UITextInputPanel::panelReleased));
        keyPressedCallbackId = world->getInput()->getKeyPressedEvent().addCallback(BIND_FUNC(UITextInputPanel::keyPressed));

        input = world->getInput();
        bIsSelected = false;
    }

    void UITextInputPanel::update(World* world, float deltaTime)
    {
        auto uiButtonShared = uiButton.lock();
        if (!uiButtonShared)
            return;

        if (bIsSelected)
            uiButtonShared->getMesh()->setMaterial(panelSelectedMaterial);
        else
            uiButtonShared->getMesh()->setMaterial(panelDefaultMaterial);
    }

    void UITextInputPanel::panelReleased(int button)
    {
        bIsSelected = !bIsSelected;

        if (bIsSelected)
            onPanelSelected.broadcast(button);
        else
            onPanelDeselected.broadcast(button);
    }

    void UITextInputPanel::keyPressed(int key)
    {
        if (!bIsSelected)
            return;

        if (key == CE_KEY_ESCAPE || key == CE_KEY_ENTER)
        {
            bIsSelected = false;
            onPanelDeselected.broadcast(key);
            return;
        }

        if ((key != CE_KEY_BACKSPACE && key != CE_KEY_DELETE) && textInput.length() == characterLimit)
        {
            onCharacterLimitReached.broadcast(characterLimit);
            return;
        }

        if ((key == CE_KEY_BACKSPACE || key == CE_KEY_DELETE) && textInput.empty())
            return;

        auto uiTextFieldShared = uiTextField.lock();
        if (!uiTextFieldShared)
            return;

        if ((key == CE_KEY_BACKSPACE || key == CE_KEY_DELETE) && !textInput.empty())
        {
            textInput.pop_back();
            uiTextFieldShared->getTextQuads()->updateText(textInput);
            return;
        }

        // force QWERTZ layout
        if (key == CE_KEY_Y)
            key = CE_KEY_Z;
        else if (key == CE_KEY_Z)
            key = CE_KEY_Y;

        std::string characterString(1, char(key));
        textInput.append(characterString);
        uiTextFieldShared->getTextQuads()->updateText(textInput);
    }

    bool UITextInputPanel::isActive() const
    {
        if (auto uiTextFieldShared = uiTextField.lock())
        {
            if (uiTextFieldShared->isActive())
                return true;
        }

        if (auto uiButtonShared = uiButton.lock())
        {
            if (uiButtonShared->isActive())
                return true;
        }

        return false;
    }

    void UITextInputPanel::setActive(bool bActive)
    {
        if (auto uiTextFieldShared = uiTextField.lock())
            uiTextFieldShared->setActive(bActive);

        if (auto uiButtonShared = uiButton.lock())
            uiButtonShared->setActive(bActive);
    }

    UITextInputPanel::~UITextInputPanel()
    {
        if (auto inputShared = input.lock())
            inputShared->getKeyPressedEvent().removeCallback(keyPressedCallbackId);
    }
}
