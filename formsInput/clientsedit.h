#ifndef CLIENTSEDIT_H
#define CLIENTSEDIT_H
#include "../Database.h"
#include <QDialog>
#include <QTableWidget>

namespace Ui {
class ClientsEdit;
}

class ClientsEdit : public QDialog
{
    Q_OBJECT

public:
    size_t _offset = 0;
    size_t _clientCount = 0;
    uint32_t *_selectedId = nullptr;
    Client _currentClient = {.id = -1};

    explicit ClientsEdit(QWidget *parent = nullptr, uint32_t *id = nullptr);
    ~ClientsEdit();

    void setUI(const Client&);
    void addToTableClient(const Client&);
    void addToTableClient(const std::vector<Client>&);
    void save(); // save data into database
    void scan(); // take data for struct what gonna be write
    void updatePreview();

    int tableAvailableSize() const;
private slots:
    void on_addClient_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_minusOffset_clicked();

    void on_plusOffset_clicked();

    void on_removeClient_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_clientName_textChanged(const QString &arg1);

    void on_address_textChanged();

    void on_phoneWork_textChanged(const QString &arg1);

    void on_clientName2_textChanged(const QString &arg1);

    void on_address2_textChanged();

    void on_phoneWork2_textChanged(const QString &arg1);

private:
    Ui::ClientsEdit *ui;
};

#endif // CLIENTSEDIT_
