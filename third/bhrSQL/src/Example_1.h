#pragma once

#include "bhrSQL/SqlSelect.h"
//#include "bhrSQL/SqlAlterTable.h"
#include "bhrSQL/SqlCondition.h"
#include "bhrSQL/SqlWhere.h"
#include "bhrSQL/SqlFrom.h"

#include "bhrSQL/SqlCreateTable.h"

#include <iostream>

namespace bhr::example
{



class Example_1
{
public:
    static void run()
    {
        using namespace bhr::sql;
        SqlCondition cond("id", "<", "20");
        SqlCondition cond2("id", ">", "40");
        cond.Or(cond2);

        SqlCondition c(SqlCondition("id < 10")
                       .Or(SqlCondition("name", "=", "skident"), true)
                       .And(SqlCondition("age", ">", "18")));

        std::cout << "CONDITION 1: " << cond.get() << std::endl;
        std::cout << "CONDITION 2: " << c.get() << std::endl;



        {
            SqlWhere where(And(GreaterThan("id", 0), LessEqual("id", 5), true).Or(Equal("name", "skident")));
            std::cout << "WHERE 1: " << where.get() << std::endl;
        }

        {
            auto from = SqlFrom("table1").LeftJoin("table2").On(Equal("table1.id", "table2.table1_id"));
            std::cout << "FROM 1: " << from.get() << std::endl;
        }

        {
            auto select = SqlSelect("col1", "col2", "col3")
                    .from(SqlFrom("tabl1").LeftJoin("table2").On(Equal("table1.id", "tabl2.table1.id")))
                    .where(SqlWhere(GreaterThan("id", 26)))
                    .orderBy(SqlOrderBy("id",true).orderByDesc("name"))
                    .limit(SqlLimit(20, 10))
                    .groupBy(SqlGroupBy("id", "name", "mail"))
                    ;
            std::cout << "SELECT 2: " << select.get() << std::endl;
        }

        {
            auto create = SqlCreateTable("Workout")
                    .column(SqlColumn("id", "INTEGER", "0", true, true, true, true))
                    .column(SqlColumn("date", "TEXT", ""))
                    .column(SqlColumn("start_time", "TEXT", ""))
                    .column(SqlColumn("end_time", "TEXT", ""))
                    .column(SqlColumn("name", "TEXT", ""))
                    .column(SqlColumn("description", "TEXT", ""))
                    .column(SqlColumn("timestamp", "INTEGER", "0"))
                    .column(SqlColumn("user_id", "INTEGER", 0))
                    .column(SqlColumn("program_id", "INTEGER", 0))
                    .column(SqlColumn("locker_number", "INTEGER", 0))
                    .column(SqlColumn("locked", "INTEGER", 0))
                    ;

            std::cout << "CREATE TABLE 1:\n" << create.get() << std::endl;
        }
    }
};

}
