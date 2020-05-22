#pragma once

#include "bhrSQL/SqlDelete.h"
#include <iostream>

#define EXPECT_EQ(lhs, rhs) if ((lhs) != (rhs)) std::cerr << "VALUES '" << lhs  << "' AND '" << rhs << "' ARE NOT EQUAL! " << __FILE__ << ":" << __LINE__ << std::endl;
#define EXPECT(lhs, op, rhs) if ((lhs) op (rhs)) std::cerr << "VALUES '" << lhs  << "' AND '" << rhs << "' ARE NOT EQUAL! " << __FILE__ << ":" << __LINE__ << std::endl;

class SqlDeleteTest
{
public:
    static void run()
    {
        using namespace bhr::sql;

        EXPECT_EQ(SqlDelete("table").get(), "DELETE FROM table");
        EXPECT(SqlDelete("table").get(), <, "DELETE FROM table");
    }
};
