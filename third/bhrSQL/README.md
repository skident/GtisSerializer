![C/C++ CI](https://github.com/skident/BhrSQL/workflows/C/C++%20CI/badge.svg?branch=master)

# BhrSQL

## Aim
Helper for creating SQL requests
```
template <class InSerializable>
static void serialize(OutSerialized& outSerialized, const InSerializable& inSerializable)
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
2. types - classes, macro and utils for a compile time reflection  
3. serializer - classes for serialization: NlohmannJSON, StdString, StdOstream. TSerializer is kind of base class for all serializers (it is no necessary to use it)
4. utils - util for converting std::tuple to structure
5. main.cpp - examples of serialization and deserialization (if possible).

## Build system:
Cmake

## C++ standard:
C++17

