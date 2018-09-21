#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

namespace p6
{
    struct Result
    {
        std::vector<std::string> files;
        std::vector<path> dirs;

        Result() {}
        Result(const Result& r) : dirs(r.dirs), files(r.files) {}
        Result(Result && r) : dirs(std::move(r.dirs)), files(std::move(r.files)) {}

        Result& operator=(const Result& r)
        {
            if (this == &r)
                return *this;

            dirs = r.dirs;
            files = r.files;

            return *this;
        }
    };

    Result listDir(path && dir)
    {
        Result result;
        for (directory_iterator it(dir); it != directory_iterator(); ++it)
        {
            if (is_directory(*it))
            {
                result.dirs.push_back(it->path().string());
            }
            else
            {
                result.files.push_back(it->path().filename().string());
            }
        }

        return result;
    }

    void executeListDir()
    {
        std::string root = "c:\\users\\Public";
        std::vector<path> dirsToDo;
        dirsToDo.push_back(root);

        std::vector<std::string> files;

        while (!dirsToDo.empty())
        {
            std::vector<std::future<Result>> futures;
            
            for (int i = 0; i < 16 and not dirsToDo.empty(); ++i)
            {
                auto future = std::async(&listDir, std::move(dirsToDo.back()));
                futures.push_back(std::move(future));
                dirsToDo.pop_back();
            }

            try
            {
                while (not futures.empty())
                {
                    auto future = std::move(futures.back());
                    auto result = future.get();
                    futures.pop_back();

                    std::copy(result.files.begin(), result.files.end(), std::back_inserter(files));
                    std::copy(result.dirs.begin(), result.dirs.end(), std::back_inserter(dirsToDo));
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

        for (auto& filename : files)
            std::cout << filename << std::endl;
    }
}
