#pragma once

#include "types/Serializable.h"
#include "serializer/Json.h"
#include <iostream>
#include "eos.h"

namespace eos::example2
{

struct Location
{
    EOS_PROPERTY(double, latitude);
    EOS_PROPERTY(double, longitude);
    EOS_PROPERTY(int, altitude);

    EOS_OBJECT
};

struct NamedLocation
{
    EOS_PROPERTY(std::string, name);
    EOS_PROPERTY(Location, location);

    EOS_OBJECT
};

struct Person
{
    EOS_PROPERTY(std::string, name);
    EOS_PROPERTY(int, age);

    EOS_OBJECT
};


struct Trip
{
    EOS_PROPERTY(std::string, title);
    EOS_PROPERTY(Person, person);
    EOS_PROPERTY(std::vector<NamedLocation>, route);

    EOS_OBJECT
};

class Example_2
{
public:
    static void run()
    {
        Trip user = {"my trip",
                     Person{"Volodymyr", 31},
                     std::vector<NamedLocation>{
                         NamedLocation{"home", Location{50.437199, 30.522338430913013, 143}},
                         NamedLocation{"office", Location{50.5016668, 30.4930195, 100}}
                     }
                    };

        std::cout << eos::toJson(user) << std::endl;
    }
};

}
