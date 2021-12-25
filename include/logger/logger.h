#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <thread>
#include <mutex>
#include <utility>
#include <ctime>
#include <chrono>

#define __SUPPORTS_META__
#ifdef __SUPPORTS_META__
#include "meta/jsonizer.h"
#endif

#define EOS_LOG(line)   eos::log_obj(eos::logger::severity::info) << line
#define EOS_LOG_E(line) eos::log_obj(eos::logger::severity::error) << line << eos::logger::endl;
#define EOS_LOG_W(line) eos::log_obj(eos::logger::severity::warning) << line << eos::logger::endl;
#define EOS_LOG_I(line) eos::log_obj(eos::logger::severity::info) << line << eos::logger::endl;
#define EOS_LOG_D(line) eos::log_obj(eos::logger::severity::debug) << line << eos::logger::endl;
#define EOS_LOG_T(line) eos::log_obj(eos::logger::severity::trace) << line << eos::logger::endl;

namespace eos
{

class logger final
{
public: // aliases
    using timestamp_callback = std::function<std::string (void)>;
    using thread_id_callback = std::function<std::string (void)>;
    using block_wrapper = std::pair<std::string /*prefix*/, std::string /*sufix*/>;

    enum class severity
    {
        debug,
        trace,
        info,
        warning,
        error,
    };

    enum class output
    {
        file        = (1u << 0),
        console     = (1u << 1),
        everywhere  = file | console,
    };

    inline std::string stringify(const severity severity)
    {
        std::string result = "NONE";
        const static std::map<eos::logger::severity, std::string> S_SEVERITY_STRINGS
        {
            { severity::error, "ERROR"},
            { severity::warning, "WARNING"},
            { severity::info, "INFO"},
            { severity::debug, "DEBUG"},
            { severity::trace, "TRACE"},
        };

        auto it = S_SEVERITY_STRINGS.find(severity);
        if (it != S_SEVERITY_STRINGS.end())
        {
            result = it->second;
        }
        return result;
    }
    static constexpr auto endl = "\n";

private:
    std::mutex m_mutex;
    std::mutex m_builderMutex;

    std::string m_filename;
    std::fstream m_file;

    std::map<std::string /* thread_id */, std::stringstream /* line */> m_threadsLinesMap;
    std::map<std::string /* thread_id */, severity /* severity */> m_threadsSeveritiesMap;

    severity m_severity = severity::debug;
    int m_output = (int)output::console;

    block_wrapper m_blockWrapper = {"[", "]"};
    timestamp_callback m_getTimestampClb;
    thread_id_callback m_getThreadIdClb;

    bool m_file_opened = false;

private:
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    logger()
    {
        set_timestamp_clb(logger::timestamp);
        set_thread_id_clb(logger::thread_id);
    }

    ~logger()
    {
        close();
    }

public: // static
    static std::string timestamp()
    {
        using namespace std::chrono;

        auto now = system_clock::now();
        auto tt = system_clock::to_time_t ( now );

        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(localtime(&tt), "%F %T");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }

    static std::string thread_id()
    {
        std::stringstream ss;
        ss << std::dec << std::this_thread::get_id();
        return ss.str();
    }

    static std::string wrap_value(const std::string& value,
                                 const block_wrapper& blockWrapper)
    {
        return blockWrapper.first + value + blockWrapper.second;
    }

public:
    static logger& get()
    {
        static logger self;
        return self;
    }
    void set_timestamp_clb(timestamp_callback clb)
    {
        if (clb)
        {
            m_getTimestampClb = clb;
        }
    }
    void set_thread_id_clb(thread_id_callback clb)
    {
        if (clb)
        {
            m_getThreadIdClb = clb;
        }
    }
    void set_block_wrapper(const block_wrapper& blockWrapper)
    {
        m_blockWrapper = blockWrapper;
    }
    bool init(const std::string& filename, const output output = output::everywhere, const severity severity = severity::debug)
    {
        if (m_file_opened)
        {
            return false;
        }

        m_filename = filename;
        m_output = (int)output;
        m_severity = severity;
        return true;
    }
    void flush()
    {
        {
            std::unique_lock<std::mutex> locker(m_builderMutex);
            for (auto& elem : m_threadsLinesMap)
            {
                log(elem.second.rdbuf(), m_threadsSeveritiesMap[elem.first]);
            }
            m_threadsLinesMap.clear();
            m_threadsSeveritiesMap.clear();
        }

        m_file.flush();
    }
    void open()
    {
        if (m_file_opened)
        {
            return;
        }

        if (m_output & (int)output::file) { // || m_output == output::everywhere) {
            m_file.open(m_filename, std::ios::out);
            m_file_opened = m_file.is_open();
        }

        if (!m_file_opened)
        {
            throw std::runtime_error("Coudn't open a file " + m_filename);
        }
    }
    void close()
    {
        if (m_file_opened)
        {
            m_file.flush();
            m_file.close();
        }
        m_file_opened = false;
    }

    void log(const std::string& str, severity severity)
    {
        if (severity < m_severity)
        {
            return;
        }

        std::string res = wrap_value(m_getTimestampClb(), m_blockWrapper) + " "
                + wrap_value(m_getThreadIdClb(), m_blockWrapper) + " "
                + wrap_value(stringify(severity), m_blockWrapper) + " "
                + str;

        std::unique_lock<std::mutex> locker(m_mutex);
        if (m_output & (int)output::file)
        {
            if (m_file_opened) {
                m_file << res << std::endl;
            }
        }
        if (m_output & (int)output::console)
        {
            if (severity == severity::error || severity == severity::warning)
                std::cerr << res << std::endl;
            else
                std::cout << res << std::endl;
        }
    }

    template<class T>
    void log(const T& value, severity severity)
    {
        std::stringstream ss;

#ifdef __SUPPORTS_META__
        if constexpr (eos::meta::is_meta<T>::object)
        {
            ss << meta::jsonizer::to_json(value);
        }
        else
        {
            ss << value;
        }
#else
        ss << value;
#endif
        log(ss.str(), severity);
    }

    template <class T>
    void add_part(const T& t)
    {
        std::unique_lock<std::mutex> locker(m_builderMutex);
        auto& ss = m_threadsLinesMap[m_getThreadIdClb()];

#ifdef __SUPPORTS_META__
        if constexpr (meta::is_meta<T>::property)
        {
            ss << meta::jsonizer::to_json(t);
        }
        else
        {
            ss << t;
        }
#else
    ss << t;
#endif
    }

    void flush_part()
    {
        const std::string threadId = m_getThreadIdClb();

        std::unique_lock<std::mutex> locker(m_builderMutex);
        log(m_threadsLinesMap[threadId].rdbuf(), m_threadsSeveritiesMap[threadId]);

        // cleanup map to avoid garbage (in case each thread lifetime is very short)
        m_threadsLinesMap.erase(threadId);
        m_threadsSeveritiesMap.erase(threadId);
    }

    logger& operator()(severity severity)
    {
        if (m_threadsSeveritiesMap[m_getThreadIdClb()] != severity)
            flush();

        m_threadsSeveritiesMap[m_getThreadIdClb()] = severity;
        return get();
    }

};

inline logger& operator<<(logger& os, const std::string& data)
{
    if (data == logger::endl)
    {
        os.flush_part();
    }
    else
    {
        os.add_part(data);
        auto pos = data.rfind("\n");
        if (pos != std::string::npos && pos == data.size()-1)
            os.flush_part();
    }
    return os;
}

inline logger& operator<<(logger& os, const char* data)
{
    os << std::string(data);
    return os;
}

template<class T>
inline logger& operator<<(logger& os, const T& data)
{
    os.add_part(data);
    return os;
}
inline logger& log_obj(logger::severity severity = logger::severity::debug)
{
    return logger::get()(severity);
}

}
