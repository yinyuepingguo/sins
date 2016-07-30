#ifndef _SINS_BASE_BASE_H
#define _SINS_BASE_BASE_H

#if defined(_WIN32) || defined(WIN64)
#   define interface __declspec(dllexport)
#else
#   define interface __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
#   define interface extern "C" __interface
#endif

#include "base-types.h"

#ifndef NULL
#   define NULL ((void *) 0)
#endif

#ifndef offsetof
#   define offsetof(TYPE, MEMBER)      ((slong)&((TYPE *)0)->MEMBER)
#endif

#ifndef offsetofend
#   define offsetofend(TYPE, MEMBER)   \
        (offsetof(TYPE, MEMBER) + sizeof(((TYPE *)0)->MEMBER))
#endif

#ifndef container_of
#   define container_of(PTR, TYPE, MEMBER) ({  \
            const typeof(((TYPE *)0)->MEMBER) *__mptr = (PTR);  \
            (TYPE *)((char *)__mptr - offsetof(TYPE, MEMBER));  \
        })
#endif

#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef likely
#   define likely(x)       __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#   define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

#ifndef deprecated
#   define deprecated      __attribute__((deprecated))
#endif

#ifndef min
#   define min(a, b)    ((a)<(b)? (a): (b))
#endif

#ifndef max
#   define max(a, b)    ((a)>(b)? (a): (b))
#endif

#ifndef abs
#   define abs(x)       ((x)>=0? (x): -(x))
#endif

#endif

