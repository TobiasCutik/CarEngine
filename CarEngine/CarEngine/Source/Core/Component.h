#pragma once

#include <vector>
#include <string>

namespace CE
{
    class Component {

    protected:
        explicit Component(bool bIsActive, std::string name);

    public:
        void setName(const std::string& name) { this->name = name; }
        std::string& getName() { return name; }

        bool isActive() const { return bIsActive; }
        void setActive(bool bActive) { bIsActive = bActive; }

        int getId() const { return id; }

        virtual ~Component() {};

    protected:
        std::string name;
        bool bIsActive;
        int id = -1;
        bool bDelete = false;
    };
}