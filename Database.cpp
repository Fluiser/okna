#include "Database.h"
#include <QMessageBox>
#include <QString>
#include <shlobj.h>
#include <windows.h>

#pragma comment(lib, "shell32.lib")

extern Database database;

Database::Database()
{

}

using namespace sqlite_orm;

suckdb* Database::operator->()
{
    return &storage.value;
}

#define INIT_LENGTH(x) const int x##_size = lengthArray(x)
const QString Work::type_names[]
    = {"Демонтаж", "Доставка", "Монтаж", "Погрузка/Разгрузка", "Подъем на этаж", "Другое"};
INIT_LENGTH(Work::type_names);

const QString Check::valute_names[] = {"Руб", "Дол", "Евро"};

const QString Component::type_names[] = {"Заполнение",
                                         "Москитные сетки",
                                         "Ограничения",
                                         "Основные профили",
                                         "Прочее"};
INIT_LENGTH(Component::type_names);
const QString Component::unit_type_names[]
    = {"кв.м.", "кг.", "компл.", "куб.м.", "л.", "м.", "мл.", "шт."};
INIT_LENGTH(Component::unit_type_names);
const QString Component::type_printout_names[]
    = {"Армирование",         "Дек. накладки",   "Дист. рамки", "Заполнение",  "М-сетка габариты",
       "М-сетка компл.",      "М-сетка профиль", "Отливы",      "Подоконники", "Профили доп.",
       "Профили осн.",        "Раскладки",       "Ручки",       "Стекло",      "Стеклопакеты",
       "Стеклопакеты компл.", "Сэндвичи",        "Уплотнение",  "Фурнитура",   "Штапики"};
INIT_LENGTH(Component::type_printout_names);
#undef INIT_LEGHT

double Check::sum() const
{
    double sum = 0.0;
    for (const auto &d : database->get_all<Product>(where(is_equal(&Product::check_id, this->id))))
        sum += d.cost;
    for (const auto &d : database->get_all<Work>(where(is_equal(&Work::check_id, this->id))))
        sum += d.sum();
    return (sum * (1.0 - static_cast<double>(this->discount_procent))
            - static_cast<double>(this->discounts));
}

void Database::connect(const QString &filename)
{
#ifdef WIN32
    wchar_t _path[MAX_PATH];
    QString path;

    HRESULT res = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, _path);
    if(res == S_OK) {
        path = QString::fromWCharArray(_path);
        path += "\\";
    }
    path += filename;

    std::string path_db = path.toStdString();
#else
    path_db = QString("~/") + filename;
#endif
    storage.construct(MAKE_DB(path_db));
    storage->sync_schema();
}
