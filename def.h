#ifndef _DEF_H_
#define _DEF_H_

#include <intrins.h>

/////////////////////////////////////////////////////////////////////
// ** defined type
typedef 	unsigned char	  u8; 		  /* 1 bytes */
typedef 	char			  c8; 		  /* 1 bytes */
typedef 	unsigned int	  u16; 		  /* 2 bytes */
typedef 	short int		  s16;			  /* 2 bytes */
typedef 	unsigned long	  u32;		  /* 4 bytes */
typedef 	long			  s32; 		  /* 4 bytes */

typedef int BOOL;
#define TRUE 1
#define FALSE 0
#ifndef true
#define  true TRUE
#endif
#ifndef false
#define  false FALSE
#endif

#ifndef null
#define null NULL
#endif


// ** defined defines

#define 	LOW 		0
#define 	HIGH		1

#define 	SUCCESS 	1
#define 	ERROR		0

#define 	SET 		1
#define 	CLEAR		0

#define 	ON			1
#define 	OFF 		0

#define	STOP		0
#define	EXCUTION		1
#define	NON		0

#define data	_data
#define pdata	_pdata
//#define xdata	_xdata

#define cli()		do{IEN0 &= ~0x80;}while(0)
#define sei()		do{IEN0 |=  0x80;}while(0)


#endif	// _DEF_H_
