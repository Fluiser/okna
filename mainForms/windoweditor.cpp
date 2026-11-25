#include "windoweditor.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include "ui_windoweditor.h"

WindowEditor::WindowEditor(QWidget *parent)
    : EditForm(parent)
    , ui(new Ui::WindowEditor)
{
    ui->setupUi(this);

    window.create((HWND) ui->graphicsView->winId());
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &WindowEditor::render);
    timer->start(20);
}

void WindowEditor::close()
{
    delete timer;
}

void WindowEditor::render()
{
    while (window.window.pollEvent(event)) {
        if (event.type == sf::Event::MouseWheelMoved) {
            window.changeScale(window.getScale() * -0.1 * event.mouseWheel.delta);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            v = sf::Mouse::getPosition(window.window);
            pressed = true;
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            pressed = false;
        }
        if (event.type == sf::Event::MouseMoved) {
            if (pressed) {
                auto $ = sf::Mouse::getPosition(window.window) - v;
                window.offset += (sf::Vector2f) $ / (1.0f / (0.1f * window.getScale()));
                v = sf::Mouse::getPosition(window.window);
            }
        }
    }

    window.window.clear(sf::Color::White);
    window.render();

    sf::RectangleShape shape({607, 1412});
    shape.setFillColor(sf::Color(00, 0x78, 0xBE, 0xFF));
    window.draw(shape);
    window.window.display();
}

WindowEditor::~WindowEditor()
{
    delete ui;
}
