// Functions.hpp
// Written by TechEverything

/*
Macros that should simplify function definition
*/

#pragma once

#define return_type(tp) tp
#define private static
#define replace inline
#define function

#ifdef _MSC_VER
    #define self_clean __stdcall
    #define caller_clean __cdecl

    #define no_return [[noreturn]]
#elif defined(__GNUC__) || defined(__clang__)
    #define self_clean __attribute__((stdcall))
    #define caller_clean __attribute__((cdecl))

    #define no_return __attribute__((noreturn))
#else
    #error "Unknown compiler"
#endif

#ifdef __cplusplus
    #define no_mangle extern "C"
#else
    #define no_mangle
#endif