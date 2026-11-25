#include <SFML/Graphics.hpp>
namespace WinEdit {
typedef sf::Vector2f v2f;

struct window_t
{
    std::vector<v2f> vertex;
};

struct Shape
{
    v2f start = {0.0f, 0.0f};
    v2f end = {0.0f, 0.0f};
};

class WinEditor
{
    float scale = 1.0f;

public:
    static sf::Font font;

    sf::RenderWindow window;
    v2f offset{.0f, .0f};

    void create(sf::WindowHandle handle);
    void render();

    void draw(const WinEdit::Shape &sh);
    void draw(const sf::RectangleShape &);

    inline v2f MMtoPX(v2f) const;
    inline v2f MMtoPX_offset(v2f) const;
    inline v2f PXtoMM(v2f) const;

    void setScale(float);
    float getScale() const;
    void changeScale(float);
};

} // namespace WinEdit
