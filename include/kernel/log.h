#include <stdint.h>
// info that will almost never be useful except for when initially creating the code.
#define LOGLEVEL_VERBOSE 0
// info that could potentially be useful if having issues
#define LOGLEVEL_DEBUG   1
// info that could potentially be useful to an average user
#define LOGLEVEL_INFO    2
// something unexpected happened, this is a warning the user
#define LOGLEVEL_WARN    3
// something bad happened, we need to tell the user
#define LOGLEVEL_ERROR   4
// something EXTREMELY bad happened, this will usually be extended info just before a kernel panic.
#define LOGLEVEL_FATAL   5
extern void log(const char module[8], const char *msg, const uint8_t level);