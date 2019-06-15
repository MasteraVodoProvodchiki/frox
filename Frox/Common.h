#if defined _WIN32 || defined _WIN64
#if defined FROX_LIB 
#define FROX_API __declspec(dllexport)
#else
#define FROX_API __declspec(dllimport)
#endif
#elif defined __linux__
#define FROX_API __attribute__((visibility("default")))
#else
#define FROX_API
#endif

#define FROX_VERSION 100

#include <stdint.h>

namespace frox {

class ComputeFrame;
class ComputeNode;
class ComputeFlow;

} // End frox