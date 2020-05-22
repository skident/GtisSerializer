#pragma once

#include <string>
#include <sstream>
#include "SqlWhere.h"

namespace bhr::sql
{

class SqlUpdate
{
public:
    SqlUpdate(const std::string& inTableName)
    {
        mStatement << "UPDATE " << inTableName;
    }

    ~SqlUpdate() {}

    template<class T>
    SqlUpdate& set(const std::string& inColumnName, const T& inValue)
    {
        if (mFields == 0)
            mStatement << " SET ";
        else
            mStatement << ", ";

        mStatement << inColumnName << "=" << "\"" << inValue << "\"";
        mFields++;
        return *this;
    }

    SqlUpdate& where(const SqlWhere& inWhere)
    {
        mWhere = inWhere;
        return *this;
    }

    template <class T>
    SqlUpdate& where(const std::string& inColumnName, const T& inValue)
    {
        return where(Equal(inColumnName, inValue));
    }

    std::string get() const
    {
        return mStatement.str() + mWhere.get();
    }

    operator std::string()
    {
        return get();
    }

private:
    std::stringstream mStatement;
    SqlWhere mWhere;
    int mFields = 0;
};

}
