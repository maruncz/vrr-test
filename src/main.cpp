#include "window.hpp"
#include <iostream>
#include <exception>

int main()
{
    try
    {
        Window window;
        window.init();
        window.exec();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }


    return 1;
}
