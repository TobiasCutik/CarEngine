#pragma once
#include <string>

namespace CE
{
    class Material {

    public:
        explicit Material(std::string name);
        virtual ~Material() = default;

        virtual bool hasTexture() const = 0;

        std::string name;
    };
}