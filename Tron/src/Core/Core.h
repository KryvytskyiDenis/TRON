#pragma once

#ifdef TR_DEBUG
    #define TR_ASSERTIONS_ENABLED
    #define TR_DEBUGBREAK() __debugbreak()
#else
    #define TR__DEBUGBREAK()
#endif

#ifdef TR_ASSERTIONS_ENABLED
    #define TR_ASSERT(expr, ...) if (expr) {} \
        else \
        { \
            TR_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            TR_DEBUGBREAK(); \
        }
#else
    #define TR_ASSERT(expr)
#endif
