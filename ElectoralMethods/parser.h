#ifndef _PARSER_H
#define _PARSER_H

#define NULL 0

const unsigned int MAX_CHAR_LINE_LENGTH=1000;

#define LZAGRADA	11
#define DZAGRADA	12
#define ZAREZ		13
#define TOCKA		14
#define SLASH		15
#define MNOZENJE	16
#define PLUS		17
#define MINUS		18
#define JEDNAKO		19
#define LKOCKASTA	20
#define DKOCKASTA	21
#define BROJ		22
#define SLOVO		23
#define RAZMAK		24
#define OSTALO		25
#define STRINGEND	26
#define POTENCIJA	27
#define NAVODNICI	28

unsigned short Identify (char [], unsigned int);
double StringToNumber (char [], unsigned int);
unsigned short GetDigit (char);
unsigned int CharStringLength (char []);

#endif
