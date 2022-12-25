#define LOGLEVEL_DEBUG 0
#define LOGLEVEL_INFO  1
#define LOGLEVEL_WARN  2
#define LOGLEVEL_ERROR 3
void log(const char *module, const char* message, uint8_t logLevel);