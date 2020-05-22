#pragma once

#include <string>

class SqlGroupBy
{
public:
    SqlGroupBy() = default;

    SqlGroupBy(const std::string& inColumn)
    {
        groupBy(inColumn);
    }

    template <class ...T>
    SqlGroupBy(const std::string& inColumn, const T& ...inColumns)
    {
        groupBy(inColumn, inColumns...);
    }


    template <class ...T>
    SqlGroupBy& groupBy(const std::string& inColumn, const T&... inColumns)
    {
        groupBy(inColumn);
        groupBy(inColumns...);
        return *this;
    }

    SqlGroupBy& groupBy(const std::string& inColumn)
    {
        if (mGroupBy.empty())
            mGroupBy = " GROUP BY ";
        else
            mGroupBy += ", ";

        mGroupBy += inColumn;

        return *this;
    }

    std::string get() const
    {
        return mGroupBy;
    }

private:
    std::string mGroupBy;
};

