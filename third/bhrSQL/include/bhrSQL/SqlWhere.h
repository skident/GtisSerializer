#pragma once

#include <string>
#include <sstream>
#include "SqlCondition.h"

namespace bhr::sql
{

class SqlWhere
{
public:
    SqlWhere() {}

    SqlWhere(const SqlWhere& rhs)
    {
        mCondition = rhs.mCondition;
    }

    SqlWhere& operator=(const SqlWhere& rhs)
    {
        mCondition = rhs.mCondition;
        return *this;
    }

    SqlWhere(const SqlCondition& inCondition)
    {
        mCondition = inCondition;
    }

    template<class T>
    SqlWhere(const std::string& inColumnName,
            const std::string& inComparator,
            const T& inValue)
    {
        mCondition = SqlCondition(inColumnName, inComparator, inValue);
    }

    SqlWhere& And(const SqlCondition& inCondition)
    {
        mCondition.And(inCondition);
        return *this;
    }

    SqlWhere& Or(const SqlCondition& inCondition)
    {
        mCondition.Or(inCondition);
        return *this;
    }

    std::string get() const
    {
        std::string where = mCondition.get();
        if (!where.empty())
            return " WHERE " + where;
        return "";
    }

private:
    SqlCondition mCondition;
};

}
