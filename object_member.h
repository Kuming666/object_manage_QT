#ifndef OBJECT_MEMBER_H
#define OBJECT_MEMBER_H

#include <QString>
#include <QTextStream>

class object_member
{
public:
    object_member();
    object_member(const object_member &om);
    virtual ~object_member();

    object_member& operator= (const object_member &om);

    void save_object_member(QTextStream &out);
    void read_object_member(QTextStream &in);

    int member_number;
    QString member_name;
    QString member_type;
    int byte_number;
    QString member_access;
};

#endif // OBJECT_MEMBER_H
