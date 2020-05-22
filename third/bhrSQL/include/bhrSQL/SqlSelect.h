#pragma once

#include <string>
#include <vector>
#include "SqlOrderBy.h"
#include "SqlGroupBy.h"
#include "SqlLimit.h"
#include "SqlFrom.h"
#include "SqlWhere.h"

namespace bhr::sql
{

class SqlSelect
{
public:
    SqlSelect(const std::string& inColumn = "")
    {
        if (!inColumn.empty())
            column(inColumn);
    }

    template <class... T>
    SqlSelect(const std::string& inColumn, const T&... inColumns)
    {
        column(inColumn, inColumns...);
    }

    ~SqlSelect() = default;

    SqlSelect& column(const std::string& inColumn)
    {
        mColumns.push_back(inColumn);
        return *this;
    }

    template <class... T>
    SqlSelect& column(const std::string& inColumn, const T&... inColumns)
    {
        column(inColumn);
        column(inColumns...);
        return *this;
    }

    SqlSelect& orderBy(const SqlOrderBy& inOrderBy)
    {
        mOrderBy = inOrderBy;
        return *this;
    }

    SqlSelect& orderBy(const std::string& inOrderBy)
    {
        mOrderBy.orderBy(inOrderBy);
        return *this;
    }

    SqlSelect& orderByDesc(const std::string& inOrderBy)
    {
        mOrderBy.orderBy(inOrderBy, false);
        return *this;
    }

    SqlSelect& limit(const SqlLimit& inLimit)
    {
        mLimit = inLimit;
        return *this;
    }

    SqlSelect& from(const SqlFrom& inFrom)
    {
        mFrom = inFrom;
        return *this;
    }

    SqlSelect& from(const std::string& inFrom)
    {
        mFrom = inFrom;
        return *this;
    }

    SqlSelect& where(const SqlWhere& inWhere)
    {
        mWhere = inWhere;
        return *this;
    }

    template <class T>
    SqlSelect& where(const std::string& inColumnName, const T& inValue)
    {
        mWhere.And(Equal(inColumnName, inValue));
        return *this;
    }

    SqlSelect& groupBy(const SqlGroupBy& inGroupBy)
    {
        mGroupBy.groupBy(inGroupBy.get());
        return *this;
    }

    SqlSelect& groupBy(const std::string& inGroupBy)
    {
        mGroupBy.groupBy(inGroupBy);
        return *this;
    }

    std::string get()
    {
        std::string columns;
        for (const auto& column : mColumns)
        {
            if (!columns.empty())
                columns += ", ";
            columns += column;
        }

        if (columns.empty())
            columns = "*";

        return "SELECT " + columns
                + mFrom.get()
                + mWhere.get()
                + mGroupBy.get()
                + mOrderBy.get()
                + mLimit.get();
    }

    operator std::string()
    {
        return get();
    }

private:
    std::vector<std::string> mColumns;
    SqlFrom mFrom;
    SqlWhere mWhere;
    SqlGroupBy mGroupBy;
    SqlOrderBy mOrderBy;
    SqlLimit mLimit;
};

}
