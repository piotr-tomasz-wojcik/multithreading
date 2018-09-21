#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <experimental/filesystem>
#include "p6.h"

using p6::Result;

namespace p7
{
    class MonitorResult
    {
        Result result;
        std::mutex fileMutex;
        std::mutex dirMutex;

        void move_n_from_back(std::vector<path>& src, unsigned count, std::vector<path>& dest)
        {
            if (count <= src.size())
            {
                std::move(src.end() - count, src.end(), std::back_inserter(dest));
                src.erase(src.end() - count, src.end());
            }
            else
            {
                std::move(src.begin(), src.end(), std::back_inserter(dest));
                src.erase(src.begin(), src.end());
            }
        }

    public:
        void putFile(std::string && s)
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            result.files.push_back(s);
        }

        void putDir(path && s)
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            result.dirs.push_back(s);
        }

        std::vector<path> getDirs(unsigned n)
        {
            std::lock_guard<std::mutex> lock(dirMutex);

            std::vector<path> dirs(result.dirs.size() > n ? result.dirs.size() : n);
            move_n_from_back(result.dirs, n, dirs);

            return dirs;
        }

        inline bool isDirsEmpty()
        {
            std::lock_guard<std::mutex> lock(dirMutex);
            return result.dirs.empty();
        }
    };

    void listDir2(path && dir, MonitorResult & result)
    {
        for (auto& entry : directory_iterator(dir))
        {
            if (is_directory(entry))
            {
                result.putDir(entry.path().string());
            }
            else
            {
                result.putFile(entry.path().filename().string());
            }
        }
    }

    void executeListDir()
    {
        std::string root = "c:\\users\\Public";
        MonitorResult result;
        result.putDir(path(root));

        while (!result.isDirsEmpty())
        {
            std::vector<path> dirsToDo = result.getDirs(16);
            std::vector<std::future<void>> futures;

            while(not dirsToDo.empty())
            {
                auto future = std::async(&listDir2, std::move(dirsToDo.back()), std::ref(result));
                futures.push_back(std::move(future));
                dirsToDo.pop_back();
            }

            try
            {
                while (not futures.empty())
                {
                    auto future = std::move(futures.back());
                    future.wait();
                    futures.pop_back();
                }
            }
            catch (std::system_error & e)
            {
                std::cout << "System error: " << e.code().message() << std::endl;
            }
            catch (std::exception & e)
            {
                std::cout << "Exception: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cout << "Unknown error..." << std::endl;
            }
        }
    }
}
