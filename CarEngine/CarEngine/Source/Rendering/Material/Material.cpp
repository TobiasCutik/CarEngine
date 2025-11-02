#include "Material.h"
#include <utility>

namespace CE
{
    Material::Material(std::string name) : name(std::move(name))
    {
    }
}