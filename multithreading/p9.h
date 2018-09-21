#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <future>
#include <condition_variable>
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

namespace p9
{
    constexpr int NUM_THREADS = 10;

    template<typename T>
    class MsgQueue
    {
        std::deque<T> queue;
        std::condition_variable condition;
        std::mutex mutex;

    public:
        void send(T && msg)
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                queue.push_front(std::move(msg));
            }
            condition.notify_one();
        }

        T receive()
        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this] { return not queue.empty(); });
            T msg = std::move(queue.back());
            queue.pop_back();

            return msg;
        }
    };

    void listDirServer(MsgQueue<path>& dirs, MsgQueue<std::string>& files)
    {
        while (true)
        {
            path dir = dirs.receive();

            for (auto& entry : directory_iterator(dir))
            {
                if (is_directory(entry.path()))
                {
                    dirs.send(entry.path().string());
                }
                else
                {
                    files.send(entry.path().filename().string());
                }
            }
        }
    }

    void printServer(MsgQueue<std::string>& names)
    {
        while (true)
        {
            std::string name = names.receive();
            std::cout << name << std::endl;
        }
    }

    void listTree(path && rootDir)
    {
        MsgQueue<path> dirs;
        MsgQueue<std::string> files;
        dirs.send(std::move(rootDir));

        std::vector<std::future<void>> futures;
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            futures.push_back(std::async(std::launch::async, &listDirServer, std::ref(dirs), std::ref(files)));
        }
        futures.push_back(std::async(std::launch::async, &printServer, std::ref(files)));

        try
        {
            while (not futures.empty())
            {
                auto future = std::move(futures.back());
                futures.pop_back();
                future.wait();
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
