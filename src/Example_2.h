#pragma once

#include "types/Serializable.h"
#include "serializer/NlohmannJSON.h"
#include <iostream>

namespace bhr::example2
{

struct Location
{
    BHR_TYPE(double, latitude);
    BHR_TYPE(double, longitude);
    BHR_TYPE(int, altitude);

    BHR_SERIALIZABLE_STRUCT
};

struct NamedLocation
{
    BHR_TYPE(std::string, name);
    BHR_TYPE(Location, location);

    BHR_SERIALIZABLE_STRUCT
};

struct Person
{
    BHR_TYPE(std::string, name);
    BHR_TYPE(int, age);

    BHR_SERIALIZABLE_STRUCT
};


struct Trip
{
    BHR_TYPE(std::string, title);
    BHR_TYPE(Person, person);
    BHR_TYPE(std::vector<NamedLocation>, route);

    BHR_SERIALIZABLE_STRUCT
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

        std::cout << serializer::NlohmannSerializer::serialize(user) << std::endl;
    }
};

}
