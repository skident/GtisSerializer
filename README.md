![C/C++ CI](https://github.com/skident/GtisSerializer/workflows/C/C++%20CI/badge.svg?branch=master)

# GitsSerializer

## Aim
Easy serialization and deserialization of simple data structures: to/from JSON, to std::string and so on.
Any new serializer can be added just by implementing a static serialize method
```
template <class InSerializable>
static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
```
For example you can implement `SqlInsert` serializer and `SqlSelect` serializer/deserializer, it will simplify your client code to something like this:
```
User user = {"my_login", "my_password"};
DataSource::get().insert(user); <--- serialization inside the 'insert' method based on which datasource is used (Sqlite, MySQL, or something else).
```

## Examples
I define next data structure
```
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
```

I want create a JSON object from this data structure to send it to the remote service
```
User user = Factory::createUser();
nlohmann::json lohmannObj;
Serializer<NlohmannJSON>::serialize(lohmannObj, user);
```
`lohmannObj` is ready for using.

let's deserialize it back:
```
User user2;
Serializer<NlohmannJSON>::deserialize(lohmannObj, user2);
```

Print results to console:
```
std::cout << "[serialized user] " << lohmannObj << std::endl;
Serializer<StdOstream>::serialize(std::cout << "[deserialized user] ", user2);
```

Console output:
```
[serialized user] {"activated":true,"id":1,"login":"user_login","password":"secure_password","timestamp":1582359281}
[deserialized user] id : 1;login : user_login;password : secure_password;activated : 1;timestamp : 1582359281;

```

## Thrid party libraries:
1. magic_get (boost::pfr) https://github.com/apolukhin/magic_get
2. nlohmann json https://github.com/nlohmann/json

## Solution structure
1. third - third party libraries
2. serializable - data structure(s) which can be serialized/deserialized 
3. serializer - classes for serialization: QtJSON, NlohmannJSON, StdString, StdOstream. TSerializer is kind of base class for all serializers (it is no necessary to use it)
4. utils - util for converting std::tuple to structure
5. main.cpp - examples of serialization and deserialization (if possible).

## Work environment:
1. IDE: Qt Creator 4.9.1
2. Qt version 5.12.3
3. C++17
4. macOS Catalina 10.15.x

## Qt modules:
1. core - Qt JSON module for serialization/deserialization  

