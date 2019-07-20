#pragma once

#if defined _WIN32 || defined _WIN64
#if defined FROXDS_LIB 
#define FROXDS_API __declspec(dllexport)
#else
#define FROXDS_API __declspec(dllimport)
#endif
#elif defined __linux__
#define FROXDS_API __attribute__((visibility("default")))
#else
#define FROXDS_API
#endif

#include <memory>