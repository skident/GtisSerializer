#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace bhr::sql
{

class SqlInsert
{
public:
    SqlInsert(const std::string& inTableName)
    {
        mStatement = "INSERT INTO " + inTableName;
    }

    ~SqlInsert() = default;

    template<class T>
    SqlInsert& add(const std::string& inColumnName, const T& inValue)
    {
        std::stringstream ss;
        ss << inValue;
        std::string v = ss.str();

        std::stringstream ss1;
        ss1 << inColumnName;
        std::string f = ss1.str();

        mFieldValues.push_back({f, v});

        return *this;
    }

    std::string get() const
    {
        if (mFieldValues.empty())
            return "";

        std::string fields;
        std::string values;
        for (auto [f, v] : mFieldValues)
        {
            if (!fields.empty())
                fields += ", ";
            fields += "\"" + f + "\"";

            if (!values.empty())
                values += ", ";
            values += "\"" + v + "\"";
        }

        std::string tail  = "(" + fields + ") VALUES (" + values + ")";
        return mStatement + tail;
    }

    operator std::string()
    {
        return get();
    }

private:
    std::string mStatement;
    std::vector<std::pair<std::string, std::string>> mFieldValues;
};

}
