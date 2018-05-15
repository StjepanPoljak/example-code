#include <iostream>
#include "proportional.h"
#include "parser.h"

using namespace std;

int main()
{
	char szInput[MAX_CHAR_LINE_LENGTH];	
	int iInput=0;
	float fInput=0;

	unsigned int uiMjesta=26;
	unsigned int uiStranke=5;
	unsigned int uiGlasovi[MAX_BROJ_STRANKI];
	unsigned int uiRezultat[MAX_BROJ_STRANKI];
	unsigned int uiProvjera=0;

	unsigned short usOdabir=0;

	uiGlasovi[0]=1182;
	uiGlasovi[1]=3319;
	uiGlasovi[2]=5259;
	uiGlasovi[3]=7179;
	uiGlasovi[4]=9061;

	cout << "Razmjerne izborne metode\n";
	cout << "------------------------\n\n";
	cout << "1. Input\n";
	cout << "2. Quotient Method\n";
	cout << "3. Divisor Method\n";
	cout << "4. Output\n";
	cout << "5. Exit\n";
		
	while(true)
	{
		cout << "\n>";
		
		cin >> szInput;

		cout << "\n";

		iInput=int(StringToNumber(szInput,CharStringLength(szInput)));

		if (iInput==5) break;
		if ((iInput==0)||(iInput>5))
		{
			cout << "Razmjerne izborne metode\n";
			cout << "------------------------\n\n";
			cout << "1. Input\n";
			cout << "2. Quotient Method\n";
			cout << "3. Divisor Method\n";
			cout << "4. Output\n";
			cout << "5. Exit\n";
		}

		switch(iInput)
		{
		case 1:
			cout << "Broj mjesta: ";
			cin >> szInput;
			uiMjesta=unsigned int(StringToNumber(szInput,CharStringLength(szInput)));
			cout << "\nBroj stranki: ";
			cin >> szInput;
			uiStranke=unsigned int(StringToNumber(szInput,CharStringLength(szInput)));
			cout << "\nUnos glasova:";
			cout << "\n-------------\n";
			for(unsigned int i=0;i<uiStranke;i++)
			{
				cout << "Stranka [" << i << "]: ";
				cin >> szInput;
				uiGlasovi[i]=unsigned int(StringToNumber(szInput,CharStringLength(szInput)));
			}
			break;
		case 2:
			cout << "Quotient method:\n";
			cout << "----------------\n";
			cout << "1. Largest Remainders (LAR)\n";
			cout << "2. Droop (DRO)\n";
			cout << "3. Imperiali (IMP)\n";
			cout << "\n>";
			cin >> szInput;
			usOdabir=unsigned short(StringToNumber(szInput,CharStringLength(szInput)));
			LargestRemainders(uiMjesta,uiStranke,uiGlasovi,uiRezultat,usOdabir);
			cout << "\n";
			uiProvjera=0;

			for(unsigned int i=0;i<uiStranke;i++)
			{
				uiProvjera=uiProvjera+uiRezultat[i];
				cout << "Stranka " << i+1 << ": " << uiRezultat[i] << "\n";
			}

			cout << "\nUkupno dodijeljenih mjesta: " << uiProvjera << "\n";
			break;
		case 3:
			cout << "Divisor method:\n";
			cout << "---------------\n";
			cout << "1. Smallest Divisors (SD)\n";
			cout << "2. Danish (DA)\n";
			cout << "3. Harmonic Mean (HM)\n";
			cout << "4. Equal Proportions (EP)\n";
			cout << "5. Saint-Lague (SL)\n";
			cout << "6. Modified Saint-Lague (MS)\n";
			cout << "7. d'Hondt (DH)\n";
			cout << "8. Belgian (BE)\n";
			cout << "\n>";
			cin >> szInput;
			usOdabir=unsigned short(StringToNumber(szInput,CharStringLength(szInput)));
			DivisorMethod(uiMjesta,uiStranke,uiGlasovi,uiRezultat,usOdabir);
			cout << "\n";
			uiProvjera=0;

			for(unsigned int i=0;i<uiStranke;i++)
			{
				uiProvjera=uiProvjera+uiRezultat[i];
				cout << "Stranka " << i+1 << ": " << uiRezultat[i] << "\n";
			}

			cout << "\nUkupno dodijeljenih mjesta: " << uiProvjera << "\n";

			break;

		case 4:
			uiProvjera=0;

			for(unsigned int i=0;i<uiStranke;i++)
			{
				uiProvjera=uiProvjera+uiRezultat[i];
				cout << "Stranka " << i+1 << ": " << uiRezultat[i] << "\n";
			}

			cout << "\nUkupno dodijeljenih mjesta: " << uiProvjera << "\n";

			break;
		}

	}

	system("PAUSE");
	return EXIT_SUCCESS;
}

