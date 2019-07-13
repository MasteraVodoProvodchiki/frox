#pragma once

#include <vector>
#include <string>
#include <memory>

#if defined _WIN32 || defined _WIN64
#if defined FROXIRS_LIB 
#define FROXIRS_API __declspec(dllexport)
#else
#define FROXIRS_API __declspec(dllimport)
#endif
#elif defined __linux__
#define FROXIRS_API __attribute__((visibility("default")))
#else
#define FROXIRS_API
#endif