#pragma once

#include <string>
#include "SqlWhere.h"

namespace bhr::sql
{

class SqlDelete
{
public:
    SqlDelete(const std::string& inTableName)
    {
        mStatement = "DELETE FROM " + inTableName;
    }

    ~SqlDelete() {}

    SqlDelete& where(const SqlWhere& inWhere)
    {
        mStatement += inWhere.get();
        return *this;
    }

    template <class T>
    SqlDelete& where(const std::string& inColumnName, const T& inValue)
    {
        return where(Equal(inColumnName, inValue));
    }

    std::string get()
    {
        return mStatement;
    }

    operator std::string()
    {
        return get();
    }

private:
    std::string mStatement;
};

}
