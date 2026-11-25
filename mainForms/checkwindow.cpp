#include "checkwindow.h"
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QTabWidget>
#include "../formsInput/clientsedit.h"
#include "../util/DateTime.hpp"
#include "../util/QT.h"
#include "ui_checkwindow.h"
#include <numbers>

extern Database database;

checkwindow::checkwindow(QWidget *parent, int n)
    : EditForm(parent)
    , ui(new Ui::checkwindow)
    , _parent(parent)
{
    ui->setupUi(this);

    _check.id = n;
    //init clients
    auto clients = database->get_all<Client>(order_by(&Client::id).desc(), limit(10));
    for (auto &client : clients) {
        ui->client->addItem(QString::fromStdString(client.name), QVariant(client.id));
    }

    ui->checkN->setValidator(
        new QIntValidator(0, std::numeric_limits<decltype(Check::N)>::max(), this));

    ui->valute->addItem("Рубль", (uint8_t) 0);
    ui->valute->addItem("Доллар", (uint8_t) 1);
    ui->valute->addItem("Евро", (uint8_t) 2);

    if (n < 1) {
        _check.id = 0;
        _check.id = database->insert(_check);
    } else {
        if (database->count<Check>(where(c(&Check::id) == n)) < 1) {
            auto *p = qobject_cast<QTabWidget *>(_parent);
            int idx = 0;
            for (; this != p->widget(idx) && idx < p->count(); ++idx)
                ;
            p->removeTab(idx);
            return;
        }
        _check = database->get<Check>(n);
        this->updateView();
    }

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    auto insertString = [&](QString str, int rid, int idx) {
        ui->tableWidget->setItem(rid, idx, disableEdit(new QTableWidgetItem(str)));
    };

    insertString("Окно", 0, 0);
    insertString("12000", 0, 1);
    insertString("1200мм", 0, 2);
    insertString("2500мм", 0, 3);
    insertString("21.05.2024", 0, 4);

    //|------------------- 4-work --------------------|
    ui->work_table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void checkwindow::close()
{
    this->scan();
    this->save();
}

checkwindow::~checkwindow()
{
    this->close();
    delete ui;
}

void checkwindow::on_toolButton_clicked()
{
    uint32_t id = 0;
    {
        auto c = ClientsEdit(nullptr, (&id));
        c.exec();
    }
    auto v = database->get_all<Client>(where(is_not_equal(&Client::id, id)),
                                       order_by(&Client::id).desc(),
                                       limit(10));
    ui->client->clear();
    if (id > 0) {
        auto cl = database->get<Client>(id);
        ui->client->addItem(QString::fromStdString(cl.name), cl.id);
    }

    for (auto &client : v) {
        ui->client->addItem(QString::fromStdString(client.name), QVariant(client.id));
    }
    ui->client->setCurrentIndex(0);
}

void checkwindow::scan()
{
    _check.declined = ui->checkBox_declined->isChecked();
    _check.paid = ui->checkbox_paid->isChecked();
    _check.in_process = ui->checkBox_inprocess->isChecked();

    _check.N = ui->checkN->text().toInt();
    _check.declarationN = ui->declaration_N->text().toStdString();
    auto d = ui->date->date();
    _check.date = DateTime(d.year(), d.month(), d.day()).getSrc();
    if (ui->client->count() > 0) {
        int index = ui->client->currentIndex();
        _check.client_id = ui->client->itemData(index).value<int>();
    }
    _check.valute = ui->valute->itemData(ui->valute->currentIndex()).value<uint8_t>();
    _check.NDS = ui->nds->value();
    _check.discounts = ui->discountValue->value();
    _check.discount_procent = ui->discountProcent->value() / 100.0;
    _check.description = ui->description->toPlainText().toStdString();
}

void checkwindow::updateView()
{
    ui->checkN->setText(QString::number(_check.N));
    ui->declaration_N->setText(QString::fromStdString(_check.declarationN));

    ui->checkBox_declined->setChecked(_check.declined);
    ui->checkBox_inprocess->setChecked(_check.in_process);
    ui->checkbox_paid->setChecked(_check.paid);
    ui->description->setText(QString::fromStdString(_check.description));

    auto d = DateTime(_check.date);
    ui->date->setDate(QDate(d.getYear(), d.getMonth(), d.getDay()));

    {
        int index = -1;
        for (int i = 0; i < ui->client->count(); ++i) {
            if (ui->client->itemData(i).value<int>() == _check.client_id) {
                index = i;
                break;
            }
        }

        if (index > -1) {
            ui->client->setCurrentIndex(index);
        } else {
            if (database->count<Client>(where(c(&Client::id) == _check.client_id)) < 1) {
                ui->client->setCurrentText("ОШИБКА: НЕ СУЩЕСТВУЕТ");
            } else {
                auto client = database->get<Client>(_check.client_id);
                ui->client->addItem(QString::fromStdString(client.name), client.id);
                ui->client->setCurrentIndex(ui->client->count() - 1);
            }
        }
    }
    ui->valute->setCurrentIndex(_check.valute);
    ui->nds->setValue(_check.NDS);
    ui->discountValue->setValue(_check.discounts);
    ui->discountProcent->setValue(_check.discount_procent * 100.0);
    ui->sum->setValue(_check.sum());
}

void checkwindow::save()
{
    this->scan();
    if (_check.id > 0) {
        if (database->count<Check>(where(c(&Check::id) == _check.id)) > 0) {
            database->update(_check);

            for (int i = 0; i < ui->work_table->rowCount(); ++i) {
                Work w;
                this->work_table_scan(w, i);
                if (database.exist<Work>(w.id)) {
                    w.check_id = _check.id;
                    database->update(w);
                } else
                    database->insert(w);
            }
        } else {
            QMessageBox msg;
            msg.setText(
                QString("Была предпринята попытка сохранения записи, которой не существует в базе "
                        "данных.\nID: %1")
                    .arg(_check.id));
            msg.exec();
            _check.id = 0;
            database->insert(_check);
        }
    }
}

void checkwindow::work_table_scan(Work &w, int r)
{
    if (r >= ui->work_table->rowCount())
        return;
    auto *table = ui->work_table;

    w.id = table->item(r, 4)->data(Qt::UserRole).value<decltype(Work::id)>();

    w.type = static_cast<QComboBox *>(table->cellWidget(r, 0))->currentIndex();
    w.cost = (static_cast<QDoubleSpinBox *>(table->cellWidget(r, 1))->value() / 100.0f);
    w.price = static_cast<QDoubleSpinBox *>(table->cellWidget(r, 2))->value();
    w.count = static_cast<QDoubleSpinBox *>(table->cellWidget(r, 3))->value();
    w.unit_name = table->item(r, 4)->text().toStdString();
    w.discount = (static_cast<QDoubleSpinBox *>(table->cellWidget(r, 5))->value() / 100.0f);
}

void checkwindow::work_table_setup(const Work &w, int r)
{
    auto check = database->get<Check>(w.check_id);

    auto *list = new QComboBox();
    list->addItems(
        {"Демонтаж", "Доставка", "Монтаж", "Погрузка/Разгрузка", "Подъем на этаж", "Другое"});
    list->setCurrentIndex(w.type);

    auto *costEditor = new QDoubleSpinBox;
    costEditor->setMaximum(1e5);
    costEditor->setValue(w.cost * 100);

    auto *priceEditor = new QDoubleSpinBox;
    priceEditor->setMinimum(0);
    priceEditor->setMaximum(1e10);
    priceEditor->setValue(w.price);

    auto *countEditor = new QDoubleSpinBox;
    countEditor->setMaximum(1e10);
    countEditor->setMinimum(0);
    countEditor->setValue(w.count);

    auto *discountEditor = new QDoubleSpinBox;
    discountEditor->setRange(0, 100);
    discountEditor->setValue(w.discount * 100.0);

    ui->work_table->setCellWidget(r, 0, list);
    ui->work_table->setCellWidget(r, 1, costEditor);
    ui->work_table->setCellWidget(r, 2, priceEditor);
    ui->work_table->setCellWidget(r, 3, countEditor);

    auto *item = new QTableWidgetItem(QString::fromStdString(w.unit_name));
    item->setData(Qt::UserRole, QVariant(w.id));
    ui->work_table->setItem(r, 4, item);

    ui->work_table->setCellWidget(r, 5, discountEditor);
    ui->work_table->setItem(r,
                            6,
                            disableEdit(
                                new QTableWidgetItem(QString("%1 %2")
                                                         .arg(w.sum(), 0, ' ', 2)
                                                         .arg(Check::valute_names[check.valute]))));

    for (const auto &x_ptr : std::initializer_list<QDoubleSpinBox *>{costEditor,
                                                                     priceEditor,
                                                                     countEditor,
                                                                     discountEditor}) {
        connect(x_ptr,
                QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this,
                &checkwindow::work_table_onDoubleSpinBoxValueChanged);
    }
}

void checkwindow::work_table_onDoubleSpinBoxValueChanged(double nv)
{
    double sum = 0.0;
    for (int r = 0; r < ui->work_table->rowCount(); ++r) {
        Work w;
        this->work_table_scan(w, r);
        float s = w.sum();
        sum += s;
        ui->work_table->item(r, 6)->setText(
            QString("%1 %2").arg(s, 0, ' ', 2).arg(Check::valute_names[_check.valute]));
    }
    ui->work_sumlabel->setText(
        QString("%1 | сумма: %2").arg(ui->work_table->rowCount()).arg(sum, 0, ' ', 2));
}

void checkwindow::on_tabWidget_currentChanged(int index)
{
    this->scan();
    this->save();

    switch (index) {
    case 2: {
        if (_check.id < 1)
            return;
        auto vec = database->get_all<Work>(where(c(&Work::check_id) == _check.id));
        double sum = 0.0;

        ui->work_table->clearContents();
        ui->work_table->setRowCount(static_cast<int>(vec.size()));
        int r = 0;
        for (const auto &p : vec) {
            sum += p.sum();
            this->work_table_setup(p, r++);
        }
        ui->work_sumlabel->setText(QString("%1 | сумма: %2").arg(vec.size()).arg(sum, 0, ' ', 2));
    } break;
    }
}

void checkwindow::on_checkN_textChanged(const QString &arg1)
{
    int tid = 0;
    for (; ((QTabWidget *) _parent)->widget(tid) != this && tid < ((QTabWidget *) _parent)->count();
         ++tid)
        ;
    ((QTabWidget *) _parent)->setTabText(tid, "Счет №" + ui->checkN->text());
}

void checkwindow::on_work_plusbutton_clicked()
{
    assert(_check.id > 0);
    ui->work_table->insertRow(ui->work_table->rowCount());
    int id = database->insert(Work{});
    Work w = database->get<Work>(id);
    w.check_id = _check.id;
    w.cost = 1.0f;
    database->update(w);
    this->work_table_setup(w, ui->work_table->rowCount() - 1);
    ui->work_table->setCurrentCell(ui->work_table->rowCount(), 0);
}

void checkwindow::on_work_minusbutton_clicked()
{
    auto list = ui->work_table->selectedItems();
    if (list.size() > 0) {
        int r = (*list.begin())->row();
        int id = ui->work_table->item(r, 4)->data(Qt::UserRole).value<decltype(Work::id)>();
        database->remove<Work>(id);
        ui->work_table->removeRow(r);
    }
}

void checkwindow::on_deleteCheckButton_clicked()
{
    database->remove<Check>(_check.id);
    _check.id = 0;
    this->close();
    auto &p = *((QTabWidget *) _parent);
    for (int i = 0; i < p.count(); ++i)
        if (p.widget(i) == (QWidget *) this) {
            ((QTabWidget) _parent).removeTab(i);
            delete this;
            break;
        }
}
