#pragma once

#include <vector>
#include <string>
#include <memory>

#if defined _WIN32 || defined _WIN64
#if defined FROXASTRA_LIB 
#define FROXASTRA_API __declspec(dllexport)
#else
#define FROXASTRA_API __declspec(dllimport)
#endif
#elif defined __linux__
#define FROXASTRA_API __attribute__((visibility("default")))
#else
#define FROXASTRA_API
#endif