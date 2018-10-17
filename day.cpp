#include "day.h"

void Day::get_info()
{
    QString s1 = l1->text();
    QString s2 = l2->text();
    QString s3 = l3->text();
    QString s4 = l4->text();
    date = s1 + "月" + s4 + "日";
    morning = s2;
    night = s3;
}
