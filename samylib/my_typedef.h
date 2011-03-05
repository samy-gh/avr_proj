
#ifndef __MY_TYPEDEF_H__
#define __MY_TYPEDEF_H__

typedef unsigned long  ULONG;
typedef unsigned int   UINT;
typedef unsigned short USHORT;
typedef unsigned char  UCHAR;
typedef long  LONG;
typedef int   INT;
typedef short SHORT;
typedef float FLOAT;
typedef double DOUBLE;
typedef UCHAR  BYTE;

#ifndef CHAR
#define CHAR char
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef BOOL
typedef enum {
	FALSE = 0,
	TRUE,
} BOOL;
#else
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#endif

#endif /* __MY_TYPEDEF_H__ */
