#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <iomanip>
#include <list>

#define ADD_PERFORMANCE_MARK(performanceObj) performanceObj.mark( __FILE__, ":",  __LINE__)
#define ADD_PERFORMANCE_MARK_ARGS(performanceObj, args...) performanceObj.mark( __FILE__, ":",  __LINE__, args)

namespace eos {

class performance
{
public:
    using clock = std::chrono::system_clock;
    using time_point = typename clock::time_point;

    static std::string timepoint_to_string(const time_point& inTimePoint)
    {
        using namespace std::chrono;

        auto tt = clock::to_time_t ( inTimePoint );

        auto ms = duration_cast<milliseconds>(inTimePoint.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(localtime(&tt), "%F %T");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }


public:
    performance(const std::string& inUniqueName = "")
        : mName(inUniqueName)
    {

    }

    ~performance()
    {
        dump();
    }

    struct entry {
        std::string name;
        time_point timepoint;
        time_point prevTimepoint;

        double elapsed() const
        {
            double elapsed_time_ms = std::chrono::duration<double, std::milli>(timepoint-prevTimepoint).count();
            return elapsed_time_ms;
        }

        std::string ToString() const
        {
            std::stringstream ss;
            ss << "[" << timepoint_to_string(timepoint) << "] [" << elapsed() << " ms] " << name;
            return ss.str();
        }
    };

    performance& mark(const std::string& inName)
    {
        entry e{inName, clock::now(), mPrevTimepoint};
        mPrevTimepoint = e.timepoint;
        mEntries.emplace_back(e);
        mDumped = false;
        return *this;
    }

    template <typename Arg, typename... Args>
    performance& mark(Arg arg, Args... args)
    {
        std::stringstream ss;
        this->to_stream(ss, arg, args...);
        this->mark(ss.str());
        return *this;
    }

    void dump(bool inClearAfterDump = true)
    {
        if (mDumped) {
            return;
        }

        std::string prefix;
        if (!mName.empty()) {
            prefix = "[" + mName + "] ";
        }
        for (const auto& entry : mEntries) {
            std::cout << prefix << entry.ToString() << std::endl;
        }
        mDumped = true;

        if (inClearAfterDump) {
            this->clear();
        }
    }

    void clear()
    {
        mEntries.clear();
    }

private:
    template <typename Arg, typename... Args>
    void to_stream(std::ostream& out, Arg&& arg, Args&&... args)
    {
        out << std::forward<Arg>(arg);
        ((out << std::forward<Args>(args)), ...);
    }

private:
    std::string mName;
    time_point mPrevTimepoint = clock::now();
    std::list<entry> mEntries;
    bool mDumped = false;
};
}
