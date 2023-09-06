#pragma once

#define AURORA_ENABLE_PROFILING !AURORA_DISTRIBUTION

#if AURORA_ENABLE_PROFILING 
#include "optick.h"
#endif

#if AURORA_ENABLE_PROFILING
#define AURORA_PROFILE_FRAME(...)           OPTICK_FRAME(__VA_ARGS__)
#define AURORA_PROFILE_FUNC(...)            OPTICK_EVENT(__VA_ARGS__)
#define AURORA_PROFILE_TAG(NAME, ...)       OPTICK_TAG(NAME, __VA_ARGS__)
#define AURORA_PROFILE_SCOPE_DYNAMIC(NAME)  OPTICK_EVENT_DYNAMIC(NAME)
#define AURORA_PROFILE_THREAD(...)          OPTICK_THREAD(__VA_ARGS__)
#else
#define AURORA_PROFILE_FRAME(...)
#define AURORA_PROFILE_FUNC(...)
#define AURORA_PROFILE_TAG(NAME, ...) 
#define AURORA_PROFILE_SCOPE_DYNAMIC(NAME)
#define AURORA_PROFILE_THREAD(...)
#endif