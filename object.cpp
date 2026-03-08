#include "object.h"

object::object()
{
    member_count = 0;
}

object::object(const object &ob)
{
    number = ob.number;
    name = ob.name;
    base_name = ob.base_name;
    function = ob.function;
    date = ob.date;
    author = ob.author;
    member = ob.member;
    member_count = ob.member_count;
}

object::~object()
{

}

object& object::operator=(const object &ob)
{
    if (this != &ob) // 增加自赋值检查，避免异常
    {
        number = ob.number;
        name = ob.name;
        base_name = ob.base_name;
        function = ob.function;
        date = ob.date;
        author = ob.author;
        member = ob.member;
        member_count = ob.member_count;
    }
    return *this;
}

void object::save_object(QTextStream &save)
{
    save << number <<"\t";
    save << name <<"\t";
    save << base_name <<"\n";
    save << function <<"\n";
    save << date.year() <<"\t";
    save << date.month() <<"\t";
    save << date.day() <<"\n";
    save << author <<"\n";
    save << member_count <<"\n";
    if(member_count == 0)
        save << "\n";
    for(int i=0; i<member_count; ++i)
    {
        member[i].save_object_member(save);
        if(i == member_count - 1)
            save << "\n";
    }
}

void object::read_object(QTextStream &read)
{
    int year,month,day;
    read >> number;
    read >> name;
    base_name = read.readLine().trimmed();
    function = read.readLine();
    read >> year;
    read >> month;
    read >> day;
    date.setDate(year, month, day);
    read.readLine();
    author = read.readLine();
    read >> member_count;
    for(int i=0; i<member_count; ++i)
    {
        object_member tempMember;
        tempMember.read_object_member(read);
        member.push_back(tempMember);
    }
}
