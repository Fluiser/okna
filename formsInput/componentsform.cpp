#include "componentsform.h"
#include <QMessageBox>
#include "../util/QT.h"
#include "ui_componentsform.h"

extern Database database;

ComponentsForm::ComponentsForm(QWidget *parent)
    : EditForm(parent)
    , ui(new Ui::ComponentsForm)
{
    ui->setupUi(this);

    for (int i = 0; i < Component::type_printout_names_size; ++i) {
        ui->comboBox->addItem(Component::type_printout_names[i]);
        ui->printout_combobox->addItem(Component::type_printout_names[i]);
    }
    for (int i = 0; i < Component::unit_type_names_size; ++i)
        ui->unittype_selector->addItem(Component::unit_type_names[i]);

    ui->tabWidget->setDisabled(true);

    this->updateView();

    ui->components_table->setSelectionMode(QAbstractItemView::SingleSelection);
}

int ComponentsForm::_totalCount_upd()
{
    return (_totalCount = database->count<Component>(where(is_equal(&Component::deleted, false))));
}

void ComponentsForm::select_component(decltype(Component::id) id)
{
    ui->tabWidget->setDisabled(false);
    _component = database->get<Component>(id);
    auto &c = _component;

    ui->width_component->setValue(c.width);
    ui->height_component->setValue(c.height);

    ui->unittype_selector->setCurrentIndex(c.unit_type);

    ui->frames_component->setValue(c.frames);
    ui->impost_component->setValue(c.impost);

    ui->fitting_groove_component->setValue(c.fitting_groove);
    ui->arch_exntesion_component->setValue(c.arch_exntesion);

    ui->printout_table->setRowCount(0);

    for (int i = 0; i < Component::type_printout_names_size; ++i) {
        if (c.type_printout & (1 << (i + 1))) {
            auto *item = disableEdit(new QTableWidgetItem(Component::type_printout_names[i]));
            item->setData(Qt::UserRole, QVariant(static_cast<int>(1 << (i + 1))));

            ui->printout_table->insertRow(ui->printout_table->rowCount());
            ui->printout_table->setItem(ui->printout_table->rowCount() - 1, 0, item);
        }
    }
}

void ComponentsForm::scan_component(Component &c, int r)
{
    c.type = static_cast<QComboBox *>(ui->components_table->cellWidget(r, 0))->currentIndex();
    c.name = ui->components_table->item(r, 1)->text().toStdString();
    c.vendor_code = ui->components_table->item(r, 2)->text().toStdString();
    c.id = ui->components_table->item(r, 2)->data(Qt::UserRole).value<decltype(Component::id)>();
    c.deleted = static_cast<QCheckBox *>(ui->components_table->cellWidget(r, 3))->isChecked();
}

void ComponentsForm::scan_current_component()
{
    auto &c = _component;

    if (c.id < 1)
        return;

    c.width = ui->width_component->value();
    c.height = ui->height_component->value();
    c.unit_type = ui->unittype_selector->currentIndex();

    c.frames = ui->frames_component->value();
    c.impost = ui->impost_component->value();

    c.fitting_groove = ui->fitting_groove_component->value();
    c.arch_exntesion = ui->arch_exntesion_component->value();

    c.type_printout = 0;
    for (int r = 0; r < ui->printout_table->rowCount(); ++r) {
        auto *item = ui->printout_table->item(r, 0);

        auto data = item->data(Qt::UserRole);
        int x = data.value<int>();

        c.type_printout |= data.value<int>();
    }
}

void ComponentsForm::setup_to_table(const Component &c, int r)
{
    if (ui->components_table->rowCount() <= r)
        ui->components_table->insertRow(r);

    auto *type = new QComboBox;
    for (size_t i = 0; i < Component::type_names_size; ++i) {
        type->addItem(Component::type_names[i]);
    }
    type->setCurrentIndex(c.type);

    auto *isDeleted = new QCheckBox;
    isDeleted->setChecked(c.deleted);

    auto *item = new QTableWidgetItem(QString::fromStdString(c.vendor_code));
    item->setData(Qt::UserRole, QVariant(c.id));

    ui->components_table->setCellWidget(r, 0, type);
    ui->components_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(c.name)));
    ui->components_table->setItem(r, 2, item);
    ui->components_table->setCellWidget(r, 3, isDeleted);
}

void ComponentsForm::updateView()
{
    if (_totalCount < 1)
        _totalCount_upd();
    ui->components_table->clearContents();

    int selectedType_printout = ui->comboBox->currentIndex();
    if (selectedType_printout > 0)
        selectedType_printout = 1 << selectedType_printout;

    int max_ = this->tableAvailableSize();

    std::vector<Component> v;
    {
        auto v_orig = ui->checkBox->isChecked()
                          ? database->get_all<Component>(where(c(&Component::type_printout)
                                                               >= selectedType_printout),
                                                         limit(max_ * 3, offset(_offset)))
                          : database->get_all<Component>(where(is_equal(&Component::deleted, false)),
                                                         limit(max_ * 3, offset(_offset)));
        if (selectedType_printout)
            for (auto &x : v_orig) {
                if (x.type_printout & selectedType_printout)
                    v.emplace_back(x);
            }
        else
            v = v_orig;
    }

    ui->components_table->setRowCount(std::min(max_, static_cast<int>(v.size())));

    int r = 0;
    for (const auto &c : v) {
        if (r >= max_)
            break;
        this->setup_to_table(c, r++);
    }
    ui->components_label->setText(
        QString("%1 из %2").arg(ui->components_table->rowCount() + _offset).arg(_totalCount));
}

int ComponentsForm::tableAvailableSize() const
{
    //    ui->components_table->adjustSize();
    int c = ui->components_table->size().height() / 34;
    if (c < 1)
        return 5;
    return c;
}

ComponentsForm::~ComponentsForm()
{
    delete ui;
}

void ComponentsForm::on_comboBox_currentIndexChanged(int index)
{
    this->updateView();
}

void ComponentsForm::on_components_left_clicked()
{
    int prev = _offset;
    if (_offset > 0)
        _offset -= ui->components_table->rowCount() > _offset ? _offset
                                                              : this->tableAvailableSize();
    if (prev != _offset) {
        this->save();
        this->updateView();
    }
}

void ComponentsForm::on_components_right_clicked()
{
    this->_totalCount_upd();
    if (_offset + ui->components_table->rowCount() < _totalCount) {
        _offset += ui->components_table->rowCount();
        this->save();
        this->updateView();
    }
}

void ComponentsForm::close()
{
    this->save();
}

void ComponentsForm::save()
{
    for (int r = 0; r < ui->components_table->rowCount(); ++r) {
        Component copy;
        this->scan_component(copy, r);
        auto c = database->get<Component>(copy.id);
        if (database.exist<Component>(c.id)) {
            c.type = copy.type;
            c.name = copy.name;
            c.vendor_code = copy.vendor_code;
            c.id = copy.id;
            c.deleted = copy.deleted;

            database->update(c);
        }
    }
}

void ComponentsForm::on_components_plus_clicked()
{
    auto id = database->insert<Component>(Component{});
    auto c = database->get<Component>(id);
    this->setup_to_table(c, ui->components_table->rowCount());
    this->select_component(id);
}

void ComponentsForm::on_components_minus_clicked()
{
    auto list = ui->components_table->selectedItems();
    if (list.size() > 0) {
        int r = (*list.begin())->row();
        int id
            = ui->components_table->item(r, 2)->data(Qt::UserRole).value<decltype(Component::id)>();
        ui->components_table->removeRow(r);
        database->remove<Component>(id);
    }
}

void ComponentsForm::on_pushButton_8_clicked()
{
    for (const auto &item : ui->printout_table->selectedItems()) {
        ui->printout_table->removeRow(item->row());
    }
}

void ComponentsForm::on_pushButton_7_clicked()
{
    ui->printout_table->insertRow(ui->printout_table->rowCount());
    int i = ui->printout_combobox->currentIndex();
    auto *item = disableEdit(new QTableWidgetItem(Component::type_printout_names[i]));

    item->setData(Qt::UserRole, QVariant(static_cast<int>(1 << (i + 1))));

    ui->printout_table->setItem(ui->printout_table->rowCount() - 1, 0, item);
}

void ComponentsForm::on_checkBox_clicked()
{
    this->save();
    this->updateView();
}

void ComponentsForm::on_components_table_cellClicked(int row, int column)
{
    if (ui->components_table->rowCount() < 1)
        return;
    auto *item = ui->components_table->item(row, 2);

    if (nullptr == item)
        return;

    if (_component.id)
        if (database.exist<Component>(_component.id)) {
            this->scan_current_component();
            database->update(_component);
        }

    auto id = item->data(Qt::UserRole).value<decltype(Component::id)>();

    this->select_component(id);
    _component.id = id;
}

void ComponentsForm::on_tabWidget_currentChanged(int index)
{
    int count = 0;
    for (int i = 1; i <= 32; ++i)
        if (_component.type_printout & (1 << i))
            ++count;
    ui->label_3->setText(QString("%1 из 20").arg(count));
}
