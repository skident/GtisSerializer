#pragma once

#include <string>
#include <vector>

#include "SqlCondition.h"

namespace bhr::sql
{

class SqlFrom
{
public:
    SqlFrom() = default;

    SqlFrom(const std::string& inTableName)
    {
        mStatement = " FROM " + inTableName;
    }

    SqlFrom(const std::vector<std::string> inTablesNames)
    {
        std::string res;
        for (auto table : inTablesNames) {
            if (!res.empty())
                res += ", ";
            res += table;
        }
        mStatement = " FROM " + res;
    }

    SqlFrom& Join(const std::string& inTableName, const std::string& inJoinType)
    {
        mStatement = get() + " " + inJoinType + " JOIN " + inTableName;
        return *this;
    }

    SqlFrom& LeftJoin(const std::string& inTableName)
    {
        return Join(inTableName, "LEFT");
    }

    SqlFrom& RightJoin(const std::string& inTableName)
    {
        return Join(inTableName, "RIGHT");
    }

    SqlFrom& InnerJoin(const std::string& inTableName)
    {
        return Join(inTableName, "INNER");
    }

    SqlFrom& On(const SqlCondition& inCondition)
    {
        mStatement = get() + " ON " + inCondition.get();
        return *this;
    }

    std::string get()
    {
        return mStatement;
    }

private:
    std::string mStatement;
};

}
