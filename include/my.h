
#ifndef MY_H_
#define MY_H_

#define STATIC_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define MAX(a, b)     ((a > b) ? a : b)
#define MIN(a, b)     ((a < b) ? a : b)

// ATTRIBUTES
#define N_U         __attribute__((unused))
#define UNUSED      __attribute__((unused))
#define PURE        __attribute__((pure))
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR  __attribute__((destructor))
#define INLINE      __attribute__((always_inline))
#define NO_INLINE   __attribute__((noinline))
#define CONST       __attribute__((const))
#define MALLOC      __attribute__((malloc))
#define NO_RETURN   __attribute__((noreturn))
// !ATTRIBUTES

#endif /* MY_H_ */
