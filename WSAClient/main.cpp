#include "handle.h"
#include "wsaevent.h"
#include "parser.h"

using namespace std;

unsigned long IPTMP;


int main(int argc, char *argv[])
{
    VarListInit();
    CommandListInit();
    vector<string> sTest[10];
    if(argc>1)
    {
        string sTmpArg;
        sTmpArg="";
        sTmpArg=sTmpArg+argv[1];
        ParseFile(sTmpArg);
    }
    else
    {

    }
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
