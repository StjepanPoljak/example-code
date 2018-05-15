#include "cmath.h"
#include <graphics>

//vraca indeks maksimalnog elementa iz vektora fArray

unsigned int MaxElement (float fArray[], unsigned int uiArraySize)
{
	float fTemp;
	unsigned int iResult=0;

	if(uiArraySize>1) fTemp=fArray[0];
	else return 0;

	for(unsigned int i=1;i<uiArraySize;i++)
	{
		if (fArray[i]>fTemp)
		{
			iResult=i;
			fTemp=fArray[i];
		}
	}

	return iResult;
}

//vraca indeks maksimalnog elementa iz vektora fArray, ali gledajuci skup indeksa tSkupIndeksa

unsigned int MaxElementInd (float fArray[], unsigned int uiArraySize, tVektor tSkupIndeksa[], unsigned int uiSkupSize)
{
	float fTemp;
	unsigned int uiResult=0;

	if(uiArraySize>1) fTemp=fArray[0];
	else return 0;

	for(unsigned int i=1;i<uiSkupSize;i++)
	{
		if (tSkupIndeksa[i].bObrisan==true) continue;

		if (fArray[tSkupIndeksa[i].uiVrijednost-1]>fTemp)
		{
			uiResult=tSkupIndeksa[i].uiVrijednost-1;
			fTemp=fArray[tSkupIndeksa[i].uiVrijednost-1];
		}
	}

	return uiResult;
}

//Racuna iRoot-i korijen iz dNum uz preciznost iDecAcc
double MAT_ZRoot (double dNum, int iRoot, int iDecAcc)
{
	int i=0;
	double dStart=0.1;
	double dResult=1;

	if (dNum==0) return 0;

	while(true)
	{
		if (i>0) dStart=dResult;
		dResult=MAT_ZPower(double(iRoot),-1)*((iRoot-1)*dResult+dNum/MAT_ZPower(double(dResult),iRoot-1));
		dResult=MAT_DecAcc(dResult,iDecAcc);
		if (dStart==dResult) break;
		i++;
	}

	return dResult;
}

//Zaokruzuje dNumber na iDecPlaces decimalnih mjesta.
double MAT_DecAcc (double dNumber, int iDecPlaces)
{
	double dTemp = dNumber;
	
	for(int i=0;i<iDecPlaces;i++)
	{
		dTemp=dTemp*10;
	}
	
	dTemp=MAT_Floor(dTemp, 2);

	for(int i=0;i<iDecPlaces;i++)
	{
		dTemp=dTemp/10;
	}

	return dTemp;
}

//Racuna dNumber na cjelobrojnu potenciju iPower.
double MAT_ZPower(double dNumber, int iPower)
{
	if (iPower < 0)
	{
		dNumber=double(1)/dNumber;
		iPower=(int)MAT_Absolute(iPower);
	}

	if (dNumber==1) return 1;
	if (dNumber==0) return 0;
	if (dNumber==-1)
	{
		if ((iPower%2)==0) return 1;
		else return -1;
	}
	if (iPower == 1) return dNumber;
	if (iPower == 0) return 1;

	if (iPower % 2 == 1) return MAT_ZPower(dNumber, iPower/2)*MAT_ZPower(dNumber, iPower/2)*dNumber;
	else return MAT_ZPower(dNumber, iPower/2)*MAT_ZPower(dNumber, iPower/2);

}

//Vraca apsolutnu vrijednost realnog broja dNum.
double MAT_Absolute (double dNum)
{
	if (dNum>=0) return dNum;
	else return -dNum;
}

//Vraca cijelobrojni dio realnog broja dNum, brzinom dOrder.
double MAT_Floor (double dNum, double dOrder)
{
	double dResult=0;
	double dTemp=0;
	double dPowTemp=1;

	while (true)
	{
		dPowTemp=dPowTemp*dOrder;
		dTemp=dResult+dPowTemp;
		if(MAT_Absolute(dNum)<dTemp)
		{
			break;
			dPowTemp=dPowTemp/dOrder;
		}
		else
		{
			dResult=dTemp;
			dPowTemp=dPowTemp*dOrder;
		}
	}

	while (true)
	{
		dTemp=dResult+dPowTemp;
		if(dTemp>MAT_Absolute(dNum))
		{
			dPowTemp=dPowTemp/dOrder;
			continue;
		}
		if(((MAT_Absolute(dNum)-dTemp)<1) && ((MAT_Absolute(dNum)-dTemp)>=0)) break;
		else dResult=dTemp;
	}

	if(dNum < 0) return -dTemp;
	else return dTemp;
}
