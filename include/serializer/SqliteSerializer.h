#pragma once

#include "boost/pfr.hpp"

#include <tuple>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

#include "utils/Utils.h"
#include "types/Serializable.h"

#include <bhrSQL/SqlHelper.h>
#include <bhrSQL/SqlCreateTable.h>

namespace bhr::serializer
{

using namespace sql;

class SqliteSerialzier
{
    using OutSerialized = std::string;

public:

public:
    template <class InSerializable>
    static void create(const std::string& inTableName,
                          OutSerialized& outSerialized,
                          const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);

        SqlCreateTable createTable(inTableName);
        auto processor = [&createTable](const auto& inField) {
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                SqlColumn column(inField.GetName(),
                                 inField.GetValue(),
                                 inField.GetFlags(),
                                 inField.GetParams());

                createTable.column(column);
            }
        };

        std::apply([processor](auto&&... args) {
            (processor(args),...);
        }, tmpTuple);

        outSerialized = createTable.get();
    }

    template <class InSerializable>
    static void alter(const std::string& inTableName,
                       std::vector<std::string>& outSerialized,
                       const InSerializable& inSerializable,
                       const GitsVersion& inVersionFrom,
                       const GitsVersion& inVersionTo)
    {
        std::vector<std::string> fields;

        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);

        const std::string prefix = "ALTER TABLE " + inTableName + " ";

        auto processor = [&fields, prefix, inVersionFrom, inVersionTo](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                bool added = (inField.versionFrom > inVersionFrom);
                bool deleted = (inField.versionTo < inVersionTo && inField.versionTo != GitsVersion{0,0});

                if (added == deleted)
                {
                    return;
                }

                std::string result = prefix;
                result += added ? "ADD" : "DROP";
                result += " COLUMN ";
                result += "`";
                result += inField.GetName();
                result += "` ";

                using T = decltype(inField.value);
                if (std::is_integral<T>::value) {
                    result += "INTEGER";
                    if (inField.unique) {
                        result += " NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE";
                    }
                } else if (std::is_same<T, float>::value || std::is_same<T, double>::value) {
                    result += "REAL";
                    if (inField.unique) {
                        result += " NOT NULL PRIMARY KEY UNIQUE";
                    }
                } else if (std::is_same<T, std::string>::value) {
                    result += "TEXT";
                    if (inField.unique) {
                        result += " NOT NULL PRIMARY KEY UNIQUE";
                    }
                }
                // TODO: numeric and blob

                fields.push_back(result);
            }
        };

        std::apply([processor](auto&&... args) {
            (processor(args),...);
        }, tmpTuple);

        outSerialized = fields;
    }


    template <class InSerializable>
    static void insert(const std::string& inTableName,
                       OutSerialized& outSerialized,
                       const InSerializable& inSerializable)
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializable);

        SqlInsert insert(inTableName);

        auto processor = [&insert](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                insert.add(inField.GetName() , inField.GetValue());
            }
        };

        std::apply([processor](auto&&... args) {
            (processor(args),...);
        }, tmpTuple);

        outSerialized = insert.get();
    }

    template <class InSerializable>
    static void update(const std::string& inTableName,
                       OutSerialized& outSerialized,
                       const InSerializable& inSerializableOld,
                       const InSerializable& inSerializableNew
                       )
    {
        auto tmpTuple = boost::pfr::structure_to_tuple(inSerializableNew);
        auto whereTuple = boost::pfr::structure_to_tuple(inSerializableOld);

        SqlUpdate update(inTableName);
        auto processorSet = [&update](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                update.set(inField.GetName(), inField.GetValue());
            }
        };

        SqlWhere where;
        auto processorWhere = [&where](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                where.And(Equal(inField.GetName(), inField.GetValue()));
            }
        };


        std::apply([processorSet](auto&&... args) {
            (processorSet(args),...);
        }, tmpTuple);

        std::apply([processorWhere](auto&&... args) {
            (processorWhere(args),...);
        }, whereTuple);

        outSerialized = update.get() + where.get();
    }

    template <class ...T, class... U>
    static void select(const std::string& inTableName,
                       OutSerialized& outSerialized,
                       const std::tuple<T...>& inSerializable,
                       const std::tuple<U...>& inWhere
                       )
    {
        auto serializable = inSerializable;
        SqlSelect select;
        auto processorSet = [&select](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                select.column(inField.GetName());
            }
        };

        SqlWhere where;
        auto processorWhere = [&where](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                where.And(Equal(inField.GetName(), inField.GetValue()));
            }
        };

        std::apply([processorSet](auto&&... args) {
            (processorSet(args),...);
        }, serializable);

        std::apply([processorWhere](auto&&... args) {
            (processorWhere(args),...);
        }, inWhere);


        outSerialized = select.from(inTableName).where(where).get();
    }

    template <class ...T, class... U>
    static void select(const std::vector<std::string>& inTablesNames,
                       OutSerialized& outSerialized,
                       const std::tuple<T...>& inSerializable,
                       const std::tuple<U...>& inWhere
                       )
    {
        auto serializable = inSerializable;
        SqlSelect select;
        auto processorSet = [&select](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                select.column(inField.GetName());
            }
        };

        SqlWhere where;
        auto processorWhere = [&where](const auto& inField){
            if constexpr (sfinae_utils::is_serializable<decltype(inField)>::value)
            {
                where.And(Equal(inField.GetName(), inField.GetValue()));
            }
        };

        std::apply([processorSet](auto&&... args) {
            (processorSet(args),...);
        }, serializable);

        std::apply([processorWhere](auto&&... args) {
            (processorWhere(args),...);
        }, inWhere);

        outSerialized = select.from(SqlFrom(inTablesNames)).where(where).get();
    }
};

}
