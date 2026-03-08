#include "object_member.h"

object_member::object_member()
{

}

object_member::object_member(const object_member &om)
{
    member_number = om.member_number;
    member_name =om.member_name;
    member_type =om.member_type;
    byte_number = om.byte_number;
    member_access = om.member_access;
}

object_member::~object_member()
{

}

object_member& object_member::operator=(const object_member &om)
{
    member_number = om.member_number;
    member_name =om.member_name;
    member_type =om.member_type;
    byte_number = om.byte_number;
    member_access = om.member_access;
    return *this;
}

void object_member::save_object_member(QTextStream &save)
{
    save << member_number << "\t";
    save << member_name << "\t";
    save << member_type << "\t";
    save << byte_number << "\t";
    save << member_access << "\n";
}

void object_member::read_object_member(QTextStream &read)
{
    read >> member_number;
    read >> member_name;
    read >> member_type;
    read >> byte_number;
    read >> member_access;
}
