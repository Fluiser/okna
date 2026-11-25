#include <iostream>
#include "WinEditor.cpp"

int main()
{
    WinEdit::WinEditor w;
    w.window.create(sf::VideoMode(1280, 720), "fdsa");

    sf::Event event;
    sf::Vector2i v;
    bool pressed = false;

    while(w.window.isOpen()) {
        while(w.window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) return 0;
            if(event.type == sf::Event::MouseWheelMoved) {
                w.changeScale(w.getScale() * -0.1 * event.mouseWheel.delta);
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                v = sf::Mouse::getPosition(w.window);
                pressed = true;
            }
            if(event.type == sf::Event::MouseButtonReleased) {
                pressed = false;
            }
            if(event.type == sf::Event::MouseMoved) {
                if(pressed) {
                    auto $ = sf::Mouse::getPosition(w.window) - v;
                    w.offset += (sf::Vector2f)$ / (1.0f/(0.1f * w.getScale()));
                    v = sf::Mouse::getPosition(w.window);
                }
            }
        }
        
   
        w.window.clear();
        w.render();
        w.draw(WinEdit::Shape{.start = sf::Vector2f{10.0f, 0.0f}, .end = sf::Vector2f{20.0f, 20.0f}});
        w.window.display();
    }

    return 0;
}
