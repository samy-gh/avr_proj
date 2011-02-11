//-----------------------------------------------
//-	フォーマット出力処理			-
//-	printfのサブセットである、書式		-
//-	付きの出力を行なう			-
//-						-
//-	2009-05-18：				-
//-		Longデータ表示時のバグ修正	-
//-----------------------------------------------

#include "pr.h"
#include "ctype.h"
#include "cstdef.h"
#include <stdlib.h>



union	DATAS {
	float		fdat;
	unsigned long	dword;
	unsigned int	word[2];
	unsigned char	byte[4];
};

static union DATAS udat;
static unsigned char tbuf[20];
static unsigned char longflag;

static unsigned int slen(const unsigned char	*s)
{
	unsigned int	len;
	for (len=0; *s; len++,s++)
		;
	return(len);
}


static void print_str(const unsigned char *c)
{
	while(*c)
		print_chr(*c++);
}

static void print_pads(
	int		width,
	unsigned int	padchar)
{
	while(width-- > 0)
		print_chr(padchar);
}

static void print_val(unsigned int	width,
	  unsigned int  padchar,
	  unsigned int  base,
	  union	DATAS	data)
{
	unsigned int	datwidth;
	unsigned char	*p;
	p = tbuf;
	if (longflag) {		// 32bitデータ
		for (datwidth=0; data.dword>0; datwidth++) {
			if ((*p = (unsigned char )((data.dword % base) + '0')) > '9')
				*p += (unsigned char)('A'-'0'-10);
			p++;
			data.dword /= base;
		}
	}
	else {			// 16bitデータ
		for (datwidth=0; data.word[0]>0; datwidth++) {
			if ((*p = (unsigned char )((data.word[0] % base) + '0')) > '9')
				*p += (unsigned char)('A'-'0'-10);
			p++;
			data.word[0] /= base;
		}
	}
	if (datwidth == 0) {
		*p++ = '0';
		datwidth = 1;
	}
	print_pads(width-datwidth,padchar);
	while(datwidth--)
		print_chr(*--p);
}

static void prtout(
	const unsigned char	*string,
	unsigned int	*param)
{
	unsigned char padchar;
//	unsigned char *cp;
	unsigned int	width;
//	int	status;
	while(*string) {
		if (*string == LF) {
			print_chr(CR);
			print_chr(LF);
			string++;
			continue;
		}
		if (*string != '%') {
			print_chr(*string++);
			continue;
		}
		string++;
		padchar = *string;
		if (padchar == '0')
			string++;
		else	padchar = ' ';
		for (width = 0; isdigit(*string); width = width*10+(*string++ - '0'))
			;
		if (!width)
			width++;
		longflag = ((*string == 'L') || (*string == 'l'))?(1):(0);
		if (longflag ) {
				string++;
				udat.word[1] = *param--;
		} else	udat.word[1] = 0;
		switch(*string) {
			case	'b':
			case	'B':	udat.word[0] = *param--;
					print_val(width,padchar,2,udat);
					break;
			case	'o':
			case	'O':	udat.word[0] = *param--;
					print_val(width,padchar,8,udat);
					break;
			case	'u':
			case	'U':	udat.word[0] = *param--;
					print_val(width,padchar,10,udat);
					break;
			case	'd':
			case	'D':	udat.word[0] = *param--;
					print_val(width,padchar,10,udat);
					break;
			case	'x':
			case	'X':	udat.word[0] = *param--;
					print_val(width,padchar,16,udat);
					break;
#if 0
			case	'f':	if (width == 1)
						width = 15;	// ftoaのバッファサイズ
					udat.word[1] = *param--;
					udat.word[0] = *param--;
					cp = ftoa(udat.fdat, &status);
					if (status != 0) break;
					while(*cp != EOS) {
						if (!width) break;
						print_chr(*cp++);
						width--;
					}
					while(width--)
						print_chr(padchar);
					break;
#endif
			case	'c':	print_pads(width-1,' ');
					print_chr(*param--);
					break;
			case	's':	width -= slen((const unsigned char *)(*param));
					print_pads(width,' ');
					print_str((const unsigned char *)(*param));
					param--;
					break;
			default:	print_chr('%');
					print_chr(*string);
					break;
		}
		string++;
	}
}


void myprintf( const unsigned char *string, unsigned int arg)
{
	prtout(string, &arg);
}

