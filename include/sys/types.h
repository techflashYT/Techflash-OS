// Get all of the various types
#define __need_wchar_t
#define __need_size_t
#define __need_ptrdiff_t
#define __need_wint_t
#define __need_NULL
#include <stddef.h>

#ifndef __ssize_t_defined
typedef signed long int ssize_t;
# define __ssize_t_defined
#endif
