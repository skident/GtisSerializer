#pragma once

#include "types/Serializable.h"
#include "serializer/Json.h"
#include <iostream>
#include <string>

namespace eos::example3
{

struct Config {
    struct MenuItem {
        EOS_PROPERTY(std::string, title);
        EOS_PROPERTY(std::string, icon);
        EOS_PROPERTY(int, id);
        EOS_PROPERTY(int, action_id);
        EOS_PROPERTY(bool, visible);

        EOS_OBJECT
    };

    struct AppCfg {
        EOS_PROPERTY(std::string, name);
        EOS_PROPERTY(std::string, version);
        EOS_PROPERTY(std::string, plugins_path);
        EOS_PROPERTY(std::vector<MenuItem>, main_menu);

        int foo() { return 1;}

        EOS_OBJECT
    };

    struct NetworkAddress {
        EOS_PROPERTY(std::string, address);
        EOS_PROPERTY(std::string, port);

        EOS_OBJECT
    };

    struct RestApiCfg {
        EOS_PROPERTY(NetworkAddress, remote);
        EOS_PROPERTY(NetworkAddress, local);

        EOS_OBJECT
    };

    EOS_PROPERTY(Config::AppCfg, app);
    EOS_PROPERTY(Config::RestApiCfg, restapi); // FIXME: nested SERIALIZABLE structures are causing crash on exit
};


class Example_3
{
private:
    Config config;
public:
    Example_3 ()
    {
        init();
    }

    void init()
    {
        std::string json = R"({
        "app": {
            "name": "GymAssistant",
            "version": "0.0.1",
            "plugins_path": "/Users/admin/Documents/QmlPlugins/macos",
            "main_menu": [
                {
                    "id": 1,
                    "action_id": 3,
                    "title": "workouts",
                    "icon": "workouts.svg",
                     "visible": true
                },
                {
                    "id": 2,
                    "action_id": 2,
                    "title": "Home",
                    "icon": "home.svg",
                     "visible": true
                }
            ]
        },
        "restapi": {
            "remote": {
                "address": "127.0.0.1",
                "port": "80"
            },
            "local": {
                "address": "127.0.0.1",
                "port": "80"
            }
        }
    }
)";



        auto jsonObj = nlohmann::json::parse(json);
        serializer::Json::fromJson(jsonObj, config);
    }

    void run()
    {
        std::cout << config.restapi->remote->address << std::endl;
        auto& title = config.app->main_menu[0].title;
        std::cout << title << std::endl;
        title = "Workouts";
        std::cout << title << std::endl;
    }
};

}
