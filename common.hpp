#ifndef __COMMON_HPP__
#define __COMMON_HPP__

bool debug = false;

#define DEBUG(...)                                              \
        do {                                                    \
                if (debug)                                      \
                        printf("[DEBUG] " __VA_ARGS__);         \
        } while (0)

#define ERROR(...)                                      \
        do {                                            \
                printf("[ERROR] " __VA_ARGS__);         \
        } while (0)

#endif  /* ifndef __COMMON_HPP__ */
