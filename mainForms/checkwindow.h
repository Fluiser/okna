#ifndef CHECKWINDOW_H
#define CHECKWINDOW_H

#include <QWidget>
#include "../Database.h"
#include "../util/Editform.h"

namespace Ui {
class checkwindow;
}

class checkwindow : public EditForm
{
    Q_OBJECT

    QWidget *_parent;
    Check _check;

    int _work_offset = 0;

public:
    explicit checkwindow(QWidget *parent = nullptr, int id = 0);
    ~checkwindow();

    void scan();
    void updateView();
    void save();

    void work_table_setup(const Work &, int row);
    void work_table_scan(Work &, int row);
public slots:
    virtual void close() override;

    void work_table_onDoubleSpinBoxValueChanged(double);

private slots:
    void on_toolButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_checkN_textChanged(const QString &arg1);

    void on_work_plusbutton_clicked();

    void on_work_minusbutton_clicked();

    void on_deleteCheckButton_clicked();

private:
    Ui::checkwindow *ui;
};

#endif // CHECKWINDOW_H
