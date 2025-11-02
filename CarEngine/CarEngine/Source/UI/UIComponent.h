#pragma once
#include "Core/Component.h"
#include "Core/Transformable.h"

namespace CE
{
    enum class UIComponentType
    {
        UI_COMPONENT,
        BUTTON,
        IMAGE,
        TEXT_FIELD
    };

    class UIComponent : public Component, public Transformable {

    friend class UIPanel;

    protected:
        explicit UIComponent(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                             glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                             glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                             bool bIsActive = true,
                             std::string name = "New UIComponent");

        void setComponentType(UIComponentType type) { componentTypeList.emplace_back(type); }

    public:
        bool isComponentOfType(UIComponentType type);

        const static UIComponentType componentType = UIComponentType::UI_COMPONENT;

    private:
        std::vector<UIComponentType> componentTypeList;
    };
}