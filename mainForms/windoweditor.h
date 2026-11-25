#ifndef WINDOWEDITOR_H
#define WINDOWEDITOR_H

#include <QWidget>
#include "../WinEdit/WinEditor.hpp"
#include "../util/Editform.h"
#include <vector>

namespace Ui {
class WindowEditor;
}


class WindowEditor : public EditForm
{
    Q_OBJECT

    WinEdit::WinEditor window;
    QTimer *timer;

    sf::Event event;
    sf::Vector2i v;
    bool pressed = false;

public:
    explicit WindowEditor(QWidget *parent = nullptr);
    ~WindowEditor();

    void render();
public slots:
    virtual void close() override;

private:
    Ui::WindowEditor *ui;
};

#endif // WINDOWEDITOR_H
