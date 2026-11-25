#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SFML/Graphics.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    sf::RenderWindow win;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_action_5_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_7_triggered();

    void on_action_2_triggered();

    void on_action_triggered();

    void on_action_6_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
