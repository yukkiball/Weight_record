#ifndef DAY_H
#define DAY_H
#include <QLineEdit>

class Day
{
private:
    QString date;
    QString morning;
    QString night;
public:
    Day(QString d = "1月1日", QString m = "0", QString n = "0"): date(d), morning(m), night(n){}
    void get_info();
    QString get_date(){return date;}
    QString get_morning(){return morning;}
    QString get_night(){return night;}
    QLineEdit * l1;
    QLineEdit * l2;
    QLineEdit * l3;
    QLineEdit * l4;
};

#endif // DAY_H
