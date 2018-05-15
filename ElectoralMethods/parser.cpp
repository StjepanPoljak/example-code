#include "parser.h"

unsigned short Identify (char szString[], unsigned int uiSLoc)
{
	switch (szString[uiSLoc])
	{
	case ' ':
		return RAZMAK;
	case '+':
		return PLUS;
	case '-':
		return MINUS;
	case '.':
		return TOCKA;
	case ',':
		return ZAREZ;
	case '(':
		return LZAGRADA;
	case ')':
		return DZAGRADA;
	case NULL:
		return STRINGEND;
	case '[':
		return LKOCKASTA;
	case ']':
		return DKOCKASTA;
	case '*':
		return MNOZENJE;
	case '/':
		return SLASH;
	case '=':
		return JEDNAKO;
	case '^':
		return POTENCIJA;
	}

	if(((szString[uiSLoc]>=65)&&(szString[uiSLoc]<=90))||((szString[uiSLoc]>=97)&&(szString[uiSLoc]<=122))) return SLOVO;

	if((szString[uiSLoc]>=48)&&(szString[uiSLoc]<=57)) return BROJ;

	return OSTALO;
}

double StringToNumber (char szNumber[], unsigned int uiSize)
{
	unsigned short ushTemp=OSTALO;
	unsigned int uiDec=0;
	double dMultiply=1;
	double dNumTemp=0;
	bool bNegative=false;

	for(unsigned int i=0;i<uiSize;i++)
	{
		ushTemp=Identify(szNumber,i);
	
		if (ushTemp==MINUS) bNegative=true;

		if (ushTemp==TOCKA)
		{
			uiDec=i;
			break;
		}
		if (ushTemp==STRINGEND)
		{
			uiDec=uiSize-1;
			break;
		}
	}

	if(uiDec<(uiSize-1))
	{
		for(unsigned int i=(uiDec+1);i<(uiSize-1);i++)
		{
			dMultiply=dMultiply/10;
			dNumTemp=dNumTemp+(GetDigit(szNumber[i])*dMultiply);
		}
		dMultiply=1;
	}

	for(int i=(uiDec-1);i>=0;i--)
	{
		dNumTemp=dNumTemp+(GetDigit(szNumber[i])*dMultiply);
		dMultiply=dMultiply*10;
	}

	if (bNegative==false) return dNumTemp;
	else return -dNumTemp;
}

unsigned short GetDigit (char szChar)
{
	return (unsigned short)(szChar-48);
}

unsigned int CharStringLength (char szString[])
{
	for (unsigned int i=0; i < MAX_CHAR_LINE_LENGTH; i++)
	{	
		if(szString[i]==NULL) return i + 1;
	}

	return MAX_CHAR_LINE_LENGTH + 1;
}
