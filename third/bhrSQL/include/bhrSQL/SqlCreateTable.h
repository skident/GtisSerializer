#pragma once

#include <string>
#include <vector>
#include <optional>
#include <sstream>

namespace bhr::sql
{

enum EFlags
{
    NotNull         = 1 << 0,
    PrimaryKey      = 1 << 1,
    AutoIncrement   = 1 << 2,
    Unique          = 1 << 3,
};

constexpr int FLAG_NP = EFlags::NotNull | EFlags::PrimaryKey;
constexpr int FLAG_AU = EFlags::AutoIncrement | EFlags::Unique;
constexpr int FLAG_NPA = FLAG_NP | EFlags::AutoIncrement;
constexpr int FLAG_NPU = FLAG_NP | EFlags::Unique;
constexpr int FLAG_NPAU = FLAG_NP | FLAG_AU;

enum class EParamDescriptor
{
    CheckFn     = 1 << 0,
    ForeignKey  = 1 << 1,
};

using Params = std::vector<std::pair<int, std::string>>;



class SqlColumn
{
private:
    template <class T>
    std::string to_string(const T& inValue)
    {
        std::stringstream ss;
        ss << inValue;
        return ss.str();
    }

public:
    template <class T>
    SqlColumn(const std::string& inName,
              const std::string& inType,
              const T& inDefaultValue = {},
              bool inNotNull = true,
              bool inPrimaryKey = false,
              bool inAutoIncrement = false,
              bool inUnique = false,
              const std::string inCheck = "",
              const std::string inForeignKey = ""
              )
        : mName(inName)
        , mType(inType)
        , mNotNull(inNotNull)
        , mPrimaryKey(inPrimaryKey)
        , mAutoIncrement(inAutoIncrement)
        , mUnique(inUnique)
        , mDefaultValue(to_string(inDefaultValue))
        , mCheck(inCheck)
        , mForeignKey(inForeignKey)
    {

    }

    template <class T>
    SqlColumn(const std::string& inName,
              const T& inValue,
              int inFlags,
              Params inParams)
        : mName(inName)
        , mType(type_to_string<T>())
        , mNotNull(inFlags & EFlags::NotNull)
        , mPrimaryKey(inFlags & EFlags::PrimaryKey)
        , mAutoIncrement(inFlags & EFlags::AutoIncrement)
        , mUnique(inFlags & EFlags::Unique)
    {
        //mDefaultValue
    }

    template<class T>
    std::string type_to_string()
    {
        if (std::is_integral<T>::value) {
            return "INTEGER";
        } else if (std::is_same<T, float>::value || std::is_same<T, double>::value) {
            return "REAL";
        } else if (std::is_same<T, std::string>::value) {
            return "TEXT";
        }

        return "";
    }

    std::string get() const
    {
        std::string statement;
        statement = "`" + mName + "` " + mType;

        if (mNotNull)
            statement += " NOT NULL";

        if (!mDefaultValue.empty())
            statement += " DEFAULT '" + mDefaultValue + "'";

        if (!mCheck.empty())
            statement += " CHCECK(" + mCheck + ")";

        if (mPrimaryKey)
            statement += " PRIMARY KEY";

        if (mAutoIncrement)
            statement += " AUTOINCREMENT";

        if (mUnique)
            statement += " UNIQUE";

        return statement;
    }

    std::string foreignKey() const
    {
        if (mForeignKey.empty())
            return "";
        return "FOREIGN KEY(`" + mName + "`) REFERENCES " + mForeignKey;
    }

private:
    const std::string mName;
    const std::string mType;
    const bool mNotNull;
    const bool mPrimaryKey;
    const bool mAutoIncrement;
    const bool mUnique;
    const std::string mDefaultValue;
    const std::string mCheck;
    const std::string mForeignKey;
};


class SqlCreateTable
{
public:
    SqlCreateTable(const std::string& inTableName)
        : mTableName(inTableName)
    {
    }

    SqlCreateTable& column(const SqlColumn& inColumnInfo)
    {
        mColumns.push_back(inColumnInfo);
        return *this;
    }

    template <class... T>
    SqlCreateTable& column(const SqlColumn& inColumnInfo, const T&... inColumnsInfo)
    {
        column(inColumnInfo);
        column(inColumnsInfo...);
        return *this;
    }

    std::string get() const
    {
        std::string foreign_keys;
        std::string statement;
        for (const auto& column : mColumns)
        {
            if (!statement.empty())
                statement += ",\n";
            statement += column.get();

            auto fk = column.foreignKey();
            if ( !fk.empty())
            {
                if (!foreign_keys.empty())
                    foreign_keys += ", ";
                foreign_keys += fk;
            }
        }

        if (!foreign_keys.empty())
            statement += ", " + foreign_keys;

        return "CREATE TABLE IF NOT EXISTS `" + mTableName + "` (" + statement + ");";
    }

private:
    std::string mTableName;
    std::vector<SqlColumn> mColumns;
};

}
