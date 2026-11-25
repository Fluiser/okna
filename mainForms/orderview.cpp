#include "orderview.h"
#include <QMessageBox>
#include "../Database.h"
#include "../util/DateTime.hpp"
#include "../util/QT.h"
#include "checkwindow.h"
#include "ui_orderview.h"

extern Database database;
/*
    Обновление при смене флагов:
        изменить тотал-колво
        изменить запросы постоянные с учётом параметров

*/

orderView::orderView(QWidget *parent)
    : StaticEditForm(parent)
    , ui(new Ui::orderView)
{
    ui->setupUi(this);
    ui->tableWidget->adjustSize();
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    this->updateView();
}

void orderView::updateBrowserLabel()
{
    ui->browserLabel->setText(
        QString("%1 из %2").arg(_offset + ui->tableWidget->rowCount()).arg(_totalCount));
}

void orderView::updateView()
{
    // Я хотел добавить 3 типа состояния кнопки
    uint32_t start = ui->thisyear_checkbox->isChecked()
                         ? DateTime(QDate::currentDate().year(), 0, 0).getSrc()
                         : DateTime(ui->dateEdit->date().year(),
                                    ui->dateEdit->date().month(),
                                    ui->dateEdit->date().day())
                               .getSrc();
    auto idx = ui->dateEdit_2->date().year();
    uint32_t end = ui->thisyear_checkbox->isChecked()
                       ? DateTime(QDate::currentDate().year(), 0xff, 0xff).getSrc()
                       : DateTime(ui->dateEdit_2->date().year(),
                                  ui->dateEdit_2->date().month(),
                                  ui->dateEdit_2->date().day())
                             .getSrc();
    _totalCount = database->count<Check>(
        where(c(&Check::paid) == ui->paid_checkbox->isChecked()
              and c(&Check::declined) == ui->canceled_checkbox->isChecked()
              and c(&Check::date) >= start and c(&Check::date) <= end));
    auto v = database->get_all<Check>(where(c(&Check::paid) == ui->paid_checkbox->isChecked()
                                            and c(&Check::declined)
                                                    == ui->canceled_checkbox->isChecked()
                                            and c(&Check::date) >= start and c(&Check::date) <= end),
                                      order_by(&Check::id).desc(),
                                      limit(this->tableAvailableSize()));
    this->clearView();
    this->addToTableCheck(v);
    this->updateBrowserLabel();
}

void orderView::addToTableCheck(const std::vector<Check> &vec)
{
    if (vec.size() < 1)
        return;
    ui->tableWidget->setRowCount(static_cast<int>(vec.size()));
    ui->tableWidget->setColumnCount(6);
    int r = 0;
    for (const auto &check : vec) {
        DateTime t(check.date);
        Client client;

        if (check.client_id > 0
            && database->count<Client>(where(c(&Client::id) == check.client_id))) {
            client = database->get<Client>(check.client_id);
        } else {
            client.name = "ОШИБКА: НЕ СУЩЕСТВУЕТ";
        }

        ui->tableWidget->setItem(r,
                                 0,
                                 disableEdit(new QTableWidgetItem(QString::number(t.getYear()))));

        auto *item_ = disableEdit(new QTableWidgetItem(QString::fromStdString(check.declarationN)));
        item_->setData(Qt::UserRole, QVariant(check.id));
        ui->tableWidget->setItem(r, 1, item_);
        ui->tableWidget->setItem(r, 2, disableEdit(new QTableWidgetItem(QString::number(check.N))));

        ui->tableWidget
            ->setItem(r, 3, disableEdit(new QTableWidgetItem(QString::fromStdString(client.name))));
        ui->tableWidget
            ->setItem(r,
                      4,
                      disableEdit(new QTableWidgetItem(
                          QString("%1.%2.%3").arg(t.getDay()).arg(t.getMonth()).arg(t.getYear()))));
        ui->tableWidget->setItem(r,
                                 5,
                                 disableEdit(new QTableWidgetItem(QString::number(check.sum()))));
        ++r;
    }
}

int orderView::tableAvailableSize()
{
    int x = ui->tableWidget->size().width() / 34;
    return x;
}

void orderView::clearView()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

orderView::~orderView()
{
    delete ui;
}

void orderView::on_pushButton_left_clicked()
{
    _offset -= _offset >= ui->tableWidget->rowCount() ? ui->tableWidget->rowCount() : _offset;
    this->updateView();
}

void orderView::on_pushButton_update_clicked()
{
    this->updateView();
}

void orderView::on_tableWidget_cellDoubleClicked(int row, int column)
{
    auto *item = ui->tableWidget->item(row, 1);
    if (nullptr != item) {
        auto *p = qobject_cast<QTabWidget *>(_parent);
        auto id = item->data(Qt::UserRole).value<decltype(Check::id)>();
        auto check = database->get<Check>(id);
        p->addTab(new checkwindow(p, id), QString("Счет №%1").arg(QString::number(check.N)));
        p->setCurrentIndex(p->count() - 1);
    }
}

void orderView::on_pushButton_minus_clicked()
{
    auto list = ui->tableWidget->selectedItems();
    if (list.count() > 0) {
        int y = (*list.begin())->row();
        auto *item = ui->tableWidget->item(y, 1);
        int id = item->data(Qt::UserRole).value<decltype(Check::id)>();
        database->remove<Check>(id);
        this->updateView();
    }
}

void orderView::on_pushButton_plus_clicked()
{
    auto *p = qobject_cast<QTabWidget *>(_parent);
    p->addTab(new checkwindow(p, 0), QString("Новый счет"));
    p->setCurrentIndex(p->count() - 1);
}
