#pragma once

#include "meta/meta_property.h"
#include <iostream>

namespace eos
{

namespace {
struct User
{
    meta_property<std::string, decltype("name"_cts)> name;
    meta_property<int, EOS_STRING(age)> age;
};


struct UserWithMacro
{
    EOS_PROPERTY(std::string, name);
    EOS_PROPERTY(int, age) = {18};
};
}

class Example_1
{
public:
    static void run()
    {
        User user{"Volodymyr", 31};
        std::cout << user.name.meta_property_name()
                  << ": " << user.name
                  << ", " << user.age.meta_property_name()
                  << ": " << user.age
                  << std::endl;


        UserWithMacro user2{"Olesia", 28};
        std::cout << user2.name.meta_property_name()
                  << ": " << user2.name
                  << ", " << user2.age.meta_property_name()
                  << ": " << user2.age
                  << std::endl;

    }
};

}
