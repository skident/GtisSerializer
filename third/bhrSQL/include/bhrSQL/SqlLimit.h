#pragma once

#include <string>

namespace bhr::sql
{

class SqlLimit
{
public:
    SqlLimit() = default;

    SqlLimit(int inLimit, int inOffset = 0)
    {
        limit(inLimit, inOffset);
    }

    SqlLimit& limit(int inLimit, int inOffset = 0)
    {
        if (inLimit > 0)
        {
            mStatement = " LIMIT " + std::to_string(inLimit);

            if (inOffset)
                mStatement += " OFFSET " + std::to_string(inOffset);
        }

        return *this;
    }

    std::string get() const
    {
        return mStatement;
    }

private:
    std::string mStatement;
};

}
