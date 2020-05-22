#pragma once

#include <string>
#include <vector>
#include <optional>
#include <sstream>

namespace bhr::sql
{

class SqlAlterTable
{
public:
    SqlAlterTable(const std::string& inTableName)
        : mTables({inTableName})
    {
        assert(!mTables.empty());
    }

    SqlAlterTable(const std::vector<std::string>& inTablesNames)
        : mTables(inTablesNames)
    {
        assert(!mTables.empty());
    }

    ~SqlAlterTable() = default;

    SqlAlterTable& add(const std::string& inColumnName, const std::string& inColumnType)
    {
        mOperations.push_back({Operation::Add, inColumnName, inColumnType});
        return *this;
    }

    template<class T>
    SqlAlterTable& add(const std::string& inColumnName, const std::string& inColumnType, const T& inDefaultValue)
    {
        std::stringstream ss;
        ss << inDefaultValue;
        mOperations.push_back({Operation::Add, inColumnName, inColumnType, ss.str()});
        return *this;
    }

    SqlAlterTable& drop(const std::string& inColumnName)
    {
        mOperations.push_back({Operation::Drop, inColumnName});
        return *this;
    }

    SqlAlterTable& modify(const std::string& inColumnName, const std::string& inColumnType)
    {
        mOperations.push_back({Operation::Modify, inColumnName, inColumnType});
        return *this;
    }

    std::string get() const
    {
        std::stringstream ss;
        for (const auto& table : mTables)
        {
            for (const auto& op : mOperations)
            {
                ss << op.get(table);
            }
        }
        return ss.str();
    }

    operator std::string()
    {
        return get();
    }

private:
    struct Operation
    {
        enum Action
        {
            Add,
            Drop,
            Modify
        };

        Action action;
        std::string column;
        std::optional<std::string> type = {};
        std::optional<std::string> defaultValue = {};


        std::string get(const std::string& table) const
        {
            std::stringstream ss;

            ss << "ALTER TABLE " << table << " ";
            if (action == Action::Drop)
                ss << "DROP";
            else if (action == Action::Modify)
                ss << "MODIFY";
            else
                ss << "ADD";

            ss << " COLUMN " << column;
            if (type)
                ss << " " << *type;
            if (defaultValue)
                ss << " DEFAULT " << *defaultValue;

            ss << ";";

            return ss.str();
        }
    };

    std::vector<std::string> mTables;
    std::vector<Operation> mOperations;
};

}
