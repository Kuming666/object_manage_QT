#ifndef OBJECT_H
#define OBJECT_H

#include <QDate>
#include <QVector>

#include "object_member.h"

class object
{
public:
    object();
    object(const object &ob);
    ~object();

    object& operator= (const object &ob);

    void save_object(QTextStream &out);
    void read_object(QTextStream &in);

    int number;
    QString name;
    QString base_name;
    QString function;
    QDate date;
    QString author;
    QVector<object_member> member;
    int member_count;
};

#endif // OBJECT_H
