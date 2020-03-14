#include "serializer/TSerializer.h"
#include "serializer/NlohmannJSON.h"
#include "serializer/StdOstream.h"
#include "serializer/StdString.h"
#include "serializer/QtSqlInsertSerializer.h"
#include "serializer/QtSqlSelectSerializer.h"

#include "serializable/Serializable.h"

#include "utils/Utils.h"
#include "utils/CompareUtils.h"

#include <iostream>
#include <map>
#include <vector>
#include <set>

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>


using namespace std;
using namespace serializer;

struct User
{
    SERIALIZABLE(int, id);
    SERIALIZABLE(string, login);
    SERIALIZABLE(string, password);
    SERIALIZABLE(bool, activated);
    SERIALIZABLE_VAL(unsigned int, timestamp, 1582359281);

    GITS_SERIALIZABLE_STRUCT
};

struct Value
{
    SERIALIZABLE(int, id);
    SERIALIZABLE(std::string, name);

    GITS_SERIALIZABLE_STRUCT
};

bool operator<(Value lhs, Value rhs)
{
    return lhs.id < rhs.id;
}

using T = std::map<std::string, int>;
struct ComplexStruct
{
    int tmp2;
    SERIALIZABLE(int, id);
    SERIALIZABLE(string, name);
    SERIALIZABLE(std::vector<std::string>, names);
    SERIALIZABLE(std::vector<User>, users);
    SERIALIZABLE(std::set<int>, ids);
    SERIALIZABLE(T, data); // templates types like std::map should be aliased due to comma inside a template args list
    SERIALIZABLE(User, user);
    int tmp; // non-serializable
};

namespace Factory
{
    User createUser()
    {
        User user = {1, std::string("user_login"), std::string("secure_password"), true, 1582359281};
        return user;
    }

    ComplexStruct createComplex()
    {
        ComplexStruct complex;
        complex.id = 22;
        complex.name = "hello";
        complex.names = {"ivan", "kolya", "tolya"};
        complex.data = {{"test", 1}, {"qwerty", 2}};
        complex.ids = {4,3,2,7};
        complex.users = { Factory::createUser(), Factory::createUser() };

        return complex;
    }
}

/////////////
/////////////
/////////////

void test_nlohmann_serializer()
{
    {
        User user = Factory::createUser();
        nlohmann::json lohmannObj;
        Serializer<NlohmannJSON>::serialize(lohmannObj, user);
        std::cout << "[" << __PRETTY_FUNCTION__ << " serialized user] " << lohmannObj << std::endl;
    }

    {
        auto complex = Factory::createComplex();
        Serializer<StdOstream>::serialize(std::cout, complex);
        nlohmann::json lohmannObj;
        Serializer<NlohmannJSON>::serialize(lohmannObj, complex);
        std::cout << "[" << __PRETTY_FUNCTION__ << "]\n" << lohmannObj << std::endl;
        std::cout << "\n\n";

        ComplexStruct res;
        Serializer<StdOstream>::serialize(std::cout, res);
        std::cout << "\n\n";
        std::cout << "ARE EQUAL: " << utils::AreEquals(complex, res) << std::endl;

        Serializer<NlohmannJSON>::deserialize(lohmannObj, res);
        Serializer<StdOstream>::serialize(std::cout, res);
        std::cout << "ARE EQUAL: " << utils::AreEquals(complex, res) << std::endl;
    }
}


void test_stdostream_serializer()
{
    User user;
    user.id = 11;
    user.login = "Tod";
    user.password = "##@$***";

    Serializer<StdOstream>::serialize(std::cout << "[" << __PRETTY_FUNCTION__ << " serialized user] ", user);

    User user2 = Factory::createUser();
    Serializer<StdOstream>::serialize(std::cout << "[" << __PRETTY_FUNCTION__ << " serialized user2] ", user2);
}

void test_stdstring_serializer()
{
    User user = Factory::createUser();
    std::string str;
    Serializer<StdString>::serialize(str, user);
    std::cout << "[" << __PRETTY_FUNCTION__ << " serialized user] " << str << std::endl;
}


/*
void test_qtsql_insert_serializer()
{
    User user = Factory::createUser();

    std::string result;
    Serializer<QtSqlInsert>::serialize(result, user);
    std::cout << result << std::endl;
}

void test_qtsql_select_deser()
{
    auto db = "/Users/skident/Documents/Projects.nosync/Experimental/GitsSerializer/db/db.db";
    auto m_dbase = QSqlDatabase::addDatabase("QSQLITE");
    m_dbase.setDatabaseName(db);
    bool res = m_dbase.open();
    if (!res)
        qDebug() << "Can't open database";

    User user = Factory::createUser();

    QString request = "SELECT * FROM users WHERE activated=1";
    QSqlQuery query;
    if (!query.exec(request))
    {
        qDebug() << "[Failed]"
                 << "'" << query.lastError() << "'"
                 << request;
        return;
    }

    std::list<User> users;
    QtSqlSelect::deserialize_all(query, users);

    std::cout << "users count = " << users.size() << std::endl;
    for (auto user : users)
        Serializer<StdOstream>::serialize(std::cout, user);
}
*/


void test()
{
    test_stdostream_serializer();
    test_stdstring_serializer();
    test_nlohmann_serializer();

//    test_qtsql_insert_serializer();
//    test_qtsql_select_deser();
}


void createTserializableManually()
{
    TSerializable<int, SerializableName<decltype("tmp"_gitsstr)>> tmp = 11;
    std::cout << __FUNCTION__ << " " << tmp.value << " " << tmp.name.GetString() << std::endl;

    using T = std::map<std::string, int>;
    TSerializable<T, SerializableName<decltype("tmp"_gitsstr)>> tmp2;
}



int main()
{
    test();

    return 0;
}



