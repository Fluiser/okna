#ifndef EDITFORM_H
#define EDITFORM_H

#include <QTabWidget>
#include <QWidget>

class EditForm : public QWidget
{
    Q_OBJECT

public:
    QTabWidget *_parent;

    EditForm(QWidget *p)
        : QWidget(p)
        , _parent((QTabWidget *) p)
    {}

public slots:
    virtual void close(){};
};

class StaticEditForm : public EditForm
{
    Q_OBJECT

public:
    using EditForm::_parent;

    StaticEditForm(QWidget *p)
        : EditForm(p)
    {}
};

#endif // EDITFORM_H
