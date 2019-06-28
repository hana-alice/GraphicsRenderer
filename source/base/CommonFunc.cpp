#ifdef _WIN64 || _WIN32
#include <windows.h>
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
        // define something for simulator   
    #elif TARGET_OS_IPHONE
        // define something for iphone  
    #else
        #define TARGET_OS_OSX 1
        // define something for OSX
    #endif
#elif __linux
#include <limits.h>
#include <unistd.h>
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif

#include "CommonFunc.h"

using namespace std;

static const int Maxpath = 128;

string CommonFunc::getCurrentDirectory()
{
    #ifdef _WIN64 || _WIN32
    char result[Maxpath];
    return string( result, GetModuleFileName( NULL, result, Maxpath) );
    #elif __linux
    char result[Maxpath];
    ssize_t count = readlink( "/proc/self/exe", result, Maxpath);
    return string( result, (count > 0) ? count : 0 );
    #endif
}