#pragma once

#ifdef _WIN32
    #include <clocale>
    #include <windows.h>
#elif __linux__
    #include <locale>
#elif __APPLE__
    #include <locale>
#else
    #error ...
#endif

#include <fcntl.h>
#include <io.h>

void enableTurkishEncoding(void)
{
#ifdef _WIN32
    setlocale(LC_ALL, "Turkish");
#elif __linux__
    std::locale::global(std::locale("tr_TR.UTF - 8"));
#elif __APPLE__
    std::locale::global(std::locale("tr_TR.UTF - 8"));
#else

#endif
}