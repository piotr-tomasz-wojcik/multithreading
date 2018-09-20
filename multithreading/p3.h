#pragma once
#include <thread>
#include <iostream>
#include <vector>
#include <list>

namespace p3
{
    struct List
    {
        struct Node
        {
            int x;
            Node* next;

            Node(int y) : x(y), next(nullptr) {}
        };

        Node* head;
        List() : head(nullptr) {}

        void insert(int x)
        {
            auto node = new Node(x);
            node->next = head;
            head = node;
        }

        int count() const
        {
            int counter = 0;
            Node* ptr = head;
            while (ptr)
            {
                counter++;
                ptr = ptr->next;
            }
            return counter;
        }
    };

    void listInsert(List& list)
    {
        for (int i = 0; i < 1000; ++i)
        {
            list.insert(i);
        }
    }

    void listThreadExecute()
    {
        List list;
        std::vector<std::thread> workers;

        for (int i = 0; i < 10; i++)
        {
            workers.push_back(std::thread(&listInsert, std::ref(list)));
        }

        for (auto& th : workers)
        {
            th.join();
        }

        std::cout << "Items: " << list.count() << std::endl;
    }

    void toSin(std::list<double>& list)
    {
        for (auto& x : list)
            x = sin(x);
    }

    void toSinExecute()
    {
        std::list<double> list;

        const double pi = 3.141592;
        const double epsilon = 0.0000001; // 10^-7
        for (double x = 0.0; x < 2 * pi + epsilon; x += pi / 16)
        {
            list.push_back(x);
        }

        std::thread th(&toSin, std::ref(list));
        th.join();

        for (auto& x : list)
        {
            int count = static_cast<int>(10 * x + 10.5);
            for (int i = 0; i < count; ++i)
                std::cout.put('*');
            std::cout << std::endl;
        }
    }

    void toSinWithSleep(std::list<double>& list)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (auto& x : list)
            x = sin(x);
    }

    void toSinExecuteWithDataRace()
    {
        std::list<double> list;

        const double pi = 3.141592;
        const double epsilon = 0.0000001; // 10^-7
        for (double x = 0.0; x < 2 * pi + epsilon; x += pi / 16)
        {
            list.push_back(x);
        }

        std::thread th(&toSinWithSleep, std::ref(list));

        for (auto& x : list)
        {
            int count = static_cast<int>(10 * x + 10.5);
            for (int i = 0; i < count; ++i)
                std::cout.put('*');
            std::cout << std::endl;
        }
        
        th.join();
    }

    void toSinWithSleepAndMove(std::list<double>&& list)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (auto& x : list)
            x = sin(x);
    }

    void toSinExecuteWithMove()
    {
        std::list<double> list;

        const double pi = 3.141592;
        const double epsilon = 0.0000001; // 10^-7
        for (double x = 0.0; x < 2 * pi + epsilon; x += pi / 16)
        {
            list.push_back(x);
        }

        std::thread th(&toSinWithSleepAndMove, std::move(list));

        for (auto& x : list)
        {
            int count = static_cast<int>(10 * x + 10.5);
            for (int i = 0; i < count; ++i)
                std::cout.put('*');
            std::cout << std::endl;
        }

        th.join();
    }
}
