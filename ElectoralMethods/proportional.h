#ifndef _PROPORTIONAL_H
#define _PROPORTIONAL_H

//#define MAX_BROJ_STRANKI 1000;
//#define MAX_BROJ_MJESTA 1000;

const unsigned int MAX_BROJ_STRANKI=1000;
//const unsigned int MAX_BROJ_MJESTA=1000;

#define SMALLEST_DIVISORS 1
#define DANISH 2
#define HARMONIC_MEAN 3
#define EQUAL_PROPORTIONS 4
#define SAINT_LAGUE 5
#define MOD_SAINT_LAGUE 6
#define DHONDT 7
#define BELGIAN 8

#define LAR 1
#define DRO 2
#define IMP 3

void LargestRemainders (unsigned int, unsigned int, unsigned int [], unsigned int [], unsigned short);
void DivisorMethod (unsigned int, unsigned int, unsigned int [], unsigned int [], unsigned short);
float DivisorCriterion (unsigned int, unsigned short);

#endif
