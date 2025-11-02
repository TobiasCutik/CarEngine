#include <memory>
#include "Materials.h"

namespace CE::Materials
{
    ColorMaterial* grey()
    {
        static const auto grey = std::make_unique<ColorMaterial>(glm::vec4 {0.7f, 0.7f, 0.7f, 1.f});
        return grey.get();
    }

    ColorMaterial* red()
    {
        static const auto red = std::make_unique<ColorMaterial>(glm::vec4 {1.f, 0.f, 0.f, 1.f});
        return red.get();
    }

    ColorMaterial* green()
    {
        static const auto green = std::make_unique<ColorMaterial>(glm::vec4 {0.f, 1.f, 0.f, 1.f});
        return green.get();
    }

    ColorMaterial* blue()
    {
        static const auto blue = std::make_unique<ColorMaterial>(glm::vec4 {0.f, 0.f, 1.f, 1.f});
        return blue.get();
    }

    ColorMaterial* pink()
    {
        static const auto pink = std::make_unique<ColorMaterial>(glm::vec4 {1.f, 0.2f, 0.9f, 1.f});
        return pink.get();
    }

    ColorMaterial* orange()
    {
        static const auto orange = std::make_unique<ColorMaterial>(glm::vec4 {1.f, 0.4f, 0.f, 1.f});
        return orange.get();
    }

    ColorMaterial* yellow()
    {
        static const auto yellow = std::make_unique<ColorMaterial>(glm::vec4 {1.f, 1.f, 0.f, 1.f});
        return yellow.get();
    }

    ColorMaterial* white()
    {
        static const auto white = std::make_unique<ColorMaterial>(glm::vec4 {1.f, 1.f, 1.f, 1.f});
        return white.get();
    }

    ColorMaterial* black()
    {
        static const auto black = std::make_unique<ColorMaterial>(glm::vec4 {0.f, 0.f, 0.f, 1.f});
        return black.get();
    }
}