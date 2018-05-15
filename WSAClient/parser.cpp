#include "parser.h"

//VarListInit - inicijalizira vektor varijabli

void VarListInit()
{
    VarListSize=0;
    VarListPointer=-1;
    
    VarTypeList[0]="VAR";
    VarTypeList[1]="FLOAT";
    VarTypeList[2]="LONG";
    VarTypeList[3]="DOUBLE";
    VarTypeList[4]="STRING";
    VarTypeList[5]="INTEGER";
}

//VarDef - definira varijablu, pridruzuje joj ime i tip

void VarDef(string vName, string vType)
{
    VarListPointer=VarListPointer+1;
    if(VarListPointer>VarListSize-1)
    {
        VarListSize=(VarListPointer*2)+1;
    }
    varList.resize(VarListSize);
    varList[VarListPointer].varName=vName;
    varList[VarListPointer].varData="0";
    varList[VarListPointer].varType=vType;
}

//CommandListInit - inicijalizira popis naredbi

void CommandListInit()
{

    CommandList[0]="ParseFile";
    CommandList[1]="Input";
    CommandList[2]="Output";
    CommandList[3]="Add";
    CommandList[4]="Winsock2Init";
    CommandList[5]="Winsock2End";
    CommandList[6]="CreateListenSocket";
    CommandList[7]="ServerLoop";
    CommandList[8]="DestroyThread";
    CommandList[9]="RESERVED";
    CommandList[10]="RESERVED";
    CommandList[11]="ClientLoop";
    CommandList[12]="SendData";
    CommandList[13]="DefaultFormat";
    CommandList[14]="DataFormat";
    
}

//ParseString - aktivira string kao naredbu

void ParseString(string sString)
{

    stringstream ssNumToStr;    
    if(sString.size()==0) return;
    
    int i, iParamNum, iSelect;
    string sTempRes;
    
    vector<string> sParam(MAX_PARAM_NUM);
     
    iParamNum=ExtractString(sString,sParam);
    
    if((sParam[0].at(0) == *"/") && (sParam[0].at(1))==*"/")
    {
        return;
    }

    if(iParamNum>1)
    {

        //Ako je prvi parametar tip varijable, definiraj tu varijablu.
        
        for(i=0;i<VAR_TYPE_COUNT;i++)
        {
            if(sParam[0]==VarTypeList[i])
            {
                iSelect=i;

                VarDef(sParam[1],VarTypeList[i]);
            
                return;
            }
        }

        //Ako je jedan od parametara varijabla, pridruzi mu njenu vrijednost.

        int j;
 
        for(i=1;i<iParamNum;i++)
        {
            for(j=0;j<=VarListPointer;j++)
            {
                if(sParam[i]==varList[j].varName)
                {
                    sParam[i]=varList[j].varData;
                }
            }
        }
        
        //Ukoliko je parametar adresa varijable, ostavi ime.
        
        string sTempVarAddr;
        
        for(i=1;i<iParamNum;i++)
        {
            if(sParam[i].at(0)==*"&")
            {
                sTempVarAddr="";
                for(j=1;j<sParam[i].size();j++)
                {
                    sTempVarAddr=sTempVarAddr+sParam[i].at(j);
                }
                sParam[i]=sTempVarAddr;
            }
        }

        //Ako je prvi parametar ime varijable, a drugi znak jednakosti,
        //pridruzi varijabli vrijednost s desne strane.

        if((sParam[1]=="=") && (iParamNum==3))
        {
            for(i=0;i<=VarListPointer;i++)
            {
                if(sParam[0]==varList[i].varName)
                {
                    varList[i].varData=sParam[2];
                }
            }
        }
    }
    
    //Ako je prvi parametar jednak nekoj funkciji, pokreni tu funkciju.

    for(i=0;i<=COMMAND_LIST_COUNT-1;i++)
    {
        if(sParam[0]==CommandList[i])
        {
            iSelect=i;
            
            switch (iSelect)
            {
                case 0:
                    ParseFile(sParam[1]);
                    break;
                case 1:
                    getline(cin,sTempRes,'\n');
                    ValueToVar(sParam[1],sTempRes);
                    break;
                case 2:
                    cout << sParam[1];            
                    break;
                case 3:
                    break;
                case 4:
                    Winsock2Init();
                    break;
                case 5:
                    Winsock2End();
                    break;
                case 6:
                    CreateListenSocket(atoi(sParam[1].data()));
                    break;
                case 7:
                    if(sParam[1]=="THREAD")
                    {
                        if(CreateThread(NULL,0,ServerThread,NULL,0,&dwThread)==NULL)
                        {
                            ErrorHandle("Couldn't create thread.");
                            return;
                        }
                    }
                    else
                    {
                        ServerLoop();
                    }
                    break;
                case 8:
                    bRunning=false;
                    break;
                case 9:
                    break;
                case 10:
                    break;
                case 11:
                    if(sParam[4]=="THREAD")
                    {
                        sTAdd=sParam[1];
                        sTPrt=sParam[2];
                        sTUNe=sParam[3];
                        if(CreateThread(NULL,0,ClientThread,NULL,0,&dwThread)==NULL)
                        {
                            ErrorHandle("Couldn't create thread.");
                            return;
                        }
                    }
                    else if (sParam[4]=="NULL")
                    {
                        ClientLoop(sParam[1],atoi(sParam[2].data()),sParam[3]);
                    }
                    break;
                case 12:
                    if(sParam[2]=="hGenSock")
                    {
                        SendData(sParam[1],hGenSock);
                    }
                    else if(sParam[2]=="TO_ALL")
                    {
                        
                    }
                    else
                    {
                        
                    }
                    break;
                case 13:
                    sDefaultFormat=sParam[1];
                    break;
                case 14:
                    sTempRes=DataFormat(sParam[1]);
                    ValueToVar(sParam[2],sTempRes);
                    break;
                default:
                    return;
            }
            return;
        }
    }
     
}

//ExtractString - dijelove odvojene razmakom u stringu sprema
//u vektor sParamTmp i vraca njegovu velicinu.
//Ono sto je izmedju "[" i "]" sprema kao jedan element.

int ExtractString(string sString, vector<string> &sParamTmp)
{
    int i,j,k,l,iEqual;
    string sTemp, sEqual;
    string sCorrStr;
    bool bEqual;
     
    j=-1;
    k=-1;
    
    bEqual=false;
    sEqual="=";
    iEqual=0;
    sCorrStr="";
    
    //Ako postoji neporavnati znak jednakosti u liniji, poravnaj.

    for(i=1;i<sString.size()-1;i++)
    {
        if((sString.at(i)==*"=") && ((sString.at(i+1)!=*" ") && (sString.at(i-1)!=*" ")))
        {
            sEqual=" = ";
            iEqual=i;
            bEqual=true;
        }
        else if((sString.at(i)==*"=") && (sString.at(i-1)!=*" "))
        {
            sEqual=" =";
            iEqual=i;
            bEqual=true;
        }
        else if((sString.at(i)==*"=") && (sString.at(i+1)!=*" "))
        {
            sEqual="= ";
            iEqual=i;
            bEqual=true;
        }
        
    }
    
    if(bEqual==true)
    {
        for(i=0;i<sString.size();i++)
        {
            if(i!=iEqual)
            {
                sCorrStr=sCorrStr+sString.at(i);
            }
            else
            {
                sCorrStr=sCorrStr+sEqual;
            }
        }
        sString=sCorrStr;
    }
    
    //Ako naidjemo na zagrade, spremi sve izmedju njih u jedan parametar.
    
    for(i=0;i<sString.size();i++)
    {
        if (sString.at(i)==*"[")
        {
            for(l=i+1;l<sString.size();l++)
            {
                if (sString.at(l)==*"]")
                {
                    break;
                }
                else
                {
                    if(sString.at(l)==*"\\")
                    {
                        l=l+1;
                        if(sString.at(l)==*"r")
                        {
                            sTemp=sTemp+char(13);
                        }
                        else if(sString.at(l)==*"n")
                        {
                            sTemp=sTemp+char(10);
                        }
                    }
                    else
                    {
                        sTemp=sTemp + sString.at(l);
                    }
                }
            }
            
            j=j+1;
//            sParamTmp.reserve(j);
            sParamTmp[j]=sTemp.data();
            sTemp="";
//            sTemp.clear();
//            sTemp.resize(0);

            if(l<sString.size()-2)
            {
                i=l+2;
                k=-1;
            }
            else
            {
                return j+1;
            }
             
        }
        
        //Ako naidjemo na razmak, spremi sve prethodno u jedan parametar.
        
        if ((sString.at(i)==*" ")||(i==(sString.size()-1)))
        {
            j=j+1;

            if(i==(sString.size()-1)) sTemp=sTemp+sString.at(i);

            sParamTmp[j]=sTemp;
            sTemp="";
        }
        else
        {
            sTemp=sTemp + sString.at(i);
        }
    }

    return j+1;
}

//ParseFile - obradjuje datoteku

void ParseFile(string sFileName)
{
    FILE * hFile;
    string sTmpStr,sTmpSubStr;
    string sTmpValVar;
    bool bRepeat=false;
    int iRepParamNum, iRepCount;
    string sTerm, sTermVar;
    vector<string> sRepParam(MAX_PARAM_NUM);
    vector<string> sRepLine(MAX_REP_LINE);
    
    int iRepBgnLen, iRepEndLen, i, j, k;
    iRepBgnLen=sizeof("RepeatBegin");
    iRepEndLen=sizeof("RepeatEnd");
    
    char cTmpChr;
    hFile=fopen(sFileName.data(),"rt");
    if(hFile!=NULL)
    {
        sTmpStr="";
        cTmpChr=*"";
        while(cTmpChr!=EOF)
        {
            cTmpChr=fgetc(hFile);
            
            if((cTmpChr==*"\n")||(cTmpChr==EOF))
            {

                if(sTmpStr.size()>=iRepBgnLen-1)
                {
                    if(sTmpStr.substr(0,iRepBgnLen-1)=="RepeatBegin")
                    {
                        iRepParamNum=ExtractString(sTmpStr,sRepParam);
 
                        int iStrPoint;
                        
                        if(sRepParam[1]=="TERM")
                        {
                            iStrPoint=3;
                        }
                        else
                        {
                            iStrPoint=1;
                        }
                        for(i=iStrPoint;i<iRepParamNum;i++)
                        {
                            for(j=0;j<=VarListPointer;j++)
                            {
                                if(sRepParam[i]==varList[j].varName)
                                {
                                    sRepParam[i]=varList[j].varData;
                                }
                            }
                        }
                        
                        bRepeat=true;
                        j=0;
                        sTmpStr="";
                        if(sRepParam[1]=="TERM")
                        {
                            sTermVar=sRepParam[2];
                            sTerm=sRepParam[3];
                            iRepCount=-1;
                        }
                        else
                        {
                            iRepCount=atoi(sRepParam.at(1).data());
                        }

                    }
                }
                
                if(bRepeat==false)
                {
                    ParseString(sTmpStr);
                    sTmpStr="";
                }
                else
                {
                    
                    if(sTmpStr.substr(0,iRepEndLen-1)=="RepeatEnd")
                    {
                        bRepeat=false;
                        if(iRepCount>0)
                        {
                            for(k=1;k<=iRepCount;k++)
                            {
                                for(i=0;i<j;i++)
                                {
                                    ParseString(sRepLine.at(i));
                                }
                            }
                        }
                        else if(iRepCount==(-1))
                        {
                            bool bRepInf=true;
                            while(bRepInf==true)
                            {
                                for(i=0;i<j;i++)
                                {

                                    ParseString(sRepLine.at(i));
                                    sTmpValVar=RetVarValue(sTermVar);

                                    if(sTmpValVar==sTerm)
                                    {
                                        bRepInf=false;
                                    }
                                }
                            }
                        }
                        
                    }
                    else
                    {
                        j=j+1;
                        sRepLine.resize(j);
                        sRepLine[j-1]=sTmpStr;
                        sTmpStr="";
                    }
                    
                }

            }
            else
            {
                sTmpStr=sTmpStr+cTmpChr;
            }
        }
    }
}

//ValueToVar - pridruzuje vrijednost varijabli

void ValueToVar(string sVarName, string sValue)
{
    string sTempEq;
    sTempEq="";
    sTempEq=sTempEq + sVarName;
    sTempEq=sTempEq + " = ";
    sTempEq=sTempEq + "[" + sValue + "]";
    ParseString(sTempEq);
}

//RetVarValue - vraca vrijednost varijable

string RetVarValue(string sVarName)
{
    int l=0;
    for(l=0;l<=VarListPointer;l++)
    {
        if(varList[l].varName==sVarName)
        {
            return varList[l].varData;
        }
    }
}
