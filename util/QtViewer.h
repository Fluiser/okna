#ifndef QTVIEWER_H
#define QTVIEWER_H

#include <QTableWidget>
#include <functional>
#include <tuple>
#include <type_traits>

template<class T>
struct get_parent;

template<class T, typename Member>
struct get_parent<Member T::*>
{
    using Type = T;
};

template<typename T>
struct get_field_type;

template<typename T, typename U>
struct get_field_type<T U::*>
{
    using type = T;
};

template<class T, typename Member, Member T::*member, class _WidgetType = QWidget>
struct QtViewer_item
{
    using WidgetType = _WidgetType;
    using ValueType = decltype(((T *) (0))->*member);

    typedef WidgetType *ViewModel;

    QtViewer_item(ViewModel widget)
        :

        item(widget)
    {}

    ViewModel item;

    void set_value(const T &x) { QtViewer_item::set_value(item, x.*member); }
    void scan_value(T &x) { QtViewer_item::scan_value(item, x.*member); }

private:
    static void set_value(ViewModel item, int x) { item->setText(QString::number(x)); }
    static void set_value(ViewModel item, const std ::string &x)
    {
        item->setText(QString::fromStdString(x));
    }
    static void set_value(ViewModel item, const QString &x) { item->setText(x); }
    static void scan_value(ViewModel item, QString &x) { x = item->text(); }
    static void scan_value(ViewModel item, std::string &x) { x = item->text().toStdString(); }
};

template<class T, typename... ViewerMembers>
class QtViewer
{
    std::tuple<ViewerMembers...> members;
    QTableWidget *table;

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type add_row(const T &val,
                                                                           QTableWidget *tab,
                                                                           int r,
                                                                           std::tuple<Tp...> &t)
    {}

    template<std::size_t I = 0, typename... Tp>
        inline typename std::enable_if < I<sizeof...(Tp), void>::type add_row(const T &val,
                                                                              QTableWidget *tab,
                                                                              int r,
                                                                              std::tuple<Tp...> &t)
    {
        auto &item = std::get<I>(t);

        using widgetType = typename std::
            tuple_element<I, typename std::remove_reference<decltype(t)>::type>::type::WidgetType;

        item.item = new widgetType;

        item.set_value(val);

        add_row<I + 1, Tp...>(val, tab, r, t);
    }

public:
    QtViewer(decltype(members) &&m, QTableWidget *windowRR = nullptr)
        : members(m)
        , table(windowRR)
    {}

    void clear() { table->setRowCount(0); }
    void addRow(const T &v)
    {
        int r = table->rowCount();
        table->insertRow(r);
        add_row(v, table, r, members);
    }
};

#define make_viewmod(member, Widget) \
    QtViewer_item<get_parent<decltype(member)>::Type, \
                  get_field_type<decltype(member)>::type, \
                  member, \
                  Widget>

#define make_viewer(type, nameVariable, VIEWTYPE, widgetsTree) \
    QtViewer<type, VIEWTYPE> nameVariable(std::make_tuple(VIEWTYPE(widgetsTree)))

QTableWidgetItem *fakeTable = nullptr; // because it's test

struct foo
{
    int field;
    float second_field;
};

make_viewer(foo, f, make_viewmod(&foo::field, QTableWidgetItem), fakeTable);

void test_call()
{
    foo example{};
    f.clear();
    f.addRow(example);
}

#endif // QTVIEWER_H
