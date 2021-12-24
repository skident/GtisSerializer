#pragma once

#include "meta/meta_property.h"
#include "meta/stringifier.h"
#include "meta/comparator.h"

class Example_4
{
public:
    static void run()
    {
        struct MenuItem {
            EOS_PROPERTY(std::string, title);
            EOS_PROPERTY(std::string, icon);
            EOS_PROPERTY(int, id);
            EOS_PROPERTY(int, action_id);
            EOS_PROPERTY(bool, visible);

            EOS_OBJECT
        };

        MenuItem item;
        MenuItem item2;

        if (eos::meta::are_equals(item, item2)) {

        }

        auto res = eos::meta::stringifier::to_string(item);
        std::cout << "res: " << res << std::endl;
    }
};
