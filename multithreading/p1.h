#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

namespace p1
{

    void threadFun()
    {
        std::cout << "From thread \n";
    }

    void execute()
    {
        std::thread th(&threadFun);
        std::cout << "Hi from main!\n";
        th.join();
    }

    void executeByLambda()
    {
        std::thread th([]() 
        {
            std::cout << "From lambda thread!\n";
        });

        std::cout << "From caller\n";
        th.join();
    }

    void executeMultipleThreads(int amount)
    {
        std::vector<std::thread> workers;
        for (int i = 0; i < amount; ++i)
        {
            workers.push_back(std::thread([i]()
            {
                std::cout << "From thread " << i << "\n";
            }));
        }

        std::cout << "From executor of threads\n";

        std::for_each(workers.begin(), workers.end(), [](std::thread& t)
        {
            t.join();
        });
    }
}
