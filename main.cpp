#include <QApplication>
#include "Database.h"
#include "WinEdit/WinEditor.hpp"
#include "mainwindow.h"
#include <iostream>

Database database;

sf::Font WinEdit::WinEditor::font = {};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    database.connect();

    WinEdit::WinEditor::font.loadFromFile("B.ttf");

    MainWindow w;
    w.setWindowTitle("WinMaster 1.0");
    w.show();

    return a.exec();
}
