#ifndef QT_H
#define QT_H
#include <QTableWidgetItem>

inline QTableWidgetItem *disableEdit(QTableWidgetItem *w)
{
    w->setFlags(w->flags() & ~Qt::ItemIsEditable);
    return w;
}

#endif // QT_H
