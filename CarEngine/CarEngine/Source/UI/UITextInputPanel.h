#pragma once
#include "UIButton.h"
#include "UIPanel.h"
#include "UITextField.h"

namespace CE
{
    class UITextInputPanel : public UIPanel {

    public:
        explicit UITextInputPanel(World* world,
                                  Material* panelDefaultMaterial,
                                  Material* panelSelectedMaterial,
                                  glm::vec2 anchorPositionInPercent = glm::vec2(50.f, -50.f),
                                  glm::vec2 textOffset = glm::vec2(0.f, 0.f),
                                  glm::vec2 scale = glm::vec2(400.f, 100.f),
                                  int characterLimit = 10,
                                  float fontSize = 35.f,
                                  glm::vec4 textColor = glm::vec4(1.f, 1.f, 1.f, 1.f),
                                  const std::filesystem::path& fontAssetPath = Loader::resolve("Assets/Fonts/Vera/Vera.ttf"),
                                  float zRotation = 0.f,
                                  bool bIsActive = true,
                                  std::string name = "New UITextInputPanel");

        UITextInputPanel(const UITextInputPanel& other) = delete;

        UITextInputPanel(World* world, UITextInputPanel&& other) noexcept: UIPanel(world, other.anchorPosition, other.rotation.z, other.scale, other.getName()) {
            std::swap(this->onPanelSelected, other.onPanelSelected);
            std::swap(this->onPanelDeselected, other.onPanelDeselected);
            std::swap(this->onCharacterLimitReached, other.onCharacterLimitReached);
            std::swap(this->bIsSelected, other.bIsSelected);
            std::swap(this->textInput, other.textInput);
            std::swap(this->characterLimit, other.characterLimit);
            std::swap(this->panelDefaultMaterial, other.panelDefaultMaterial);
            std::swap(this->panelSelectedMaterial, other.panelSelectedMaterial);
            std::swap(this->uiButton, other.uiButton);
            std::swap(this->uiTextField, other.uiTextField);
            std::swap(this->input, other.input);
            std::swap(this->keyPressedCallbackId, other.keyPressedCallbackId);
        }

        ~UITextInputPanel();

        void update(World* world, float deltaTime) override;

        bool isActive() const;
        void setActive(bool bActive);

        bool isSelected() const { return bIsSelected; }

        void setTextInput(const std::string& Text);
        std::string getTextInput() const { return textInput; }
        void clearTextInput() { textInput = ""; }

        Event<UITextInputPanel, int> onPanelSelected;
        Event<UITextInputPanel, int> onPanelDeselected;
        Event<UITextInputPanel, int> onCharacterLimitReached;

    private:
        void panelReleased(int button);
        void keyPressed(int key);

        bool bIsSelected{};

        std::string textInput;
        int characterLimit{};

        Material* panelDefaultMaterial = nullptr;
        Material* panelSelectedMaterial = nullptr;

        std::weak_ptr<UIButton> uiButton;
        std::weak_ptr<UITextField> uiTextField;

        std::weak_ptr<Input> input;
        std::list<std::function<void(int)>>::iterator keyPressedCallbackId;
    };
}