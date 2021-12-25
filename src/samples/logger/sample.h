#pragma once

#include <iostream>
#include "logger/logger.h"

#include <vector>
#include <chrono>

using namespace std;

namespace eos::samples::logger {

namespace  {
    void func()
    {
        static int k = 0;
        k++;
        for (int i = 0; i < 10; i++)
        {
            eos::log_obj(eos::logger::severity::error) << "Hello" << " from " << " thread #" << k << eos::logger::endl;
            eos::log_obj() << "Iteration #" << i+1  << eos::logger::endl;
            this_thread::sleep_for(std::chrono::milliseconds(rand()%10));
        }
    }
}

void run()
{
    using namespace eos;
    using LogSeverity = eos::logger::severity;
    try
    {
        auto& logger = eos::logger::get();
        logger.init("test.log");
        logger.open();


        std::vector<std::thread> threads;
        for (int i = 0; i < 10; i++)
        {
            threads.emplace_back(std::thread(func));
        }

        logger.log("Achtung!!!", LogSeverity::error);
        logger.log(42, LogSeverity::warning);
        logger.log('c', LogSeverity::info);

        logger.log(3.14, LogSeverity::debug);


        logger << "streamed log";
        logger << "more stream" << 123 << 'c' << eos::logger::endl;
        logger(LogSeverity::error) << "Error is here" << eos::logger::endl;

        for (auto& elem : threads)
        {
            if (elem.joinable()) {
                elem.join();
            }
        }

        logger.close();
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown exceptopn caught" << std::endl;
    }
}

}
