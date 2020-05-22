#pragma once

#include <string>
#include <sstream>

namespace bhr::sql {


template <typename T>
class has_string_operator
{
    template<typename C> static std::true_type test( decltype(std::declval<C>().operator std::string()) ) {}
    template<typename C> static std::false_type test(...) {}

public:
    static bool const value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
};



template <class T>
inline std::string to_string(const T& inValue)
{
    std::stringstream ss;
    if constexpr (has_string_operator<T>::value) {
        ss << (std::string)inValue;
    }
    else
    {
        ss << inValue;
    }
    return ss.str();
}

class SqlCondition
{
public:
    SqlCondition(const std::string& inCondition = "")
        : mStatement(inCondition)
    {

    }

    SqlCondition(const std::string& inLeftCond, const std::string& inOperation, const std::string& inRightCond)
        : SqlCondition(inLeftCond + " " + inOperation + " " + inRightCond)
    {
    }

    template <class T>
    SqlCondition(const std::string& inLeftCond, const std::string& inOperation, const T& inRightCond)
        : SqlCondition(inLeftCond, inOperation, to_string(inRightCond))
    {
    }

    SqlCondition(const SqlCondition& inLhs, const std::string& inOperation, const SqlCondition& inRhs, bool inWrapWithParentheses = false)
    {
        mStatement = wrap_if_needed(inLhs.get() +  " " + inOperation + " " + inRhs.get(), inWrapWithParentheses);
    }

    SqlCondition& And(const SqlCondition& inRhs, bool inWrapWithParentheses = false)
    {
        std::string tmp = this->get();
        if (!tmp.empty())
            tmp += " AND ";
        tmp += inRhs.get();

        mStatement = wrap_if_needed(tmp, inWrapWithParentheses);
        return *this;
    }

    SqlCondition& Or(const SqlCondition& inRhs, bool inWrapWithParentheses = false)
    {
        std::string tmp = this->get();
        if (!tmp.empty())
            tmp += " OR ";
        tmp += inRhs.get();

        mStatement = wrap_if_needed(tmp, inWrapWithParentheses);
        return *this;
    }

    std::string get() const
    {
        return mStatement;
    }

private:
    static std::string wrap_if_needed(const std::string& inValue, bool inWrapWithParentheses)
    {
        return (inWrapWithParentheses ? "(" : "") + inValue + (inWrapWithParentheses ? ")" : "");
    }

private:
    std::string mStatement;
};


template<class T>
inline SqlCondition LessThan(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, "<", to_string(inValue));
}

template<class T>
inline SqlCondition LessEqual(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, "<=", to_string(inValue));
}

template<class T>
inline SqlCondition GreaterThan(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, ">", to_string(inValue));
}

template<class T>
inline SqlCondition GreatEqual(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, ">=", to_string(inValue));
}

template<class T>
inline SqlCondition Equal(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, "=", to_string(inValue));
}

template<class T>
inline SqlCondition NotEqual(const std::string& inName, const T& inValue)
{
    return SqlCondition(inName, "<>", to_string(inValue));
}

inline SqlCondition And(const SqlCondition& inLhs, const SqlCondition& inRhs, bool inWrapWithParentheses = false)
{
    return SqlCondition(inLhs).And(inRhs, inWrapWithParentheses);
}

inline SqlCondition Or(const SqlCondition& inLhs, const SqlCondition& inRhs, bool inWrapWithParentheses = false)
{
    return SqlCondition(inLhs).Or(inRhs, inWrapWithParentheses);
}

}
