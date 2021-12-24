#pragma once

#include "any_property_set.h"
#include <fstream>
#include <filesystem>
#include <random>
#include "nlohmann/json.hpp"

namespace eos {
class settings : public eos::any_property_set
{
public:
    struct file_options
    {
        std::string filename;
        int saveTimeoutMs = 0;

        bool use_file() const { return !filename.empty(); }
    };

    settings(const file_options& inOptions)
        : mOptions(inOptions)
    {
        load();
    }

    ~settings()
    {
        save();
    }

    void save()
    {
        nlohmann::json result;

        for (const auto& [key, value] : *this)
        {
            switch (value.type())
            {
            case eType_Bool:
                result[key] = value.get<bool>();
                break;

            case eType_Char:
                result[key] = value.get<char>();
                break;

            case eType_Int32:
                result[key] = value.get<int32_t>();
                break;

            case eType_Int64:
                result[key] = value.get<int64_t>();
                break;

            case eType_Double:
                result[key] = value.get<double>();
                break;

            case eType_String:
                result[key] = value.get<std::string>();
                break;

            default:
                break;
            }
        }

        if (mOptions.use_file())
        {
            auto tmpFileName = temp_directory_path() + uuid();
            std::fstream fs(tmpFileName, std::ios::out);
            fs << result.dump();
            fs.close();

            std::filesystem::rename(tmpFileName, mOptions.filename);
        }
    }

    void load()
    {
        if (!mOptions.use_file())
            return;

        std::fstream fs(mOptions.filename, std::ios::in);
        if (fs.is_open())
        {
            nlohmann::json result = nlohmann::json::parse(fs);

            for (const auto& [key, value]  : result.items())
            {
                if (value.is_string()) {
                    set(key, value.get<std::string>());
                }
                else if (value.is_number_float()) {
                    set(key, value.get<double>());
                }
                else if (value.is_number_integer()) {
                    set(key, value.get<int>());
                }
                else if (value.is_boolean()) {
                    set(key, value.get<bool>());
                }
                else {
                    std::cerr << "UNKNOWN TYPE" << std::endl;
                }
            }
        }
    }

    void dump()
    {
        std::cout << "=============" << std::endl;
        for (const auto& it : *this)
        {
            std::cout << it.first << ", " << it.second.get<std::string>() << ", " << it.second.type() << std::endl;
        }
        std::cout << "=============" << std::endl;
    }

    void propertyChanged(const std::string& /*inKey*/) override
    {
        save(); // TODO: use a timer to aviod extra I/O operations
    }

private:
    std::string temp_directory_path()
    {
        return std::filesystem::temp_directory_path();
    }

    std::string uuid()
    {
        using namespace std;
        static random_device dev;
        static mt19937 rng(dev());

        uniform_int_distribution<int> dist(0, 15);

        const char *v = "0123456789abcdef";
        const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

        string res;
        for (int i = 0; i < 16; i++) {
            if (dash[i]) res += "-";
            res += v[dist(rng)];
            res += v[dist(rng)];
        }
        return res;
    }

private:
    file_options mOptions;
};
}
