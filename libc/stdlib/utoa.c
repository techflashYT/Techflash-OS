#include <stdint.h>
#include <stdlib.h>


char* utoa(unsigned long value, volatile char* result, int base){

        char buff[sizeof(long)*8/4];

        if(base < 2 || base > 36){
                *result = '\0';
                return result;
        }

        size_t charc = 0;

        do{
                uintmax_t offset = value % base;
                value /= base;
                buff[charc++] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[offset];
        }
        while(value);
        
        for(uintmax_t i = 0; i < charc; ++i){

                result[i] = buff[charc-i-1];
                
        }

        result[charc] = '\0';
        
        return result;
}

