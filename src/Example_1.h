#pragma once

#include "types/Serializable.h"
#include <iostream>

namespace eos::example
{


struct User
{
    Property<std::string, decltype("name"_cts)> name;
    Property<int, EOS_STRING(age)> age;
};


struct UserWithMacro
{
    EOS_PROPERTY(std::string, name);
    EOS_PROPERTY(int, age) = {18};
};

class Example_1
{
public:
    static void run()
    {
        User user{"Volodymyr", 31};
        std::cout << user.name.GetName()
                  << ": " << user.name
                  << ", " << user.age.GetName()
                  << ": " << user.age
                  << std::endl;


        UserWithMacro user2{"Olesia", 28};
        std::cout << user2.name.GetName()
                  << ": " << user2.name
                  << ", " << user2.age.GetName()
                  << ": " << user2.age
                  << std::endl;

    }
};

}
