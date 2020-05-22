#pragma once

#include "types/Serializable.h"
#include "serializer/SqliteSerializer.h"
#include "bhrSQL/SqlCreateTable.h"
#include <iostream>
#include <list>

namespace bhr::example3
{

struct User
{
    BHR_TYPE_FLAGS(int, id, sql::FLAG_NPAU) = {0};
    BHR_TYPE(std::string, name);
    BHR_TYPE_INITED(int, age, 18);

    BHR_SERIALIZABLE_STRUCT
};

struct UserInfo
{
    BHR_TYPE_FLAGS(int, id, sql::FLAG_NPAU) = {0};
    BHR_TYPE(int, user_id);
    BHR_TYPE(std::string, email);

    BHR_SERIALIZABLE_STRUCT
};

struct ProgramItem
{
    BHR_TYPE_INITED(int, id                      , 0   );
    BHR_TYPE_INITED(int, remoteId                , 0   );
    BHR_TYPE_INITED(int, userId                  , 0   );
    BHR_TYPE_INITED(int, programId               , 0   );
    BHR_TYPE_INITED(int, machineId               , 0   );
    BHR_TYPE_INITED(int, sets                    , 4   );
    BHR_TYPE_INITED(double, setsTimeoutMin       , 2.  );
    BHR_TYPE_INITED(double, machineTimeoutMin    , 2.  );
    BHR_TYPE_INITED(unsigned int, timestamp,    1243);
    //    SERIALIZABLE(cmn::Machine, machine);

    BHR_SERIALIZABLE_STRUCT
};


struct Program
{
    BHR_TYPE(int, id);
    BHR_TYPE(int, remoteId);
    BHR_TYPE(int, userId);
    BHR_TYPE(std::string, name);
    BHR_TYPE(std::string, description);
    BHR_TYPE(unsigned int, timestamp) = {123456};
//    BHR_TYPE(std::list<ProgramItem>, items);

    BHR_SERIALIZABLE_STRUCT
};

/*
std::string From(const std::string& inLeft)
{
    return "FROM " + inLeft;
}

std::string From(const std::string& inLeft, const std::string& inRight)
{
    return From(inLeft + ", " + inRight);
}

std::string Join(const std::string& inLeft, const std::string& inRight, const std::string& inJoinType, const std::string& inOn)
{
    return inLeft + " " + inJoinType + " JOIN " + inRight + " " + inOn;
}

std::string LeftJoin(const std::string& inLeft, const std::string& inRight, const std::string& inOn)
{
    return Join(inLeft, inRight, "LEFT", inOn);
}

std::string RightJoin(const std::string& inLeft, const std::string& inRight,  const std::string& inOn)
{
    return Join(inLeft, inRight, "RIGHT", inOn);
}

std::string InnerJoin(const std::string& inLeft, const std::string& inRight, const std::string& inOn)
{
    return Join(inLeft, inRight, "INNER", inOn);
}

std::string On(const std::string& inLeft, const std::string& inRight, const std::string& inOperation)
{
    return "ON " + inLeft + inOperation + inRight;
}

std::string Condition(const std::string& inLeft, const std::string& inCondition, const std::string& inRight)
{
    return inLeft + " " + inCondition + " " + inRight;
}

std::string Or(const std::string& inLeftCondition, const std::string& inRightCondition, bool inWrapWithParentheses = true)
{
    return (inWrapWithParentheses ? "(" : "") + inLeftCondition + " OR " + inRightCondition + (inWrapWithParentheses ? ")" : "");
}

std::string And(const std::string& inLeftCondition, const std::string& inRightCondition, bool inWrapWithParentheses = true)
{
    return (inWrapWithParentheses ? "(" : "") + inLeftCondition + " AND " + inRightCondition + (inWrapWithParentheses ? ")" : "");
}

std::string Where(const std::string& inLeftCondition, const std::string& inOperation, const std::string& inRightCondition)
{
    return "WHERE " + inLeftCondition + " " + inOperation + " " + inRightCondition;
}

std::string Where(const std::string& inCondition)
{
    return "WHERE " + inCondition;
}
*/

class Example_3
{
public:
    static void run()
    {
        if (0)
        {
            User user{1, "Volodymyr", 31};
            auto user2 = user;
            user2.age -= 1;

            std::string result;
            serializer::SqliteSerialzier::create("name", result, user);
            std::cout << "CREATE RESULT: " << result << std::endl;

            result.clear();
            serializer::SqliteSerialzier::update("name", result, user, user2);
            std::cout << "UPDATE RESULT: " << result << std::endl;

            result.clear();
            serializer::SqliteSerialzier::select(std::vector<std::string>{"name", "name2"},
                                                 result,
                                                 std::make_tuple(user.id, user.name),
                                                 std::make_tuple(user.age, user.name));
            std::cout << "SELECT RESULT: " << result << std::endl;
        }

        {
            Program program;
            std::string result;
            serializer::SqliteSerialzier::create("name", result, program);
            std::cout << "CREATE RESULT: " << result << std::endl;
        }

        {
            Program program;
            std::string result;
            serializer::SqliteSerialzier::insert("name", result, program);
            std::cout << "INSERT RESULT: " << result << std::endl;
        }
    }
};

}
