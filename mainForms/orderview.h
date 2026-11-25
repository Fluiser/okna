#ifndef ORDERVIEW_H
#define ORDERVIEW_H

#include <QWidget>
#include "../Database.h"
#include "../util/Editform.h"
#include <vector>

namespace Ui {
class orderView;
}

class orderView : public StaticEditForm
{
    Q_OBJECT

    int _offset = 0;
    int _totalCount = 0;

    void clearView();
    void addToTableCheck(const std::vector<Check> &);
    void updateBrowserLabel();
    void updateView();

    int tableAvailableSize();

public:
    explicit orderView(QWidget *parent = nullptr);
    ~orderView();

private slots:
    void on_pushButton_left_clicked();

    void on_pushButton_update_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_minus_clicked();

    void on_pushButton_plus_clicked();

private:
    Ui::orderView *ui;
};

#endif // ORDERVIEW_H
