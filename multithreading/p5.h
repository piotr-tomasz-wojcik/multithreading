#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <future>

namespace p5 
{
    void tasks(int amount)
    {
        std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

        std::vector<std::future<void>> futures;
        for (int i = 0; i < amount; i++)
        {
            auto future = std::async([]
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << std::this_thread::get_id() << " ";
            });
            futures.push_back(std::move(future));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    void tasksWithoutWaiting(int amount)
    {
        std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

        std::vector<std::future<void>> futures;
        for (int i = 0; i < amount; i++)
        {
            auto future = std::async([]
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << std::this_thread::get_id() << " ";
            });
            futures.push_back(std::move(future));
        }
    }

}
