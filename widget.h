#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "day.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    static Day myday[366];
    static int sum_day;
    static QSqlDatabase database;
    QSqlTableModel *model;

private slots:
    void show_gra();
    void show_data();
    void input();
    void clear();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
