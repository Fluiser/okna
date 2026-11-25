#ifndef NEWPRODUCT_H
#define NEWPRODUCT_H

#include <QDialog>
#include <QTabWidget>

namespace Ui {
class NewProduct;
}

class NewProduct : public QDialog
{
    Q_OBJECT

    QTabWidget *_tab;

public:
    explicit NewProduct(QWidget *parent = nullptr, QTabWidget *wid = nullptr);
    ~NewProduct();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::NewProduct *ui;
};

#endif // NEWPRODUCT_H
