#pragma once
#include <memory>
#include "Core/Transformable.h"
#include "UIComponent.h"

namespace CE
{
    class UIPanel : public Transformable {

    friend class UIContainer;

    public:
        explicit UIPanel(World* world,
                         glm::vec2 anchorPositionInPercent = glm::vec2(0.f, 0.f),
                         float zRotation = 0.f,
                         glm::vec2 scale = glm::vec2(1.f, 1.f),
                         std::string name = "New UIPanel");

        virtual void start(World* world);
        virtual void update(World* world, float deltaTime);

        void setName(const std::string& name) { this->name = name; }
        std::string& getName() { return name; }

        void setKeepOnLevelChange(bool keepOnLevelChange) { bKeepOnLevelChange = keepOnLevelChange; }
        bool isKeptOnLevelChange() const { return bKeepOnLevelChange; }

        int getId() const { return id; }

        template <class T, typename... Args>
        std::weak_ptr<T> addComponent(Args&&... args)
        {
            const auto component = std::make_shared<T>(std::forward<Args>(args)...);

            if (unusedComponentIds.empty())
            {
                component->id = generateNewIdForComponent();
            }
            else
            {
                component->id = unusedComponentIds.at(0);
                unusedComponentIds.erase(unusedComponentIds.begin());
            }

            // ensure UIComponents that are added later are drawn above UIComponents that are added earlier
            component->position.z = (float) componentList.size() * 0.001f;

            this->componentList.emplace_back(component);

            return component;
        }

        template <class T>
        std::vector<T*> getComponentsRaw()
        {
            std::vector<T*> components;

            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->isComponentOfType(T::componentType))
                    components.emplace_back(static_cast<T*>(component.get()));
            }

            return components;
        }

        template <class T>
        std::vector<std::weak_ptr<T>> getComponentsWeak()
        {
            std::vector<std::weak_ptr<T>> components;

            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->isComponentOfType(T::componentType))
                    components.emplace_back(std::static_pointer_cast<T>(component));
            }

            return components;
        }

        template <class T>
        std::weak_ptr<T> getComponentById(int id)
        {
            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->id == id && component->isComponentOfType(T::componentType))
                    return std::static_pointer_cast<T>(component);
            }

            return {};
        }

        template <class T>
        std::weak_ptr<T> getComponentByName(const std::string& name)
        {
            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->name == name && component->isComponentOfType(T::componentType))
                    return std::static_pointer_cast<T>(component);
            }

            return {};
        }

        static void destroy(UIPanel* uiPanelRawPtr);
        static void destroy(const std::weak_ptr<UIPanel>& uiPanelWeakPtr);
        static void destroy(const std::vector<std::weak_ptr<UIPanel>>& uiPanelWeakPtrList);

        static void destroy(UIComponent* componentRawPtr);
        static void destroy(const std::weak_ptr<UIComponent>& componentWeakPtr);
        static void destroy(const std::vector<std::weak_ptr<UIComponent>>& componentWeakPtrList);

        ~UIPanel();

        glm::vec2 anchorPosition;

    private:
        void deleteFlaggedComponents();
        int generateNewIdForComponent() { return nextComponentId++; }

        std::string name;
        bool bDelete = false;
        bool bKeepOnLevelChange = false;
        int id = -1;
        int nextComponentId = 0;
        std::vector<int> unusedComponentIds;

        std::vector<std::shared_ptr<UIComponent>> componentList;
    };
}