#ifndef DB_H
#define DB_H

#include <QString>
#include <sqlite_orm/sqlite_orm.h>

struct Client {
    int id = -1;
    int8_t type = 0; // 0 - физ лицо. 1 - юр. лицо.
    std::string name = "";
    std::string address = "";
    //Физ лицо
    std::string passportSeries = ""; // Серия паспорта
    std::string passportId = ""; // Ид паспорта
    std::string passportGiver = ""; // Кто дал
    std::string addressLive = ""; // Прописка
    uint32_t birdday = 0; // unix
    std::string phoneHome = ""; // домашний телефон
    // Юр. лицо
    std::string fullnameLead = ""; // ФИО руковода
    std::string postLead = ""; // Должность руковода
    std::string checkingAccount = ""; // Расчёт счёт
    std::string bank = ""; // банк
    std::string correspondentAccount = ""; // Корр. счет
    std::string BIK = ""; // БИК. В душе не ебу, чё это
    std::string bankAddress = ""; // Адресс банка
    std::string correspondentRequisites = ""; // Реквизиты коррес.
    std::string FAX = ""; // Факс
    std::string TIN = ""; // ИНН
    std::string CPP = ""; // КПП
    std::string MSRN = ""; // ОГРНГ
    uint32_t MSRN_date = 0; // ОГРН дата
    //

    std::string proxies = ""; // доверенные рыла
    std::string description = ""; // примечание
    std::string phoneWork = ""; // раб. телефон
    std::string phone = ""; // угадай
    std::string email = ""; // тоже самое
};

struct Check
{
    static const QString valute_names[];
    double sum() const;

    bool paid = false;
    bool declined = false;
    bool in_process = false;

    int id = 0; // внутренний ид
    int N = 0;  //Номер
    std::string declarationN = "";
    uint32_t date = 0;
    int client_id = 0;
    uint8_t valute = 0; // 0-рубль, 1 - доллар, 2- евро
    double NDS = 0.0;
    //Скидка/
    float discounts = 0.0f;
    float discount_procent = 0.0f;

    std::string description = "";
};

struct Work
{
    int id = 0;
    int check_id = 0;

    float sum() const { return (price * cost * count) * (1 - discount); }
    static const int type_names_size;
    static const QString type_names[];
    int8_t type = 0;       // Наименование: монтаж, доставка и т.д.
    float price = 0.0;     // Цена
    float discount = 0.0f; // Скидка
    float cost = 1.0f;     // процент стоимости
    float count = 1.0f;
    std::string unit_name = ""; // Единица измерения
};

struct Component
{
    int id = 0;
    static const int type_names_size;
    static const QString type_names[];
    int8_t type = 0;
    std::string name = "";
    std::string vendor_code = ""; // Артикул
    bool deleted = false;
    //    int nomenclature_group_id = 0;

    float height = 0.0f;
    float width = 0.0f;

    int8_t unit_type = 0;
    static const int unit_type_names_size;
    static const QString unit_type_names[];

    float frames = 0.0f; // рамы, створки
    float impost = 0.0f; // Импосты, штульпы

    float fitting_groove = 0.0f; // разм. до фурн. паза
    float arch_exntesion = 0.0f; // Удлинение арки

    static const int type_printout_names_size;
    static const QString type_printout_names[];
    enum type_printout_t {
        Reinforcement = 1 << 1,
        Dec_overlays = 1 << 2,
        Dist_framework = 1 << 3,
        Filling = 1 << 4,
        M_mesh_dimensions = 1 << 5,
        M_mesh_set = 1 << 6,
        M_mesh_profile = 1 << 7,
        Low_tides = 1 << 8,
        Window_sills = 1 << 9,
        Additional_profiles = 1 << 10,
        Main_profiles = 1 << 11,
        Layouts = 1 << 12,
        Pens = 1 << 13,
        Glass = 1 << 14,
        Double_glazed_windows = 1 << 15,
        Double_glazed_windows_set = 1 << 16,
        Sandwiches = 1 << 17,
        Seal = 1 << 18,
        Accessories = 1 << 19,
        Glazing_beads = 1 << 20
    };

    int type_printout = 0;
};

struct Product
{
    size_t check_id = 0;
    size_t id = 0; // внутренний ид
    int N = 0;     //Номер
    size_t sketch_id = 0;
    std::string color = "";
    float square_area = 0.0;
    std::string description = "";
    uint8_t state = 0; //  0 - Не сделано; 1 - в процессе; 2 - не установлено; 3 - сделано
    uint32_t datecomplete = 0;
    uint32_t scheduled_date = 0;
    double cost = 0.0; // =0.0 - значит, не установлено
};

template<class T, size_t N>
size_t constexpr lengthArray(const T (&a)[N])
{
    return N;
}

using namespace sqlite_orm;

#include "Database_orm.h"

#define MAKE_DB(path) \
    make_storage(path, \
                 make_table("Clients", \
                            make_column("id", &Client::id, primary_key().autoincrement()), \
                            make_column("name", &Client::name), \
                            make_column("type", &Client::type), \
                            make_column("address", &Client::address), \
                            make_column("passportSeries", &Client::passportSeries), \
                            make_column("passportId", &Client::passportId), \
                            make_column("passportGiver", &Client::passportGiver), \
                            make_column("addressLive", &Client::addressLive), \
                            make_column("birdday", &Client::birdday), \
                            make_column("phoneHome", &Client::phoneHome), \
                            make_column("fullnameLead", &Client::fullnameLead), \
                            make_column("postLead", &Client::postLead), \
                            make_column("checkingAccount", &Client::checkingAccount), \
                            make_column("bank", &Client::bank), \
                            make_column("correspondentAccount", &Client::correspondentAccount), \
                            make_column("BIK", &Client::BIK), \
                            make_column("bankAddress", &Client::bankAddress), \
                            make_column("correspondentRequisites", \
                                        &Client::correspondentRequisites), \
                            make_column("FAX", &Client::FAX), \
                            make_column("TIN", &Client::TIN), \
                            make_column("CPP", &Client::CPP), \
                            make_column("MSRN", &Client::MSRN), \
                            make_column("MSRN_date", &Client::MSRN_date), \
                            make_column("proxies", &Client::proxies), \
                            make_column("description", &Client::description), \
                            make_column("phoneWork", &Client::phoneWork), \
                            make_column("phone", &Client::phone), \
                            make_column("email", &Client::email)), \
                 make_table("Check", \
                            make_column("id", &Check::id, primary_key().autoincrement()), \
                            make_column("paid", &Check::paid), \
                            make_column("in_process", &Check::in_process), \
                            make_column("declined", &Check::declined), \
                            make_column("declarationN", &Check::declarationN), \
                            make_column("date", &Check::date), \
                            make_column("client_id", &Check::client_id), \
                            make_column("valute", &Check::valute), \
                            make_column("NDS", &Check::NDS), \
                            make_column("discounts", &Check::discounts), \
                            make_column("discount_procent", &Check::discount_procent), \
                            make_column("description", &Check::description), \
                            make_column("N", &Check::N)), \
                 make_table("Product", \
                            make_column("id", &Product::id, primary_key().autoincrement()), \
                            make_column("check_id", &Product::check_id), \
                            make_column("N", &Product::N), \
                            make_column("cost", &Product::cost), \
                            make_column("sketch_id", &Product::sketch_id), \
                            make_column("color", &Product::color), \
                            make_column("square_area", &Product::square_area), \
                            make_column("description", &Product::description), \
                            make_column("state", &Product::state), \
                            make_column("datecomplete", &Product::datecomplete), \
                            make_column("scheduled_date", &Product::scheduled_date)), \
                 make_table("Work", \
                            make_column("id", &Work::id, primary_key().autoincrement()), \
                            make_column("check_id", &Work::check_id), \
                            make_column("type", &Work::type), \
                            make_column("price", &Work::price), \
                            make_column("discount", &Work::discount), \
                            make_column("cost", &Work::cost), \
                            make_column("count", &Work::count), \
                            make_column("unit_name", &Work::unit_name)), \
                 make_table("Component", \
                            make_column("id", &Component::id, primary_key().autoincrement()), \
                            make_column("type", &Component::type), \
                            make_column("name", &Component::name), \
                            make_column("vendor_code", &Component::vendor_code), \
                            make_column("deleted", &Component::deleted), \
                            make_column("height", &Component::height), \
                            make_column("width", &Component::width), \
                            make_column("unit_type", &Component::unit_type), \
                            make_column("frames", &Component::frames), \
                            make_column("impost", &Component::impost), \
                            make_column("fitting_groove", &Component::fitting_groove), \
                            make_column("arch_exntesion", &Component::arch_exntesion), \
                            make_column("type_printout", &Component::type_printout)))

typedef decltype(MAKE_DB("")) suckdb;

template<class T>
union wrapperFromShit {
    char mem[sizeof(T)];
    T value;

    wrapperFromShit()
    {}
    template<class... Args>
    void construct(Args&&... args)
    {
        new (&value) T(args...);
    }
    ~wrapperFromShit()
    { value.~T(); }

    T *operator->() { return &value; }
};

class Database
{
    wrapperFromShit<suckdb> storage;
public:
    Database();
    void connect(const QString &path = "db.sqlite");
    suckdb *operator->();

    template<class T>
    bool exist(const decltype(T::id) &id)
    {
        return this->storage->count<T>(where(c(&T::id) == id), limit(1));
    }
};

#endif // DB_H
