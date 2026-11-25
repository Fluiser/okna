#include "WinEditor.hpp"
#include <cmath>
#include <qstring.h>

namespace WinEdit {

    void WinEditor::create(sf::WindowHandle handle)
    {
        window.create(handle);
    }

    inline v2f WinEditor::PXtoMM(v2f x) const
    {
        float mmppx = 0.1f * scale; // mm per pixel
        float pxpmm = (1.0f/mmppx);

        return ((x/pxpmm) - offset);
    }

    inline v2f WinEditor::MMtoPX_offset(v2f x) const
    {
        float mmppx = 0.1f * scale; // mm per pixel
        float pxpmm = (1.0f / mmppx);

        return (x + offset) * pxpmm;
    }

    inline v2f WinEditor::MMtoPX(v2f x) const
    {
        float mmppx = 0.1f * scale; // mm per pixel
        float pxpmm = (1.0f / mmppx);

        return (x) *pxpmm;
    }

    void WinEditor::setScale(float x)
    {
        if (x < 40.0f && x > 0.125f) {
            scale = x;
            return;
        }
    }

    float WinEditor::getScale() const {return scale;}

    void WinEditor::changeScale(float s)
    {
        setScale(scale + s);
    }

    void WinEditor::draw(const sf::RectangleShape &_sh)
    {
        sf::RectangleShape shape = _sh;
        shape.setPosition(WinEditor::MMtoPX_offset(shape.getPosition()));
        shape.setSize(WinEditor::MMtoPX(shape.getSize()));

        auto size = shape.getSize();
        auto pos = shape.getPosition();

        sf::VertexArray vx(sf::Lines, 12);
        vx[0].position = pos + v2f{0.0f, size.y + 16.0f};
        vx[1].position = pos + v2f{size.x, size.y + 16.0f};

        vx[2].position = pos + v2f{size.x + 16.0f, 0};
        vx[3].position = pos + v2f{size.x + 16.0f, size.y};

        for (int i = 0; i < 12; ++i)
            vx[i].color = sf::Color::Black;

        vx[4].position = vx[0].position - v2f{0.0f, 5.0f};
        vx[5].position = vx[0].position + v2f{0.0f, 5.0f};

        vx[6].position = vx[1].position - v2f{0.0f, 5.0f};
        vx[7].position = vx[1].position + v2f{0.0f, 5.0f};

        vx[8].position = vx[2].position - v2f{5.0f, 0.0f};
        vx[9].position = vx[2].position + v2f{5.0f, 0.0f};

        vx[10].position = vx[3].position - v2f{5.0f, 0.0f};
        vx[11].position = vx[3].position + v2f{5.0f, 0.0f};

        window.draw(vx);

        window.draw(shape);

        sf::Text text;
        text.setFont(WinEditor::font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);

        text.setString((QString::number(_sh.getSize().x, 'g', '1') + "mm").toStdString());
        text.setPosition(vx[0].position
                         + v2f{(shape.getSize().x - text.getLocalBounds().width) / 2.0f, -16.0f});
        window.draw(text);

        text.setString((QString::number(_sh.getSize().y, 'g', '1') + "mm").toStdString());
        text.setPosition(vx[2].position
                         + v2f{-16.0f, (shape.getSize().y + text.getLocalBounds().width) / 2.0f});
        text.setRotation(-90.0f);
        window.draw(text);
    }

    void WinEditor::draw(const WinEdit::Shape &sh)
    {
        float mmppx = 0.1f * scale; // mm per pixel
        float pxpmm = (1.0f/mmppx);

        sf::VertexArray vx(sf::Lines, 2);
        vx[0].color = sf::Color::Red;
        vx[1].color = sf::Color::Red;

        vx[0].position = (sh.start + offset) * pxpmm;
        vx[1].position = (sh.end + offset) * pxpmm;

        window.draw(vx);
    }

    void WinEditor::render()
    {
        float mmppx = 0.01f * scale; // mm per pixel
        float pxpmm = (1.0f/mmppx);
        auto size = window.getSize();

        int linesX = size.x/(1.0f/mmppx) + 2;
        int linesY = size.y/(1.0f/mmppx) + 2;

        float offsetX = offset.x / mmppx / 10.0f;
        offsetX = offsetX - (std::truncf(offsetX/pxpmm) * pxpmm);
        float offsetY = offset.y / mmppx / 10.0f;
        offsetY = offsetY - (std::truncf(offsetY / pxpmm) * pxpmm);

        sf::VertexArray lines(sf::Lines, (linesX + linesY) * 2);

        sf::Color color = sf::Color(0, 0, 0, (1.0f - scale / 40.0f) * 10 + 5);

        for(int y = 0; y < linesY; ++y)
        {
            lines[y*2].color = color;
            lines[y*2 + 1].color = color;
            lines[y*2].position = v2f{0, offsetY + pxpmm*y}; 
            lines[y*2 + 1].position = v2f{static_cast<float>(size.x), offsetY + pxpmm*y}; 
        }
        
        for(int x = 0; x < linesX; ++x)
        {
            lines[linesY*2 + x*2].color = color;
            lines[linesY*2 + x*2 + 1].color = color;
            lines[linesY*2 + x*2].position = v2f{offsetX + pxpmm*x, 0}; 
            lines[linesY*2 + x*2 + 1].position = v2f{offsetX + pxpmm*x, static_cast<float>(size.y)}; 
        }
        window.draw(lines);
    }

}
