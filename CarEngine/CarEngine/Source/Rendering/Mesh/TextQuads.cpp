#include "TextQuads.h"
#include <utility>

namespace CE
{
    TextQuads::TextQuads(std::string text, float fontSize, const std::filesystem::path& fontAssetPath, glm::vec4 color, bool bUseOnlyInitialTextWidth, bool bUseOnlyInitialTextHeight)
        : text(std::move(text)), color(color), bUseOnlyInitialTextWidth(bUseOnlyInitialTextWidth), bUseOnlyInitialTextHeight(bUseOnlyInitialTextHeight)
    {
        textureAtlas = ftgl::texture_atlas_new(1024, 1024, 1);
        font = ftgl::texture_font_new_from_file(textureAtlas, fontSize, fontAssetPath.string().c_str());

        setupRendering();
        setupTextureAtlas();

        updateText(this->text);
    }

    void TextQuads::setupRendering()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void TextQuads::setupTextureAtlas()
    {
        glGenTextures(1, &textureAtlas->id);
        glBindTexture(GL_TEXTURE_2D, textureAtlas->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void TextQuads::updateText(const std::string& text)
    {
        this->text = text;

        // update glyphs
        ftgl::texture_font_load_glyphs(font, text.data());

        // update textureAtlas
        glBindTexture(GL_TEXTURE_2D, textureAtlas->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int) textureAtlas->width, (int) textureAtlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, textureAtlas->data);

        calculateTextQuadData();
    }

    void TextQuads::calculateTextQuadData()
    {
        textQuadDataList.clear();

        float xPosOfCurrCharacter = 0.f;
        int maxHeightBelowBaseline = 0;
        float textOffsetRightX = 0.f;

        for (int i = 0; i < text.length(); i++)
        {
            const char& character = text.at(i);
            ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, &character);

            if (glyph)
            {
                if (i > 0)
                {
                    float kerning = ftgl::texture_glyph_get_kerning(glyph, &text[i - 1]);
                    xPosOfCurrCharacter += kerning;
                }

                int xPos0 = (int) (xPosOfCurrCharacter) + glyph->offset_x;
                int yPos0 = -glyph->offset_y;
                int xPos1 = (int) (xPos0 + glyph->width);
                int yPos1 = (int) (yPos0 + glyph->height);

                float u0 = glyph->s0;
                float v0 = glyph->t0;
                float u1 = glyph->s1;
                float v1 = glyph->t1;

                std::vector<float> textVertexData1 = {static_cast<float>(xPos0), static_cast<float>(yPos0), u0, v0};
                std::vector<float> textVertexData2 = {static_cast<float>(xPos0), static_cast<float>(yPos1), u0, v1};
                std::vector<float> textVertexData3 = {static_cast<float>(xPos1), static_cast<float>(yPos1), u1, v1};
                std::vector<float> textVertexData4 = {static_cast<float>(xPos0), static_cast<float>(yPos0), u0, v0};
                std::vector<float> textVertexData5 = {static_cast<float>(xPos1), static_cast<float>(yPos0), u1, v0};
                std::vector<float> textVertexData6 = {static_cast<float>(xPos1), static_cast<float>(yPos1), u1, v1};

                std::vector<std::vector<float>> textQuadData = {textVertexData1, textVertexData2, textVertexData3, textVertexData4, textVertexData5, textVertexData6};
                textQuadDataList.push_back(textQuadData);

                xPosOfCurrCharacter += glyph->advance_x;

                if (i == 0)
                    textOffsetLeftX = glyph->offset_x;

                if (i == (text.length() - 1))
                    textOffsetRightX = xPosOfCurrCharacter - (float) xPos1;

                if (maxHeightTotal == 0 || !bUseOnlyInitialTextHeight)
                {
                    int currHeightAboveBase = glyph->offset_y;
                    int currHeightBelowBase = yPos1;

                    if (currHeightAboveBase > maxHeightAboveBaseline)
                        maxHeightAboveBaseline = currHeightAboveBase;

                    if (currHeightBelowBase > maxHeightBelowBaseline)
                        maxHeightBelowBaseline = currHeightBelowBase;
                }
            }
        }

        if (maxHeightTotal == 0 || !bUseOnlyInitialTextHeight)
            maxHeightTotal = maxHeightAboveBaseline + maxHeightBelowBaseline;

        if (textWidth != 0.f && bUseOnlyInitialTextWidth)
            return;

        textWidth = xPosOfCurrCharacter - (float) textOffsetLeftX - textOffsetRightX;
    }

    void TextQuads::draw(TextShader* shader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlas->id);

        glBindVertexArray(vao);

        shader->setTextColor(color);

        for (auto textQuadData : textQuadDataList)
        {
            auto textVertexData1 = textQuadData.at(0);
            auto textVertexData2 = textQuadData.at(1);
            auto textVertexData3 = textQuadData.at(2);
            auto textVertexData4 = textQuadData.at(3);
            auto textVertexData5 = textQuadData.at(4);
            auto textVertexData6 = textQuadData.at(5);

            float preparedTextQuadData[6][4] =
            {
                    {textVertexData1.at(0), textVertexData1.at(1), textVertexData1.at(2), textVertexData1.at(3)},
                    {textVertexData2.at(0), textVertexData2.at(1), textVertexData2.at(2), textVertexData2.at(3)},
                    {textVertexData3.at(0), textVertexData3.at(1), textVertexData3.at(2), textVertexData3.at(3)},

                    {textVertexData4.at(0), textVertexData4.at(1), textVertexData4.at(2), textVertexData4.at(3)},
                    {textVertexData5.at(0), textVertexData5.at(1), textVertexData5.at(2), textVertexData5.at(3)},
                    {textVertexData6.at(0), textVertexData6.at(1), textVertexData6.at(2), textVertexData6.at(3)}
            };

            glClear(GL_DEPTH_BUFFER_BIT);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(preparedTextQuadData), preparedTextQuadData);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

        }
        glBindVertexArray(0);
    }

    TextQuads::~TextQuads()
    {
        texture_font_delete(font);
        texture_atlas_delete(textureAtlas);

        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}