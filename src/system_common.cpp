#include <cstdlib>
#include "system_common.h"

#ifdef _WIN32
#include <conio.h>

#elif __linux__
#include <iostream>
#include <termios.h>
#include <unistd.h>

#else
    #error "Unknown compiler"
#endif // Compiler check

namespace SysCmm {

/**
 * @brief Clears the terminal screen.
 *
 * This function clears the terminal screen to provide a clean interface for the user.
 * It is implemented to work on both Windows and Linux platforms.
 * 
 * On Windows, it uses the `system("cls")` command.
 * On Linux, it uses the `system("clear")` command.
 */
void ClearTerminal(void)
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#else
    #error "Unknown compiler"
#endif // Compiler

}

/**
 * @brief Waits for the user to press any key.
 *
 * This function pauses the execution and waits for the user to press any key before continuing.
 * It is implemented to work on both Windows and Linux platforms.
 * 
 * On Windows, it uses `_getch()` from the `conio.h` library.
 * On Linux, it manipulates terminal settings to read a single character input without requiring
 * the user to press Enter.
 */
void WaitForAnyKey(void)
{
#ifdef _WIN32
    _getch();
#elif __linux__
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
#else
    #error "Unknown compiler"
#endif // Compiler
}

}