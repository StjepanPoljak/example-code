#ifndef _CMATH_H
#define _CMATH_H

#define MAX_ACCURACY 10

typedef struct _Vektor
{
	bool bObrisan;
	unsigned int uiVrijednost;
} tVektor, *tpVektor;

unsigned int MaxElement (float [], unsigned int);
unsigned int MaxElementInd (float [], unsigned int, tVektor [], unsigned int);

double MAT_DecAcc (double, int);
double MAT_ZPower (double, int);
double MAT_Absolute (double);
double MAT_Floor (double, double);
double MAT_ZRoot (double, int, int);

#endif
