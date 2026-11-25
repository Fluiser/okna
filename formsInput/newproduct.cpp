#include "newproduct.h"
#include <QSpinBox>
#include "../mainForms/windoweditor.h"
#include "../util/QT.h"
#include "ui_newproduct.h"

NewProduct::NewProduct(QWidget *parent, QTabWidget *w)
    : QDialog(parent)
    , ui(new Ui::NewProduct)
    , _tab(w)

{
    ui->setupUi(this);
    this->setWindowTitle("Выбор нового изделия");

    ui->listWidget->addItems({"Основная", "С фурнитурой"});

    auto insertString = [&](QString str, QWidget *w) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int rid = ui->tableWidget->rowCount() - 1;
        ui->tableWidget->setItem(rid, 0, disableEdit(new QTableWidgetItem(str)));
        ui->tableWidget->setCellWidget(rid, 1, w);
    };

    for (const QString &s : QStringList{"Кол-во камер", "Ширина рамы", "Ширина стеклопакета"}) {
        auto item = new QSpinBox(nullptr);
        item->setMinimum(0);
        item->setMaximum(0xfffffff);
        insertString(s, item);
    }
}

NewProduct::~NewProduct()
{
    delete ui;
}

void NewProduct::on_pushButton_2_clicked()
{
    _tab->addTab(new WindowEditor(_tab), "Чертёж");
    _tab->setCurrentIndex(_tab->count() - 1);
    this->close();
}

void NewProduct::on_pushButton_clicked()
{
    this->close();
}
