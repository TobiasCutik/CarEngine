#include "Component.h"
#include <utility>

namespace CE
{
    Component::Component(bool bIsActive, std::string name) : bIsActive(bIsActive), name(std::move(name))
    {
    }
}