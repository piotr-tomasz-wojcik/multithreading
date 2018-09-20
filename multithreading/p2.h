#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

namespace p2
{

    void withArguemnt(int i)
    {
        std::cout << "From thread " << i << "\n";
    }

    void executeMultipleWithArgumentPassing(int amount)
    {
        std::vector<std::thread> workers;
        for (int i = 0; i < amount; ++i)
        {
            workers.push_back(std::thread(&withArguemnt, i));
        }

        std::cout << "From executor of threads\n";

        std::for_each(workers.begin(), workers.end(), [](std::thread& t)
        {
            t.join();
        });
    }

    void withArguemntByRef(int& i)
    {
        std::cout << "From thread " << i << "\n";
    }


    void executeMultipleWithArgumentPassingByRef(int amount)
    {
        std::vector<std::thread> workers;
        for (int i = 0; i < amount; ++i)
        {
            workers.push_back(std::thread(&withArguemnt, std::ref(i)));
        }

        std::cout << "From executor of threads\n";

        std::for_each(workers.begin(), workers.end(), [](std::thread& t)
        {
            t.join();
        });
    }
}
