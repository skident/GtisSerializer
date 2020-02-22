#include "serializer/TSerializer.h"
#include "serializer/QtJSON.h"
#include "serializer/NlohmannJSON.h"
#include "serializer/StdOstream.h"
#include "serializer/StdString.h"

#include "serializable/Serializable.h"

#include "utils/Utils.h"
#include <iostream>
#include <QDebug>

using namespace std;
using namespace serializer;

struct User
{
    SERIALIZABLE(int, id);
    SERIALIZABLE(string, login);
    SERIALIZABLE(string, password);
    SERIALIZABLE(bool, activated);
    SERIALIZABLE_VAL(unsigned int, timestamp, 1582359281);
};

namespace Factory
{
    User createUser()
    {
        User user = {1, std::string("user_login"), std::string("secure_password"), true, 1582359281};
        return user;
    }
}

/////////////
/////////////
/////////////

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

void test_nlohmann_serializer()
{
    User user = Factory::createUser();
    nlohmann::json lohmannObj;
    Serializer<NlohmannJSON>::serialize(lohmannObj, user);
    std::cout << "[" << __PRETTY_FUNCTION__ << " serialized user] " << lohmannObj << std::endl;

    User user2;
    Serializer<NlohmannJSON>::deserialize(lohmannObj, user2);
    Serializer<StdOstream>::serialize(std::cout << "[" << __PRETTY_FUNCTION__ << " deserialized user] ", user2);
}

void test_qtjson_serializer()
{
    User user = Factory::createUser();

    QJsonObject obj;
    Serializer<QtJSON>::serialize(obj, user);
    qDebug() << "[" << __PRETTY_FUNCTION__ << " serialized user] " << obj;

    User user2;
    Serializer<QtJSON>::deserialize(obj, user2);
    Serializer<StdOstream>::serialize(std::cout << "[" << __PRETTY_FUNCTION__ << " deserialized user] ", user2);
}

void test()
{
    test_stdostream_serializer();
    test_stdstring_serializer();

    test_nlohmann_serializer();
    test_qtjson_serializer();
}


void createTserializableManually()
{
    TSerializable<int, SerializableName<decltype("tmp"_gitsstr)>> tmp = 11;
    std::cout << __FUNCTION__ << " " << tmp.value << " " << tmp.name.GetString() << std::endl;
}


int main()
{
    test();
    createTserializableManually();

    return 0;
}



