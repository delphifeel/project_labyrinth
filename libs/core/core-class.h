#ifndef _CORE_CLASS_H_
#define _CORE_CLASS_H_

#define NOT_COPYABLE(T)                    \
    T(const T&) = delete;                  \
    T(T&&) = delete;                       \
    T& operator=(const T&) = delete;       \
    T& operator=(T&&) = delete;           

#endif