#pragma once

#include <string>

namespace bhr::sql
{

class SqlOrderBy
{
public:
    SqlOrderBy() = default;

    SqlOrderBy(const std::string& inColumnName, bool inAsc = true)
    {
        orderBy(inColumnName, inAsc);
    }

    SqlOrderBy& orderBy(const std::string& inColumnName, bool inAsc = true)
    {
        if (mOrderBy.empty())
            mOrderBy = " ORDER BY ";
        else
            mOrderBy += ", ";

        mOrderBy += inColumnName + " " + (inAsc ? "ASC" : "DESC");

        return *this;
    }

    SqlOrderBy& orderByAsc(const std::string& inColumnName)
    {
        return orderBy(inColumnName, true);
    }

    SqlOrderBy& orderByDesc(const std::string& inColumnName)
    {
        return orderBy(inColumnName, false);
    }

    std::string get() const
    {
        return mOrderBy;
    }

private:
    std::string mOrderBy;
};

}
