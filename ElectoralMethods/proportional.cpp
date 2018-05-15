#include "proportional.h"
#include "cmath.h"

//Metoda najvecih ostataka

void LargestRemainders (unsigned int uiMjesta, unsigned int uiStranke, unsigned int uiGlasovi[], unsigned int uiDodjela[], unsigned short usMetoda)
{
	tVektor tJ[MAX_BROJ_STRANKI];
	float fQuotArray[MAX_BROJ_STRANKI];
	unsigned int uiUkupno=0;
	float fOstatak[MAX_BROJ_STRANKI];
	unsigned int uiSuma=0;
	unsigned int uiIndex=0;

	//racunamo:
	//uiUkupno => zbroj svih glasova
	//uiJ => vektor koji sadrzi brojeve 1,...,uiStranke

	for (unsigned int i=0;i<uiStranke;i++)
	{
		uiUkupno=uiUkupno+uiGlasovi[i];
		tJ[i].uiVrijednost=i+1;
		tJ[i].bObrisan=false;
	}

	//racunamo kvocijente i ostatke po formuli, te dodamo mjesta po ostatcima
	for (unsigned int i=0;i<uiStranke;i++)
	{
		switch(usMetoda)
		{
		case LAR:
			fQuotArray[i]=uiGlasovi[i]*(float(uiMjesta)/float(uiUkupno));
			break;
		case DRO:
			fQuotArray[i]=float(uiGlasovi[i])/float(float(uiUkupno)/float(uiMjesta+1)+1);
			break;
		case IMP:
			fQuotArray[i]=uiGlasovi[i]*(float(uiMjesta+2)/float(uiUkupno));
			break;
		}
		
		fOstatak[i]=fQuotArray[i]-int(fQuotArray[i]);
		
		uiDodjela[i]=int(fQuotArray[i]);
		uiSuma=uiSuma+uiDodjela[i];
	}

	//dodjela preostalih mjesta
	while(true)
	{
		if ((uiMjesta-uiSuma)==0) break;
		else
		{
			uiIndex=MaxElementInd(fOstatak,uiStranke,tJ,uiStranke);
			uiDodjela[uiIndex]++;
			uiSuma++;
			tJ[uiIndex].bObrisan=true;
		}
	}

	return;
}

//Metoda divizora

void DivisorMethod (unsigned int uiMjesta, unsigned int uiStranke, unsigned int uiGlasovi[], unsigned int uiDodjela[], unsigned short usMetoda)
{
	
	float fQuotArray[MAX_BROJ_STRANKI];
	bool bZero=false;
	unsigned int uiSuma=0;
	unsigned int uiIndex=0;

	float fDivisorTemp=0;

	//racunamo pocetne omjere za svaku stranku
	for (unsigned int i=0;i<uiStranke;i++)
	{
		uiDodjela[i]=0;

		fDivisorTemp=DivisorCriterion(uiDodjela[i],usMetoda);

		//ako smo kod bilo koje stranke imali dijeljenje s nulom, zapamtimo to
		if (fDivisorTemp==0)
		{
			bZero=true;
			continue;
		}
		else fQuotArray[i]=float(uiGlasovi[i])/fDivisorTemp;
	}

	//ako smo imali dijeljenje s nulom, racunamo nove kvocijente, a svakoj stranci dodajemo po jedno mjesto
	if (bZero==true)
	{
		for (unsigned int i=0;i<uiStranke;i++)
		{
			uiDodjela[i]++;
			uiSuma++;
			fQuotArray[i]=float(uiGlasovi[i])/DivisorCriterion(uiDodjela[i],usMetoda);
		}
	}

	//dodjeljivanje mjesta po algoritmu
	while(true)
	{
		uiSuma++;
		uiIndex=MaxElement(fQuotArray,uiStranke);
		uiDodjela[uiIndex]++;
		fQuotArray[uiIndex]=float(uiGlasovi[uiIndex])/DivisorCriterion(uiDodjela[uiIndex],usMetoda);
		if ((uiMjesta-uiSuma)==0) break;
	}

	return;
}

//racunanje divizora

float DivisorCriterion (unsigned int uiSeatNum, unsigned short usMethod)
{
	switch(usMethod)
	{
	case SMALLEST_DIVISORS:
		return float(uiSeatNum);
	case DANISH:
		return float(uiSeatNum*3+1);
	case HARMONIC_MEAN:
		return float(2*uiSeatNum*(uiSeatNum+1))/float(2*uiSeatNum+1);
	case EQUAL_PROPORTIONS:
		return float(MAT_ZRoot(float(uiSeatNum*(uiSeatNum+1)),2,MAX_ACCURACY));
	case SAINT_LAGUE:
		return float(1+2*uiSeatNum);
	case MOD_SAINT_LAGUE:
		if (uiSeatNum==0) return 1.4;
		else return float(1+2*(uiSeatNum));
	case DHONDT:
		return float(uiSeatNum+1);
	case BELGIAN:
		return float(1+float(uiSeatNum)/float(2));
	}

	return 0;
}
