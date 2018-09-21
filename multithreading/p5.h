#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <experimental/filesystem>
#include "util.h"

using namespace std::experimental::filesystem::v1;

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

    using strings = std::vector<std::string>;

    strings listDirectory(std::string && dir)
    {
        strings listing;
        std::string dirStr("\n> ");
        dirStr += dir;
        dirStr += ":\n";
        listing.push_back(dirStr);

        std::vector<std::future<strings>> futures;

        std::cout << "Dir: " << dirStr << std::endl;

        for (directory_entry entry : directory_iterator(dir))
        {
            if (is_directory(entry))
            {
                std::wstring wpath = entry.path().native();

                auto future = std::async(std::launch::async, 
                    &listDirectory, 
                    wide2std(wpath));

                futures.push_back(std::move(future));
            }
            else
            {
                std::wstring wpath = entry.path().filename().native();
                listing.push_back(wide2std(wpath));
            }
        }

        for (auto& future : futures)
        {
            strings list = future.get();
            std::copy(list.begin(), list.end(), std::back_inserter(listing));
        }
        
        return listing;
    }

    void executeListDirectory(std::string root)
    {
        auto future = std::async(std::launch::async, &listDirectory, root);
        try
        {
            strings listing = future.get();
            for (auto& filename : listing)
                std::cout << filename << std::endl;
        }
        catch (std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "Unknown exception\n";
        }
    }
}
