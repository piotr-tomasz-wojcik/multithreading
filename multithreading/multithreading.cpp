#include "pch.h"
#include <numeric>
#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"
#include "p6.h"
#include "p7.h"
#include "p9.h"

void p6p7Check()
{
    std::vector<unsigned long long> p6Times;
    std::vector<unsigned long long> p7Times;
    for (int i = 0; i < 100; i++)
    {
        {
            auto startTime = std::chrono::system_clock::now();
            p6::executeListDir();
            auto duration = std::chrono::system_clock::now() - startTime;
            p6Times.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
        }

        {
            auto startTime = std::chrono::system_clock::now();
            p7::executeListDir();
            auto duration = std::chrono::system_clock::now() - startTime;
            p7Times.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
        }
    }

    unsigned long long p6Sum = std::accumulate(p6Times.begin(), p6Times.end(), 0ull);
    unsigned long long p7Sum = std::accumulate(p7Times.begin(), p7Times.end(), 0ull);

    std::cout << "P6:: " << p6Sum / 100.0f << " P7:: " << p7Sum / 100.0f << std::endl;
}


int main()
{
    p9::listTree("C:\\Users\\Public");
}
