#include <sqlite_orm/sqlite_orm.h>
#pragma once
namespace sqlite_orm {

//uint32_t
template<>
struct type_printer<uint32_t> : public integer_printer
{
    const std::string &print() const
    {
        static const std::string res = "UNSIGNED INT";
        return res;
    }
};

template<>
struct statement_binder<uint32_t>
{
    int bind(sqlite3_stmt *stmt, int index, const uint32_t &val)
    {
        return sqlite3_bind_int64(stmt, index, static_cast<int64_t>(val));
    }
};
template<>
struct field_printer<uint32_t>
{
    std::string operator()(const uint32_t &value) const { return std::to_string(value); }
};

template<>
struct row_extractor<uint32_t>
{
    uint32_t extract(const char *row_value) { return static_cast<uint32_t>(std::stoll(row_value)); }

    uint32_t extract(sqlite3_stmt *stmt, int columnIndex)
    {
        auto i64 = sqlite3_column_int64(stmt, columnIndex);
        return static_cast<uint32_t>(i64);
    }
};
// end uint32_t

} // namespace sqlite_orm
