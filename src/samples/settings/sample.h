#include <iostream>
#include "settings/any_property_set.h"
#include "settings/settings.h"

namespace eos::samples::settings {

using namespace eos;

void simpleTest()
{
    eos::any any;
    any.set(27);
    auto v = any.get<std::string>();
    std::cout << "V: " << v << std::endl;

    any.set("3.14");
    std::cout << "V2: " << any.get<double>() << std::endl;
    std::cout << "V3: " << any.get<int>() << std::endl;
}

void propertySetTest()
{
    eos::any_property_set props;
    props.set("char", 'c');
    props.set("bool", true);
    props.set("int32", 32);
    props.set("int64", 64);
    props.set("double", 3.14);
    props.set("string", "my string");

    for (const auto& it : props)
    {
        std::cout << it.first << ", " << it.second.get<std::string>() << std::endl;
    }
}

void propertiesFileSave()
{
    eos::settings props({"test.json"});
    props.set("char", 'c');
    props.set("bool", true);
    props.set("int32", 32);
    props.set("int64", 64);
    props.set("double", 3.14);
    props.set("string", "ur string");

    props.save();
}

void propertiesFileLoad()
{
    eos::settings props({"test.json"});
    props.dump();
}

int run()
{
//    simpleTest();
//    propertySetTest();
    propertiesFileSave();
    propertiesFileLoad();

    return 0;
}

}
