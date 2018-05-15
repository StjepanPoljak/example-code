#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

const int COMMAND_LIST_COUNT=15;
const int MAX_PARAM_NUM=10;
const int VAR_TYPE_COUNT=6;
const int MAX_REP_LINE=100;
vector<string> VarTypeList(VAR_TYPE_COUNT);
vector<string> CommandList(COMMAND_LIST_COUNT);
void ParseFile(string);
void ValueToVar(string, string);
string RetVarValue(string);

void CommandListInit();
int ExtractString(string, vector<string>&);
//void ParseString(string);
void VarListInit();
void VarDef();

struct _variable
{
    string varName;
    string varData;
    string varType;
};

int VarListSize, VarListPointer;
vector<_variable> varList;
