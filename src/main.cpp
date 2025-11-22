#include "window.hpp"
#include <iostream>
#include <exception>

int main(int, char**)
{
    try
    {
        Window window;
        window.init();
        window.exec();
        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }


    return 1;
}
