/*====================================================================*
 *
 *   number.h
 *
 *.  published 2003 by charles maier associates limited for internal use;
 *:  modified sun 07 sep 03 by user root on host zeus;
 *;  compiled for debian gnu/linux using gcc compiler;
 *
 *--------------------------------------------------------------------*/

#ifndef NUMBER_HEADER
#define NUMBER_HEADER
 
/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
 
/*====================================================================*
 *   numeric converson sizes and limits;
 *--------------------------------------------------------------------*/

#define RADIX_BIN 2
#define RADIX_OCT 8
#define RADIX_DEC 10
#define RADIX_HEX 16

#define RADIX_MIN 2
#define RADIX_MAX 36
 
#define BIN_DIGITS 8
#define DEC_DIGITS 3
#define OCT_DIGITS 3
#define HEX_DIGITS 2

#define IPv4_SIZE 4
#define IPv6_SIZE 16

/*====================================================================*
 *   numeric conversion strings;
 *--------------------------------------------------------------------*/

#define DIGITS_BIN "01" 
#define DIGITS_OCT "01234567" 
#define DIGITS_DEC "0123456789" 
#define DIGITS_HEX "0123456789ABCDEF" 
#define DIGITS_ALL "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
 
/*====================================================================*
 *   numeric string extenders;
 *--------------------------------------------------------------------*/

#define BIN_EXTENDER '-'
#define DEC_EXTENDER '.'
#define HEX_EXTENDER ':'
#define CHR_EXTENDER '.'
 
/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#define isodd(n)  (((n)&(1)) != 0)
#define iseven(n) (((n)&(1)) == 0)
 
/*====================================================================*
 *
 *--------------------------------------------------------------------*/
 
uint64_t basespec (const char * string, unsigned base, unsigned size);
uint64_t uintspec (const char * string, uint64_t minimum, uint64_t maximum);
uint64_t tonumber (const char * string, uint64_t minimum, uint64_t maximum, uint64_t nominal);
signed sintspec (const char *string, signed number);
unsigned totruth (const char *string, signed fail);
unsigned todigit (unsigned c);

/*====================================================================*
 *   end definitions;
 *--------------------------------------------------------------------*/

#endif
 

