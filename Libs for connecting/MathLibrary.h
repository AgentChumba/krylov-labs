#ifndef MATHLIBRARY_H
#define MATHLIBRARY_H

#if defined(_WIN32) || defined(_WIN64)
    #ifdef MATHLIBRARY_EXPORTS
        #define MATHLIBRARY_API __declspec(dllexport)
    #else
        #define MATHLIBRARY_API __declspec(dllimport)
    #endif
#else
    #define MATHLIBRARY_API
#endif

// Пространство имён MathLibrary
namespace MathLibrary {
    MATHLIBRARY_API double add(double a, double b);
    MATHLIBRARY_API double subtract(double a, double b);
    MATHLIBRARY_API double multiply(double a, double b);
    MATHLIBRARY_API double divide(double a, double b);
}

#endif // MATHLIBRARY_H
