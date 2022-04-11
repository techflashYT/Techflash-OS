typedef unsigned   long int    size_t;
#ifdef SIZE_T_MAX  
#undef SIZE_T_MAX  
#endif             
#define SIZE_T_MAX             0xffffffffffffffffUL
                   
typedef unsigned   long int    uint64_t;
#ifdef UINT64_MAX  
#undef UINT64_MAX  
#endif             
#define UINT64_MAX             0xffffffffffffffffUL
typedef unsigned   int         uint32_t;
#ifdef UINT32_MAX  
#undef UINT32_MAX  
#endif             
#define UINT32_MAX             0xffffffffU
typedef unsigned   short       uint16_t;
#ifdef UINT16_MAX  
#undef UINT16_MAX  
#endif             
#define UINT16_MAX             0xffff
typedef unsigned   char        uint8_t;
#ifdef UINT8_MAX   
#undef UINT8_MAX   
#endif             
#define UINT8_MAX              0xff
                   
typedef signed     long int    int64_t;
#ifdef INT64_MAX   
#undef INT64_MAX   
#endif             
#define INT64_MAX              0x7fffffffffffffffL
typedef signed     int         int32_t;
#ifdef INT32_MAX   
#undef INT32_MAX   
#endif             
#define INT32_MAX              0x7fffffff
typedef signed     short       int16_t;
#ifdef INT16_MAX   
#undef INT16_MAX   
#endif             
#define INT16_MAX              0x7fff
typedef signed     char        int8_t;
#ifdef INT8_MAX    
#undef INT8_MAX    
#endif             
#define INT8_MAX               0x7f

#ifdef NULL
#undef NULL
#endif
#define NULL      ((void*)0)
#ifdef NULLPTR
#undef NULLPTR
#endif
#define NULLPTR  *((void*)0)


#ifdef stdout
#undef stdout
#endif
#define stdout 0x00
#ifdef stderr
#undef stderr
#endif
#define stderr 0x01
#ifdef stdin
#undef stdin
#endif
#define stdin 0x02
