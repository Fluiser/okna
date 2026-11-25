#ifndef COMPONENTSFORM_H
#define COMPONENTSFORM_H

#include <QWidget>
#include "../Database.h"
#include "../util/Editform.h"

namespace Ui {
class ComponentsForm;
}

class ComponentsForm : public EditForm
{
    Q_OBJECT

    int _offset = 0;
    int _totalCount = 0;
    int _totalCount_upd();

    Component _component;

public:
    explicit ComponentsForm(QWidget *parent = nullptr);
    ~ComponentsForm();

    void setup_to_table(const Component &, int r);
    void select_component(decltype(Component::id) id);
    void scan_component(Component &, int r);
    void scan_current_component();

    void updateView();
    int tableAvailableSize() const;

    void save();

public slots:
    virtual void close() override;

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void on_components_right_clicked();

    void on_components_left_clicked();

    void on_components_plus_clicked();

    void on_components_minus_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_checkBox_clicked();

    void on_components_table_cellClicked(int row, int column);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::ComponentsForm *ui;
};

#endif // COMPONENTSFORM_H
