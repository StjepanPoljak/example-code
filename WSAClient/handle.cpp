#include "handle.h"

void ErrorHandle(string sErrorMessage)
{
    cout << sErrorMessage << "\n\n";
}

void MessageHandle(string sMessage)
{
    cout << sMessage << "\n\n";
}

string NumberToString(long lNumber)
{
        ostringstream ssStringTemp;
        ssStringTemp << lNumber;
        return(ssStringTemp.str());
}

int Add(int a, int b)
{
    cout << "ZBROJ: " << a+b << endl;
    return a+b;
}
