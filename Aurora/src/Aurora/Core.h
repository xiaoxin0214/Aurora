#pragma once
#ifdef AURORA_PLATFORM_WINDOWS
#ifdef AURORA_BUILD_DLL
#define AURORA_API __declspec(dllexport)
#else
#define AURORA_API __declspec(dllimport)
#endif
#else
#error Aurora only support Windows for now!
#endif

#ifdef AURORA_ENABLE_ASSERT
#define AURORA_ASSERT(x,...) {if(!(x)){AURORA_ERROR("Assertion Failed:{0}",__VA_ARGS__);__debugbreak();}}
#define AURORA_CORE_ASSERT(x,...) {if(!(x)){AURORA_CORE_ERROR("Assertion Failed:{0}",__VA_ARGS__);__debugbreak();}} 
#else
#define AURORA_ASSERT(x,...)
#define AURORA_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1<<x)

#define BIND_EVENT_FN(func) std::bind(&func,this,std::placeholders::_1)