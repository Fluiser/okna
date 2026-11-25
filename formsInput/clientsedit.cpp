#include "clientsedit.h"
#include "ui_clientsedit.h"
#include "../Database.h"
#include "../util/DateTime.hpp"
#include <QMessageBox>
#include "../util/QT.h"

extern Database database;

ClientsEdit::ClientsEdit(QWidget *parent, uint32_t *whereSave)
    : QDialog(parent)
    , ui(new Ui::ClientsEdit)
    , _selectedId(whereSave)
{
    _currentClient.type = 0;
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Физ. лицо");
    ui->tabWidget->setTabText(1, "Юр. лицо");

    ui->tableWidget->setColumnWidth(0, 0);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionsMovable(false);
    ui->tabWidget->setEnabled(false);

    ui->tableWidget->adjustSize();

    _clientCount = database->count<Client>();
    this->on_plusOffset_clicked();
}

void ClientsEdit::save()
{
    this->scan();
    if (_currentClient.id > 0)
        database->update(_currentClient);
}

ClientsEdit::~ClientsEdit()
{
    this->save();
    if (_selectedId != nullptr && _currentClient.id > 0)
        *_selectedId = _currentClient.id;
    _currentClient = Client{.id = -1};
    delete ui;
}

#define _SETUITEXT(where, who) ui-> where ->setText(QString::fromStdString(who));
void ClientsEdit::setUI(const Client& cl)
{
    _SETUITEXT(clientName, cl.name);
    _SETUITEXT(clientName2, cl.name);

    _SETUITEXT(address, cl.address);
    _SETUITEXT(address2, cl.address);

    _SETUITEXT(proxies, cl.proxies)
    _SETUITEXT(proxies2, cl.proxies)

    _SETUITEXT(description, cl.description)
    _SETUITEXT(description2, cl.description)

    _SETUITEXT(email, cl.email)
    _SETUITEXT(email2, cl.email)

    _SETUITEXT(fullnameLead, cl.fullnameLead)
    _SETUITEXT(postLead, cl.postLead)
    _SETUITEXT(checkingAccount, cl.checkingAccount)
    _SETUITEXT(bank, cl.bank)
    _SETUITEXT(correspondentAccount, cl.correspondentAccount)
    _SETUITEXT(BIK, cl.BIK)
    _SETUITEXT(bankAddress, cl.bankAddress)
    _SETUITEXT(correspondentRequisites, cl.correspondentRequisites)
    _SETUITEXT(TIN, cl.TIN)
    _SETUITEXT(CPP, cl.CPP)
    _SETUITEXT(MSRN, cl.MSRN)

    DateTime d(cl.MSRN_date);
    ui->MSRNDate->setDate(QDate(d.getYear(), d.getMonth(), d.getDay()));

    _SETUITEXT(phone, cl.phone)
    _SETUITEXT(phone2, cl.phone)

    _SETUITEXT(phoneWork, cl.phoneWork)
    _SETUITEXT(phoneWork2, cl.phoneWork)

    _SETUITEXT(phoneHome, cl.phoneHome)
    _SETUITEXT(FAX, cl.FAX)

    DateTime bd(cl.birdday);
    ui->birdday->setDate(QDate(bd.getYear(), bd.getMonth(), bd.getDay()));


    _SETUITEXT(addressLive, cl.addressLive)
    _SETUITEXT(whoGivePassport, cl.passportGiver)
    _SETUITEXT(passportSeries, cl.passportSeries)
    _SETUITEXT(passportNum, cl.passportId)

    ui->tabWidget->setCurrentIndex(cl.type);
}
#undef _SETUITEXT

void ClientsEdit::addToTableClient(const Client& cl)
{
    ui->tableWidget->insertRow( ui->tableWidget->rowCount() );
    int rid = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(rid, 0, disableEdit(new QTableWidgetItem(QString::number(cl.id))));
    ui->tableWidget->setItem(rid,
                             1,
                             disableEdit(new QTableWidgetItem(QString::fromStdString(cl.name))));
    ui->tableWidget->setItem(rid, 2, disableEdit(new QTableWidgetItem(QString::fromStdString(cl.phoneWork))));
    ui->tableWidget->setItem(rid, 3, disableEdit(new QTableWidgetItem(QString::fromStdString(cl.address))));
}

void ClientsEdit::addToTableClient(const std::vector<Client>& vec)
{
    if (vec.size() < 1)
        return;
    ui->tableWidget->setRowCount(static_cast<int>(vec.size()));
    ui->tableWidget->setColumnCount(4);
    int r = 0;
    for(const auto& cl: vec)
    {
        ui->tableWidget->setItem(r, 0, disableEdit(new QTableWidgetItem(QString::number(cl.id))));
        ui->tableWidget->setItem(r, 1, disableEdit(new QTableWidgetItem(QString::fromStdString(cl.name))));
        ui->tableWidget->setItem(r, 2, disableEdit(new QTableWidgetItem(QString::fromStdString(cl.phoneWork))));
        ui->tableWidget->setItem(r, 3, disableEdit(new QTableWidgetItem(QString::fromStdString(cl.address))));
        ++r;
    }
}

int ClientsEdit::tableAvailableSize() const
{
    return ui->tableWidget->size().height() / 34;
}

void ClientsEdit::on_addClient_clicked()
{
    if (!ui->tabWidget->isEnabled())
        ui->tabWidget->setEnabled(true);
    auto id = database->insert(Client{});
    ++_clientCount;
    _currentClient = database->get<Client>(id);
    addToTableClient(database->get<Client>(id));
    this->setUI(_currentClient);
    ui->browserLabel->setText(
        QString("%1 из %2").arg(_offset + ui->tableWidget->rowCount()).arg(_clientCount));
}

void ClientsEdit::on_removeClient_clicked()
{
    if (_currentClient.id > 0) {
        ui->tableWidget->clearContents();
        database->remove<Client>(_currentClient.id);
        auto vec = database->get_all<Client>(
            limit(this->tableAvailableSize(), offset(_offset)));
        this->addToTableClient(vec);
        --_clientCount;
    }
    _currentClient.id = -1;
    ui->browserLabel->setText(
        QString("%1 из %2").arg(_offset + ui->tableWidget->rowCount()).arg(_clientCount));
}

void ClientsEdit::on_tableWidget_cellClicked(int row, int column)
{
    if (_currentClient.id > 0) {
        this->save();
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    if (id != _currentClient.id) {
        _currentClient = database->get<Client>(id);
        this->setUI(_currentClient);
        if (!ui->tabWidget->isEnabled())
            ui->tabWidget->setEnabled(true);
    }
}

void ClientsEdit::on_minusOffset_clicked()
{
    if (_offset > 0) {
        _offset -= ui->tableWidget->rowCount() > _offset ? _offset
                                                         : this->tableAvailableSize();
        ui->tableWidget->clearContents();
        auto vec = database->get_all<Client>(
            limit(this->tableAvailableSize(), offset(_offset)));
        this->addToTableClient(vec);

        ui->browserLabel->setText(
            QString("%1 из %2").arg(_offset + ui->tableWidget->rowCount()).arg(_clientCount));
    }
}

void ClientsEdit::on_plusOffset_clicked()
{
    if (_offset + ui->tableWidget->rowCount() < _clientCount) {
        _offset += ui->tableWidget->rowCount();
        ui->tableWidget->clearContents();
        auto vec = database->get_all<Client>(
            limit(this->tableAvailableSize(), offset(_offset)));
        this->addToTableClient(vec);
        ui->browserLabel->setText(
            QString("%1 из %2").arg(_offset + ui->tableWidget->rowCount()).arg(_clientCount));
    }
}

void ClientsEdit::on_tabWidget_currentChanged(int index)
{
    this->scan();
    _currentClient.type = index;
    this->setUI(_currentClient);
    if (_currentClient.id > 0) {
        _currentClient.type = index;
        this->save();
    }
}

#define _SETUITEXT(what, where) where = ui->what->text().toStdString();
#define _SETUITEXTAREA(what, where) where = ui->what->toPlainText().toStdString();
void ClientsEdit::scan()
{
    if (_currentClient.type == 0)
        _SETUITEXT(clientName, _currentClient.name)
    else
        _SETUITEXT(clientName2, _currentClient.name)

    if (_currentClient.type == 0)
        _SETUITEXTAREA(address, _currentClient.address)
    else
        _SETUITEXTAREA(address2, _currentClient.address);

    if (_currentClient.type == 0)
        _SETUITEXTAREA(proxies, _currentClient.proxies)
    else
        _SETUITEXTAREA(proxies2, _currentClient.proxies)

    if (_currentClient.type == 0)
        _SETUITEXTAREA(description, _currentClient.description)
    else
        _SETUITEXTAREA(description2, _currentClient.description)

    if (_currentClient.type == 0)
        _SETUITEXT(email, _currentClient.email)
    else
        _SETUITEXT(email2, _currentClient.email)

    _SETUITEXT(fullnameLead, _currentClient.fullnameLead)
    _SETUITEXT(postLead, _currentClient.postLead)
    _SETUITEXT(checkingAccount, _currentClient.checkingAccount)
    _SETUITEXT(bank, _currentClient.bank)
    _SETUITEXT(correspondentAccount, _currentClient.correspondentAccount)
    _SETUITEXT(BIK, _currentClient.BIK)
    _SETUITEXT(bankAddress, _currentClient.bankAddress)
    _SETUITEXTAREA(correspondentRequisites, _currentClient.correspondentRequisites)
    _SETUITEXT(TIN, _currentClient.TIN)
    _SETUITEXT(CPP, _currentClient.CPP)
    _SETUITEXT(MSRN, _currentClient.MSRN)

    auto d = ui->MSRNDate->date();
    _currentClient.MSRN_date = DateTime(d.year(), d.month(), d.day()).getSrc();

    if (_currentClient.type == 0)
        _SETUITEXT(phone, _currentClient.phone)
    else
        _SETUITEXT(phone2, _currentClient.phone)

    if (_currentClient.type == 0)
        _SETUITEXT(phoneWork, _currentClient.phoneWork)
    else
        _SETUITEXT(phoneWork2, _currentClient.phoneWork)

    if (_currentClient.type == 0)
        _SETUITEXT(phoneHome, _currentClient.phoneHome)
    else
        _SETUITEXT(FAX, _currentClient.FAX)

    auto bd = ui->birdday->date();
    _currentClient.birdday = DateTime(bd.year(), bd.month(), bd.day()).getSrc();

    _SETUITEXTAREA(addressLive, _currentClient.addressLive)
    _SETUITEXTAREA(whoGivePassport, _currentClient.passportGiver)
    _SETUITEXT(passportSeries, _currentClient.passportSeries)
    _SETUITEXT(passportNum, _currentClient.passportId)
}
#undef _SETUITEXT

void ClientsEdit::updatePreview()
{
    int rid = -1;
    for (int y = 0; y < ui->tableWidget->rowCount(); ++y) {
        if (ui->tableWidget->item(y, 0)->text().toInt() == _currentClient.id) {
            rid = y;
            break;
        }
    }
    if (rid > 0) {
        ui->tableWidget->setItem(rid,
                                 1,
                                 disableEdit(new QTableWidgetItem(
                                     QString::fromStdString(_currentClient.name))));
        ui->tableWidget->setItem(rid,
                                 2,
                                 disableEdit(new QTableWidgetItem(
                                     QString::fromStdString(_currentClient.phoneWork))));
        ui->tableWidget->setItem(rid,
                                 3,
                                 disableEdit(new QTableWidgetItem(
                                     QString::fromStdString(_currentClient.address))));
    }
}

//update preview

void ClientsEdit::on_clientName_textChanged(const QString &arg1)
{
    _currentClient.name = ui->clientName->text().toStdString();
    this->updatePreview();
}

void ClientsEdit::on_address_textChanged()
{
    _currentClient.address = ui->address->toPlainText().toStdString();
    this->updatePreview();
}

void ClientsEdit::on_phoneWork_textChanged(const QString &arg1)
{
    _currentClient.phoneWork = ui->phoneWork->text().toStdString();
    this->updatePreview();
}
//table2
void ClientsEdit::on_clientName2_textChanged(const QString &arg1)
{
    _currentClient.name = ui->clientName2->text().toStdString();
    this->updatePreview();
}

void ClientsEdit::on_address2_textChanged()
{
    _currentClient.address = ui->address2->toPlainText().toStdString();
    this->updatePreview();
}

void ClientsEdit::on_phoneWork2_textChanged(const QString &arg1)
{
    _currentClient.phoneWork = ui->phoneWork2->text().toStdString();
    this->updatePreview();
}
////////////////////////////////
