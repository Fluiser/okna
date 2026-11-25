#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QWindow>
#include "./ui_mainwindow.h"
#include "formsInput/componentsform.h"
#include "formsInput/newproduct.h"
#include "mainForms/checkwindow.h"
#include "mainForms/orderview.h"
#include "mainForms/windoweditor.h"
#include "util/Editform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setMovable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

template<typename T>
    requires std::is_base_of<StaticEditForm, T>::value
void addTable(QTabWidget &tab, QString title)
{
    for (int i = 0; i < tab.count(); ++i) {
        auto *p = dynamic_cast<StaticEditForm *>(tab.widget(i));
        if (p != nullptr) {
            tab.setCurrentIndex(i);
            return;
        }
    }
    tab.addTab(new T(&tab), title);
    tab.setCurrentIndex(tab.count() - 1);
}

template<typename T>
    requires(!std::is_base_of<StaticEditForm, T>::value)
void addTable(QTabWidget &tab, QString title)
{
    tab.addTab(new T(&tab), title);
    tab.setCurrentIndex(tab.count() - 1);
}

void MainWindow::on_action_5_triggered()
{
    addTable<checkwindow>(*(ui->tabWidget), "Новый счёт");
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    EditForm *tab = qobject_cast<EditForm *>(ui->tabWidget->widget(index));
    tab->close();
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_action_7_triggered()
{
    addTable<orderView>(*(ui->tabWidget), "Счета");
}

void MainWindow::on_action_2_triggered()
{
    addTable<ComponentsForm>(*(ui->tabWidget), "Компоненты");
}

void MainWindow::on_action_triggered()
{
    addTable<WindowEditor>(*(ui->tabWidget), "SFML TEST");
}

void MainWindow::on_action_6_triggered()
{
    NewProduct product(this, ui->tabWidget);
    product.exec();
}
