#pragma once
#include <thread>
#include <future>
#include <iostream>
#include <string>

namespace p4
{
    void thFun(std::promise<std::string> && promise)
    {
        std::string s("hello from fruture");
        promise.set_value(s);
    }

    void promiseByMove()
    {
        std::promise<std::string> promise;
        std::future<std::string> future = promise.get_future();
        std::thread thread(&thFun, std::move(promise));

        std::cout << "Hello from execute\n";

        std::string hello = future.get();

        std::cout << hello << std::endl;
        thread.join();
    }

    void thFunPromiseByRef(std::promise<std::string> & promise)
    {
        std::string s("hello from fruture");
        std::cout << "String data future: " << (void*)s.data() << std::endl;

        promise.set_value(std::move(s));

    }

    void promiseByRef()
    {
        std::promise<std::string> promise;
        
        std::thread thread(&thFunPromiseByRef, std::ref(promise));

        std::cout << "Hello from execute\n";

        std::future<std::string> future = promise.get_future();
        std::string hello = future.get();
        std::cout << "String data local: " << (void*)hello.data() << std::endl;

        std::cout << hello << std::endl;
        thread.join();
    }

    std::string fun()
    {
        std::string s("Hello from future");
        return s;
    }

    void futureByAsync()
    {
        auto future = std::async(&fun);
        std::cout << "Hello from byAsync\n";
        std::string hello = future.get();
        std::cout << hello << std::endl;
    }

    std::string funWithException()
    {
        std::string s("Hello from future");
        throw(std::exception("Exception from task"));
        return s;
    }

    void futureByAsyncWithException()
    {
        auto future = std::async(&funWithException);
        std::cout << "Hello from byAsync\n";
        try 
        {
            std::string hello = future.get();
            std::cout << hello << std::endl;
        }
        catch (std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    void funWithoutReturn()
    {
        std::cout << "start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "end" << std::endl;
    }

    void exeuteNoReturn()
    {
        auto future = std::async(&funWithoutReturn);
        std::cout << "no return called" << std::endl;
    }
}
