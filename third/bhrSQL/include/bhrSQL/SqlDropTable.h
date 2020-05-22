#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace bhr::sql
{

class SqlDropTable
{
public:
    SqlDropTable(const std::string& inTableName)
    {
        table(inTableName);
    }

    template <class ...T>
    SqlDropTable(const std::string& inTableName, const T&... inTablesNames)
    {
        table(inTableName, inTablesNames...);
    }

    ~SqlDropTable() {}

    SqlDropTable& table(const std::string& table)
    {
        mTables.push_back(table);
        return *this;
    }

    template <class ...T>
    SqlDropTable& table(const std::string& inTableName, const T&... inTablesNames)
    {
        table(inTableName);
        table(inTablesNames...);

        return *this;
    }

    std::string get() const
    {
        std::string statement;
        for (const auto& table : mTables)
        {
            statement += "DROP TABLE " + table + ";";
        }

        return statement;
    }

    operator std::string()
    {
        return get();
    }

private:
    std::vector<std::string> mTables;
};

}
