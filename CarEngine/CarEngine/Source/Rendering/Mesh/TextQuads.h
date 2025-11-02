#pragma once
#include "Rendering/Shader/TextShader.h"
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>

namespace CE
{
    class TextQuads {

    public:
        explicit TextQuads(std::string text, float fontSize, const std::filesystem::path& fontAssetPath, glm::vec4 color, bool bUseOnlyInitialTextWidth, bool bUseOnlyInitialTextHeight);

        TextQuads(const TextQuads &other) = delete;

        TextQuads(TextQuads &&other) noexcept
        {
            std::swap(this->text, other.text);
            std::swap(this->color, other.color);
            std::swap(this->textQuadDataList, other.textQuadDataList);

            std::swap(this->maxHeightTotal, other.maxHeightTotal);
            std::swap(this->maxHeightAboveBaseline, other.maxHeightAboveBaseline);
            std::swap(this->textWidth, other.textWidth);
            std::swap(this->textOffsetLeftX, other.textOffsetLeftX);
            std::swap(this->bUseOnlyInitialTextWidth, other.bUseOnlyInitialTextWidth);

            std::swap(this->textureAtlas, other.textureAtlas);
            std::swap(this->font, other.font);
            std::swap(this->vao, other.vao);
            std::swap(this->vbo, other.vbo);
        }

        int getMaxHeight() const { return maxHeightTotal; }
        int getMaxHeightAboveBaseline() const { return maxHeightAboveBaseline; }
        float getWidth() const { return textWidth; }
        int getOffsetLeftX() const { return textOffsetLeftX; }

        void updateText(const std::string& text);
        void draw(TextShader* shader);

        ~TextQuads();

        // ensures that rapidly changing texts will not flicker when their width changes (slightly)
        // this issue is only present when using UI anchors
        bool bUseOnlyInitialTextWidth = false;

        // ensures that rapidly changing texts will not flicker when their height changes (slightly)
        // this issue is only present when using UI anchors
        bool bUseOnlyInitialTextHeight = false;

    private:
        void setupRendering();
        void setupTextureAtlas();
        void calculateTextQuadData();

        std::string text;
        glm::vec4 color{};
        std::vector<std::vector<std::vector<float>>> textQuadDataList;

        int maxHeightTotal = 0;
        int maxHeightAboveBaseline = 0;
        float textWidth = 0.f;
        int textOffsetLeftX = 0;

        ftgl::texture_atlas_t* textureAtlas{};
        ftgl::texture_font_t* font{};

        unsigned int vao{};
        unsigned int vbo{};
    };
}