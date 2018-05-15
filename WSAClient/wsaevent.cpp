#include "wsaevent.h"

//Winsock2Init - inicijalizira Winsock2

int Winsock2Init()
{
    int iError;

    iError=WSAStartup(MAKEWORD(NEEDED_VERSION_LOBYTE,NEEDED_VERSION_HIBYTE),&pWinsockData);

    if(iError==0)
    {
        if(LOBYTE(pWinsockData.wVersion) < NEEDED_VERSION_LOBYTE)
        {
            ErrorHandle("Required winsock version not found.");
            Winsock2End();
            return 0;
        }
        else
        {
            iMailArraySize=0;
            iMailNum=0;
            bClient=false;
            bServer=false;
            bOnline=false;
            return 1;
        }
    }
    else
    {
        ErrorHandle("Winsock initialization failed.");
        return 0;    
    }
}


//Winsock2End - zatvara Winsock2

int Winsock2End()
{
    if (WSACleanup()==0)
    {
        return 1;
    }
    else
    {
        ErrorHandle("Winsock cleanup failed.");
        return 0;
    }
}

//CreateListenSocket - stvara socket koji ce primati konekcije

int CreateListenSocket(unsigned short ushPort)
{

    string sMessageTemp;
    int iError;

    hGenSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(hGenSock==INVALID_SOCKET)
    {
        ErrorHandle("Creating listening socket failed.");
        return 0;
    }

    if(CreateSocketInformation(hGenSock,ushPort)==0)
    {
        return 0;   
    }

    iError=WSAEventSelect(hGenSock,wsaEventArray[iEventNum-1],FD_ACCEPT | FD_CLOSE);
    if(iError==SOCKET_ERROR)
    {
        ErrorHandle("Error with selecting event.");
        return 0;
    }

    pInternetAddress.sin_family=AF_INET;
    pInternetAddress.sin_port=htons(ushPort);
    pInternetAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    
    iError=bind(hGenSock,reinterpret_cast<sockaddr*>(&pInternetAddress),sizeof(pInternetAddress));
    if(iError==1)
    {
        ErrorHandle("Error binding socket.");
        return 0;
    }
    
    iError=listen(hGenSock,5);
    if(iError==1)
    {
        ErrorHandle("Unable to put socket in listening mode.");
        return 0;
    }
    
    sMessageTemp="Server online and listening on port ";
    sMessageTemp=sMessageTemp+NumberToString(ushPort);
    sMessageTemp=sMessageTemp+".";
    MessageHandle(sMessageTemp);
    bServer=true;
    bClient=false;
    bOnline=true;
    return 1;

}

//CreateSocketInformation - stvara informacije o odredjenom socketu

int CreateSocketInformation(SOCKET hSocketTemp,in_addr *addrIPTmp,unsigned short ushPortTmp)
{
    tpSocketInformation tpSockInfoTmp;
    
    wsaEventArray[iEventNum]=WSACreateEvent();
    if(wsaEventArray[iEventNum]==WSA_INVALID_EVENT)
    {
        ErrorHandle("Invalid event.");
        return 0;
    }
    
    tpSockInfoTmp=(tpSocketInformation)GlobalAlloc(GPTR,sizeof(tSocketInformation));
    if(tpSockInfoTmp==NULL)
    {
        ErrorHandle("Socket allocation failed.");
        return 0;
    }
    
    tpSockInfoTmp->hSocket=hSocketTemp;
    tpSockInfoTmp->iReceived=0;
    tpSockInfoTmp->iSent=0;
    tpSockInfoTmp->addrIPAddress=addrIPTmp;
    tpSockInfoTmp->ushPortNumber=ushPortTmp;
    tpSockInfoTmp->bAuthorized=false;
    strcpy(tpSockInfoTmp->cUserName,"Anonymus");
        
    tpSocket[iEventNum]=tpSockInfoTmp;
    iEventNum=iEventNum+1;
    
    return 1;
    
}

int CreateSocketInformation(SOCKET hSocketTemp,unsigned short ushPortTmp)
{
    tpSocketInformation tpSockInfoTmp;
    
    wsaEventArray[iEventNum]=WSACreateEvent();
    if(wsaEventArray[iEventNum]==WSA_INVALID_EVENT)
    {
        ErrorHandle("Invalid event.");
        return 0;
    }
    
    tpSockInfoTmp=(tpSocketInformation)GlobalAlloc(GPTR,sizeof(tSocketInformation));
    if(tpSockInfoTmp==NULL)
    {
        ErrorHandle("Socket allocation failed.");
        return 0;
    }
    
    tpSockInfoTmp->hSocket=hSocketTemp;
    tpSockInfoTmp->iReceived=0;
    tpSockInfoTmp->iSent=0;
    tpSockInfoTmp->ushPortNumber=ushPortTmp;
    tpSockInfoTmp->bAuthorized=false;
    strcpy(tpSockInfoTmp->cUserName,"Anonymus");
    
    tpSocket[iEventNum]=tpSockInfoTmp;
    iEventNum=iEventNum+1;
    
    return 1;
    
}

//ServerLoop - glavna petlja za server

int ServerLoop()
{
    
    SOCKET hAcceptTemp;
    int iError;
    bRunning=true;
   
    sockaddr_in pClientSockTmp;
    int pClientSockSize=sizeof(pClientSockTmp);
    unsigned long ulIPNumber;
    unsigned short ushPort;
    tpSocketInformation tpSockTemp;
    string sMessageTemp,sUNTemp;
    DWORD iRcvTemp, iSndTemp, iFlags;
    
    int i;
    
    while(bRunning==true)
    {

        iEvent=WSAWaitForMultipleEvents(iEventNum,wsaEventArray,false,WSA_INFINITE,false);
        if(iEvent==WSA_WAIT_FAILED)
        {
            ErrorHandle("Wait function failed.");
            return 0;
        }

        if(bRunning==false) return 1;
        
        iError=WSAEnumNetworkEvents(tpSocket[iEvent-WSA_WAIT_EVENT_0]->hSocket,wsaEventArray[iEvent-WSA_WAIT_EVENT_0],&wsaNetEvent);
        if(iError==SOCKET_ERROR)
        {
            ErrorHandle("Error enumerating network events.");
            return 0;            
        }
        
        // prihvacanje veze
        
        if(wsaNetEvent.lNetworkEvents & FD_ACCEPT)
        {
            if(wsaNetEvent.iErrorCode[FD_ACCEPT_BIT]!=0)
            {
                ErrorHandle("Error in wsaNetEvent.");
                return 0;
            }

            hAcceptTemp=accept(tpSocket[iEvent-WSA_WAIT_EVENT_0]->hSocket,reinterpret_cast<sockaddr*>(&pClientSockTmp),&pClientSockSize);
        
            if(hAcceptTemp==INVALID_SOCKET)
            {
                ErrorHandle("Unable to accept client connection.");
                return 0;
            }
            else
            {
                ushPort=pClientSockTmp.sin_port;
            }
            
            if(iEventNum>WSA_MAXIMUM_WAIT_EVENTS)
            {
                closesocket(hAcceptTemp);
                ErrorHandle("Too many connections.");
                return 0;
            }
            
            if(CreateSocketInformation(hAcceptTemp,&pClientSockTmp.sin_addr,ushPort)==0)
            {
                return 0;   
            }

            iError=WSAEventSelect(hAcceptTemp,wsaEventArray[iEventNum-1],FD_READ|FD_WRITE|FD_CLOSE);
            if(iError==SOCKET_ERROR)
            {
                ErrorHandle("Error with selecting event.");
                return 0;
            }
            
            sMessageTemp="";
            sMessageTemp=sMessageTemp + "Connection accepted: ";
            sMessageTemp=sMessageTemp + inet_ntoa(pClientSockTmp.sin_addr);
            sMessageTemp=sMessageTemp + ":";
            sMessageTemp=sMessageTemp + NumberToString(ushPort);
            MessageHandle(sMessageTemp);

        }
        
        // primanje podataka
            
        if(wsaNetEvent.lNetworkEvents & FD_READ)
        {
            if(wsaNetEvent.lNetworkEvents & FD_READ && wsaNetEvent.iErrorCode[FD_READ_BIT]!=0)
            {
                ErrorHandle("Read error.");
                return 0;
            }
                        
            iFlags=0;

            ZeroMemory(&tpSocket[iEvent-WSA_WAIT_EVENT_0]->cReadBuffer,sizeof(tpSocket[iEvent-WSA_WAIT_EVENT_0]->cReadBuffer));

            tpSocket[iEvent-WSA_WAIT_EVENT_0]->wsaReadBuffer.buf=tpSocket[iEvent-WSA_WAIT_EVENT_0]->cReadBuffer;
            tpSocket[iEvent-WSA_WAIT_EVENT_0]->wsaReadBuffer.len=BUFFER_SIZE;

            if(WSARecv(tpSocket[iEvent-WSA_WAIT_EVENT_0]->hSocket,&(tpSocket[iEvent-WSA_WAIT_EVENT_0]->wsaReadBuffer), 1, &iRcvTemp, &iFlags, NULL, NULL)==SOCKET_ERROR)
            {
                if(WSAGetLastError()!=WSAEWOULDBLOCK)
                {
                    ErrorHandle("Receiving data failed.");
                    FreeSocketInformation(iEvent-WSA_WAIT_EVENT_0);
                    return 0;
                }
            }
            else
            {
                tpSocket[iEvent-WSA_WAIT_EVENT_0]->iReceived=iRcvTemp;
                sMessageTemp="";
                sMessageTemp=sMessageTemp + tpSocket[iEvent-WSA_WAIT_EVENT_0]->wsaReadBuffer.buf;
                AnalyzeData(sMessageTemp);
            }
        }

        // slanje podataka

        if (wsaNetEvent.lNetworkEvents & FD_WRITE)
        {

        }
        
        // zatvaranje veze
        
        if(wsaNetEvent.lNetworkEvents & FD_CLOSE)
        {

            sMessageTemp="";
            sMessageTemp=sMessageTemp + "Connection closed: ";
            sMessageTemp=sMessageTemp + inet_ntoa(*tpSocket[iEvent-WSA_WAIT_EVENT_0]->addrIPAddress);
            sMessageTemp=sMessageTemp + ":";
            sMessageTemp=sMessageTemp + NumberToString(tpSocket[iEvent-WSA_WAIT_EVENT_0]->ushPortNumber);
            MessageHandle(sMessageTemp);
            
            iError=FreeSocketInformation(iEvent-WSA_WAIT_EVENT_0);
            if(iError==0)
            {
                MessageHandle("Warning! Connection may not be closed properly.");
            }
          
        }

    }
        
}

//GetHostIP - vraca IP dane adrese

unsigned long GetHostIP(string sHostName)
{
    HOSTENT * pHostData;
    pHostData=gethostbyname(sHostName.data());
    if(pHostData==NULL)
    {
        ErrorHandle("Error resolving hostname.");
        return 0;
    }
    else
    {
        if(pHostData->h_addr_list && pHostData->h_addr_list[0])
        {
            return *reinterpret_cast<unsigned long*>(pHostData->h_addr_list[0]);
        }
    }
}

//FreeSocketInformation - brise informacije o odredjenom socketu

int FreeSocketInformation(int iEventTemp)
{
    tpSocketInformation tpSockInfoTmp=tpSocket[iEventTemp];
    int i;

    closesocket(tpSockInfoTmp->hSocket);
    GlobalFree(tpSockInfoTmp);
    
    if(WSACloseEvent(wsaEventArray[iEventTemp])==false)
    {
        ErrorHandle("Closing event failed.");
        return 0;
    }

    for(i=iEventTemp;i<iEventNum;i++)
    {
        wsaEventArray[i]=wsaEventArray[i+1];
        tpSocket[i]=tpSocket[i+1];
    }
    
    iEventNum=iEventNum-1;
    
    return 1;
    
}

//SocketClose - zatvara socket

int SocketClose(SOCKET hSockTmp)
{
    if(hSockTmp!=INVALID_SOCKET)
    {
        closesocket(hSockTmp);
        return 1;
    }
    else
    {
        ErrorHandle("Socket cannot be closed.");
        return 0;
    }
}

//ServerThread - pokretanje servera u pozadini

DWORD WINAPI ServerThread(LPVOID lpParameter)
{
    ServerLoop();
}

//ClientThread - pokretanje clienta u pozadini

DWORD WINAPI ClientThread(LPVOID lpParameter)
{
    ClientLoop(sTAdd,atoi(sTPrt.data()),sTUNe);
}

//ClientLoop - petlja za client

int ClientLoop(string sAddress,unsigned short ushPort, string sUserNameTemp)
{

    WSAEVENT wsaEventHandle;
    WSABUF wsaReadBuffer;
    char cReadBuffer[BUFFER_SIZE];
    DWORD iFlags, iRcvTemp, iSntTemp;
    int iError;
    string sMessageTemp;
    bool bAuth=false;
    bRunning=true;
    sClientUserName="";

    hGenSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(hGenSock==INVALID_SOCKET)
    {
        ErrorHandle("Creating listening socket failed.");
        return 0;
    }
    
    wsaEventHandle=WSACreateEvent();
    if(wsaEventHandle==WSA_INVALID_EVENT)
    {
        ErrorHandle("Error creating event.");
        return 0;    
    }

    iError=WSAEventSelect(hGenSock,wsaEventHandle,FD_CONNECT|FD_CLOSE|FD_READ|FD_WRITE);
    if(iError==SOCKET_ERROR)
    {
        ErrorHandle("Error with selecting event.");
        return 0;
    }

    sockaddr_in pSockTemp;
    pSockTemp.sin_family=AF_INET;
    pSockTemp.sin_port=htons(ushPort);

    unsigned long ulIPTemp;
    bool bIP;
    
    bIP=IPCheck(sAddress);
    
    if(bIP==false)
    {
        ulIPTemp=GetHostIP(sAddress);
        if(ulIPTemp==0)
        {
            ErrorHandle("Error resolving host IP.");
            return 0;
        }

        pSockTemp.sin_addr.S_un.S_addr=ulIPTemp;

    }
    else
    {
        pSockTemp.sin_addr.S_un.S_addr=inet_addr(sAddress.data());
    }

    bClient=true;
    bOnline=true;
    bServer=false;
   
    iError=connect(hGenSock,(sockaddr*)(&pSockTemp),sizeof(pSockTemp));
    
    int iSingleEvent;
    
    while(bRunning==true)
    {
        
        iSingleEvent=WaitForSingleObject(wsaEventHandle,60*1000);
        
        if(iSingleEvent==WSA_WAIT_FAILED)
        {
            ErrorHandle("Wait function failed.");
            return 0;
        }

        if(bRunning==false) return 1;
        
        iError=WSAEnumNetworkEvents(hGenSock,wsaEventHandle,&wsaNetEvent);
        if(iError==SOCKET_ERROR)
        {
            ErrorHandle("Error enumerating network events.");
            return 0;            
        }
        
        if(wsaNetEvent.lNetworkEvents & FD_CONNECT)
        {
            sMessageTemp="Connected to ";
            sMessageTemp=sMessageTemp+sAddress;
            sMessageTemp=sMessageTemp+".";
            MessageHandle(sMessageTemp);

            bATH=false;
            sMessageTemp="/ATH ";
            sMessageTemp=sMessageTemp+sUserNameTemp;
            
            sClientUserName="";
            for(int ii=0;ii<sUserNameTemp.size();ii++)
            {
                if(sUserNameTemp.at(ii)==*" ") break;
                else sClientUserName=sClientUserName + sUserNameTemp.at(ii);
            }
//            sClientUserName=sClientUserName+sUserNameTemp;
            SendData(sMessageTemp,hGenSock);
        }
        
        if(wsaNetEvent.lNetworkEvents & FD_READ)
        {

            if(wsaNetEvent.lNetworkEvents & FD_READ && wsaNetEvent.iErrorCode[FD_READ_BIT]!=0)
            {
                ErrorHandle("Read error.");
                return 0;
            }
                        
            iFlags=0;

            ZeroMemory(&cReadBuffer,sizeof(cReadBuffer));

            wsaReadBuffer.buf=cReadBuffer;
            wsaReadBuffer.len=BUFFER_SIZE;

            if(WSARecv(hGenSock,&wsaReadBuffer, 1, &iRcvTemp, &iFlags, NULL, NULL)==SOCKET_ERROR)
            {
                iError=WSAGetLastError();
                if(iError==WSAECONNRESET)
                {
                    bOnline=false;
                    bRunning=false;
                    ErrorHandle("Server connection closed.");
                    return 0;
                }
                if(iError!=WSAEWOULDBLOCK)
                {
                    ErrorHandle("Receiving data failed.");
                    SocketClose(hGenSock);
                    return 0;
                }
            }
            else
            {
                sMessageTemp="";
                sMessageTemp=sMessageTemp + wsaReadBuffer.buf;
                AnalyzeData(sMessageTemp);
            }
        }        
        
        if(wsaNetEvent.lNetworkEvents & FD_CLOSE)
        {
            bOnline=false;
            bRunning=false;
            ErrorHandle("Server connection closed.");
            return 0;            
        }   
    }
}

//DataFormat - stavlja podatke u odredjeni format

string DataFormat(string sData)
{
    if((sData.at(0)==*"*")||(sData.at(0)==*"/")) return sData;

    if(sData.at(0)==*"#")
    {
        ParseString(sData.substr(1,sData.size()));
        return sData;
    }

    string sTemp;

    if(sDefaultFormat=="")
    {
        sTemp="/TXT <";
        sTemp=sTemp + sData;
        sTemp=sTemp + ">";
    }
    else
    {
        sTemp="";
        int i;
        for(i=0;i<sDefaultFormat.size();i++)
        {
            
            if(sDefaultFormat.at(i)==*"$")
            {
                if(i<(sDefaultFormat.size()-1))
                {
                    if(sDefaultFormat.at(i+1)==*"D")
                    {
                        sTemp=sTemp+sData;
                        i++;
                    }
                }
            }
            else
            {
                sTemp=sTemp+sDefaultFormat.at(i);
            }
        }
    }
    
    return sTemp;
}

//SendData - salje string podataka na zadani socket

int SendData(string sData, SOCKET hSockTmp)
{
    int iError;

    if((bOnline==false)||(sData.at(0)==*"#")) return 0;

    sData=sData + "\r\n";

    while(true)
    {
        iError=send(hSockTmp,sData.data(),sData.size(),0);

        if(iError==sData.size())
        {
            sData.erase(0,sData.size());
            return 1;
        }
        else if(iError==SOCKET_ERROR)
        {
            sErrorMessage="Unable to send data.";
            return 0;
        }

    }

}

//IPCheck - provjerava je li zadani string IP broj

bool IPCheck(string sTemp)
{
    int i,dotcount;
    
    dotcount=0;
    
    for(i=0;i<sTemp.size();i++)
    {
        if(sTemp.at(i)==*".")
        {
            dotcount++;
        }
        else
        {
            if (!(sTemp.at(i)==*"0" || sTemp.at(i)==*"1" || sTemp.at(i)==*"2" ||
                  sTemp.at(i)==*"3" || sTemp.at(i)==*"4" || sTemp.at(i)==*"5" ||
                  sTemp.at(i)==*"6" || sTemp.at(i)==*"7" || sTemp.at(i)==*"8" ||
                  sTemp.at(i)==*"9")) return false;
        }
    }
    
    if (dotcount==3)
    {
        return true;
    }
    
}

//AnalyzeData - analizira primljene podatke

int AnalyzeData(string sAnalyze)
{
    //cout << sAnalyze << "\n";
    if(CheckStringEnd(sAnalyze,sAnalyze.size()))
    {
        sAnalyze=sAnalyze.substr(0,sAnalyze.size()-2);
    }
    else
    {
        ErrorHandle("Incorrect data format.");
        return 0;
    }

    int sADParamNum, i, iError;
    short shStart, shMode;
    bool bTextMode=false;
    bool bBrackets;
    string sTemp;
    sTemp="";
    sADParamNum=0;
    vector<string> sADParam(MAX_ANALYZE_PARAMS);

    if(sAnalyze.at(sAnalyze.size()-1)==*"|")
    {
        SendData("/QUE",hGenSock);
        sAnalyze=sAnalyze.substr(0,sAnalyze.size()-1);
    }
    
    if(sAnalyze.at(0)==*"*") // poruka
    {
        shMode=1;
        shStart=1;
    }
    else if(sAnalyze.at(0)==*"/") // naredba
    {
        shMode=2;
        shStart=1;
    }
    else if(sAnalyze.at(0)==*"|")
    {
        bTextMode=true;
        sAnalyze=sAnalyze.substr(1,sAnalyze.size());
        shMode=0;
    }
    else // tekst
    {
        bTextMode=true;
        shMode=0;
//        shStart=0;
    }
    if(bTextMode==false)
    {
        bBrackets=false;
    
        for(i=shStart;i<sAnalyze.size();i++)
        {
            if(sAnalyze.at(i)==*"<")
            {
                bBrackets=true;
                sTemp="";
            }
    
            if(bBrackets==false)
            {
                if((sAnalyze.at(i)==*" ")||(i==(sAnalyze.size()-1)))
                {
                    if(i==(sAnalyze.size()-1)) sTemp=sTemp+sAnalyze.at(i);
                    sADParam[sADParamNum]=sTemp;
                    sADParamNum++;
                    sTemp="";
                }
                else
                {
                    sTemp=sTemp+sAnalyze.at(i);
                }
            }
            else
            {
                if(sAnalyze.at(i)!=*"<" && sAnalyze.at(i)!=*">")
                {
                    sTemp=sTemp+sAnalyze.at(i);
                }
    
                if(sAnalyze.at(i)==*">")
                {
                    bBrackets=false;
                    sADParam[sADParamNum]=sTemp;
                    sADParamNum++;
                    sTemp="";
                    
                    if(i!=(sAnalyze.size()-1))
                    {
                        if(sAnalyze.at(i+1)=*" ")
                        {
                            i++;
                        }
                    }
                }
            }
        }
    }
    switch(shMode)
    {
        case 0:
            sMailData=sMailData + sAnalyze;
            break;
        case 1:
            if(bServer==true)
            {
                if(tpSocket[iEvent-WSA_WAIT_EVENT_0]->bAuthorized==false)
                {
                    SendToSender("*ERR 3");
                    return 0;
                }
                else
                {
                    SendToSender("*ERR 1");
                    return 0;
                }
            }
            if(sADParam[0]=="MFR")
            {
                if(sADParamNum==2)
                {
                    sMailData="";
                    sTemp="From: ";
                    sTemp=sTemp + sADParam[1];
                    MessageHandle(sTemp);
                }
            }
            if(sADParam[0]=="MND")
            {
                if(sADParamNum==2)
                {
                    MessageHandle(sMailData);
                    sMailData="/MRD ";
                    sMailData=sMailData + sADParam[1];
                    SendData(sMailData,hGenSock);
                    sMailData="";

                }
            }
            if(sADParam[0]=="TXT")
            {
                if(sADParamNum==3)
                {
                    sTemp="";
                    sTemp=sTemp + sADParam[1];
                    sTemp=sTemp + ": ";
                    sTemp=sTemp + sADParam[2];
                    MessageHandle(sTemp);
                }
            }
            if(sADParam[0]=="WSP")
            {
                if(sADParamNum==3)
                {
                    sTemp="";
                    sTemp=sTemp + sADParam[1];
                    sTemp=sTemp + " (whispers to you): ";
                    sTemp=sTemp + sADParam[2];
                    MessageHandle(sTemp);
                }
            }
            if(sADParam[0]=="ERR")
            {
                if(sADParamNum==2)
                {
                    iError=atoi(sADParam[1].data());
                    switch(iError)
                    {
                        case 1:
                            sTemp="Notifications can only be sent by server.";
                            break;
                        case 2:
                            sTemp="You are not authorized on this server.";
                            break;
                        case 3:
                            sTemp="You are already authorized on this server.";
                            break;
                        case 4:
                            sTemp="Requested user not found.";
                            break;
                        case 5:
                            sTemp="ID update not needed.";
                            break;
                        case 6:
                            sTemp="Requested username is taken.";
                            break;
                        case 7:
                            sTemp="Internal server error.";
                            break;
                        case 8:
                            sTemp="Requested group name is taken.";
                            break;
                        case 9:
                            sTemp="Maximum group count reached.";
                            break;
                        case 10:
                            sTemp="Requested group not found.";
                            break;
                        case 11:
                            sTemp="No free lobbies found.";
                            break;
                        case 12:
                            sTemp="No free groups found.";
                            break;
                        case 13:
                            sTemp="Maximum user count reached.";
                            break;
                        case 14:
                            sTemp="You are not registered on this server.";
                            break;
                        case 15:
                            sTemp="Wrong password.";
                            break;
                        default:
                            break;
                    }
                    ErrorHandle(sTemp);                    
                }
            }
            if(sADParam[0]=="ATH")
            {
                if(sADParamNum==3)
                {
                    if(sADParam[2]=="OK")
                    {
                        if(sADParam[1]==sClientUserName)
                        {
                            bATH=true;
                            MessageHandle("Authorization complete.");
                        }
                        else
                        {
                            sTemp=sADParam[1];
                            sTemp=sTemp + " logged in.";
                            MessageHandle(sTemp);
                        }
                    }
                    else if(sADParam[2]=="FAIL")
                    {
                        if(sADParam[1]==sClientUserName)
                        {
                            bATH=false;
                            ErrorHandle("Authorization failed.");
                        }
                    }
                }
            }
            if(sADParam[0]=="ANN")
            {
                if(sADParamNum==2) MessageHandle(sADParam[1]);
            }
            if(sADParam[0]=="UPD")
            {
                if(sADParamNum==4)
                {
                    if(sADParam[1]=="ID")
                    {
                        sTemp="User ";
                        sTemp=sTemp + sADParam[2];
                        sTemp=sTemp + " updates his SOCKET ID to ";
                        sTemp=sTemp + sADParam[3];
                        MessageHandle(sTemp);
                    }
                }
            }
            if(sADParam[0]=="CHK")
            {
                if(sADParamNum==3)
                {
                    if(sADParam[1]=="ID")
                    {
                        sTemp="Your SOCKET ID is ";
                        sTemp=sTemp + sADParam[2];
                        MessageHandle(sTemp);
                    }
                }
            }
            break;
        case 2:
            if(sADParam[0]=="ATH")
            {
                if(sADParamNum==2)
                {
                    if(tpSocket[iEvent-WSA_WAIT_EVENT_0]->bAuthorized==false)
                    {
                        sTemp="";
                        sTemp=sTemp + "User ";
                        sTemp=sTemp + sADParam[1];
                        sTemp=sTemp + " authorized.";
                        MessageHandle(sTemp);

                        strcpy(tpSocket[iEvent-WSA_WAIT_EVENT_0]->cUserName,sADParam[1].data());
                        tpSocket[iEvent-WSA_WAIT_EVENT_0]->bAuthorized=true;
                        
                        sTemp="*ATH ";
                        sTemp=sTemp + SenderName();
                        sTemp=sTemp + " OK";
                        SendToAll(sTemp);
                    }
                    else
                    {
                        SendToSender("*ERR 3");
                    }
                }
            }
            if(tpSocket[iEvent-WSA_WAIT_EVENT_0]->bAuthorized==false)
            {
                SendToSender("*ERR 3");
                return 0;
            }
            if(sADParam[0]=="WSP")
            {
                if(sADParamNum==3)
                {
                    sTemp="*WSP ";
                    sTemp=sTemp + SenderName();
                    sTemp=sTemp + " <";
                    sTemp=sTemp + sADParam[2];
                    sTemp=sTemp + ">";
                    
                    if(SendToUser(sTemp,sADParam[1])==0)
                    {
                        SendToSender("*ERR 4");
                    }
                }
            }
            if(sADParam[0]=="END")
            {
                sTemp="*ANN <";
                sTemp=sTemp+SenderName();
                sTemp=sTemp + " logged out.>";
                SendToAll(sTemp);
                sTemp="User ";
                sTemp=sTemp + SenderName();
                sTemp=sTemp + " disconnected.";
                MessageHandle(sTemp);
                FreeSocketInformation(iEvent-WSA_WAIT_EVENT_0);
            }
            if(sADParam[0]=="SSD")
            {
                SendToAll("*ANN <Server shutting down.>");
                MessageHandle("Server shut down.");
                bRunning=false;
                for(i=iEventNum-1;i>=0;i--)
                {
                    FreeSocketInformation(i);
                }
            }
            break;
        default:
            break;
    }
    
    return 1;
}

//SendToAllButSender - salje poruku svima osim posiljatelju

void SendToAllButSender(string sSendString)
{
    int i;
    for(i=1;i<iEventNum;i++)
    {
        if(i!=(iEvent-WSA_WAIT_EVENT_0))
        {
            SendData(sSendString,tpSocket[i]->hSocket);
        }
    }
}

//SendToAll - salje poruku svima

void SendToAll(string sSendString)
{
    int i;
    for(i=1;i<iEventNum;i++)
    {
        SendData(sSendString,tpSocket[i]->hSocket);
    }   
}

//SendToSender - salje poruku posiljatelju

void SendToSender(string sSendString)
{
    SendData(sSendString,tpSocket[iEvent-WSA_WAIT_EVENT_0]->hSocket);
}

//SendToUser - salje podatke odredjenom korisniku

int SendToUser(string sSendString,string sUserNameSend)
{
    int iTemp;
    iTemp=FindUserName(sUserNameSend);
    if(iTemp!=0)
    {
        SendData(sSendString,tpSocket[iTemp]->hSocket);
        return 1;
    }
    else
    {
        ErrorHandle("Requested username not found.");
        return 0;
    }
}

//SenderName - vraca ime posiljatelja

string SenderName()
{
    string sNTemp;
    sNTemp="";
    sNTemp=sNTemp + tpSocket[iEvent-WSA_WAIT_EVENT_0]->cUserName;
    return sNTemp;
}

//FindUserName - trazi socket po imenu korisnika

int FindUserName(string sFindUser)
{
    int i;
    string sTempFind;
    for(i=1;i<iEventNum;i++)
    {
        sTempFind="";
        sTempFind=sTempFind + tpSocket[i]->cUserName;
        if(sTempFind==sFindUser) return i;
    }
    return 0;
}

int FindStringLength(string sTemp)
{
    int i;
    for(i=0;i<sTemp.size()-1;i++)
    {
        if(sTemp.at(i)==*"\r" && sTemp.at(i+1)==*"\n")
        {
            return i;
        }
    }
    return sTemp.size();
}

bool CheckStringEnd(string sTemp, int iSizeTemp)
{
    if(sTemp.at(iSizeTemp-2)==*"\r" && sTemp.at(iSizeTemp-1)==*"\n") return true;
    
    return false;
}

//AddMail - dodaje mail poruku

int AddMail(string sUserName, string sFrom, int iMailSizeTemp, string sMailData)
{    
//    if((tpRegArray[iUserSpot]->iInboxSize)<MAX_MESSAGE_COUNT)
//    {
//        tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]=(tpMailInformation)GlobalAlloc(GPTR,sizeof(tMailInformation));
//        if(tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]==NULL)
//        {
//            ErrorHandle("Error allocating mail data.");
//            return 0;
//        }
//        strcpy(tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]->cFrom,sFrom.data());
//        strcpy(tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]->cMailData,sMailData.data());
//        tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]->iMailSize=iMailSizeTemp;
//        tpRegArray[iUserSpot]->iInboxSize=(tpRegArray[iUserSpot]->iInboxSize)+1;
//    }
//    else
//    {
//        return (-2); //inbox pun
//    }
//    
//    return 1;
}

//DeleteMail - brise odredjenu mail poruku

int DeleteMail(string sUserName, int iMailSpot)
{
//    int iUserSpot;
//    
//    iUserSpot=FindRegUser(sUserName);
//    
//    if(iUserSpot==(-1)) return (-1);
//
//    tpMailInformation tpMailDelete=tpRegArray[iUserSpot]->tpInbox[iMailSpot];
//    
//    for(int j=iMailSpot;j<((tpRegArray[iUserSpot]->iInboxSize)-1);j++)
//    {
//        tpRegArray[iUserSpot]->tpInbox[j]=tpRegArray[iUserSpot]->tpInbox[j+1];
//    }
//    tpRegArray[iUserSpot]->iInboxSize=(tpRegArray[iUserSpot]->iInboxSize)-1;
//    ZeroMemory(&(tpRegArray[iUserSpot]->tpInbox[tpRegArray[iUserSpot]->iInboxSize]),sizeof(tMailInformation));
//    GlobalFree(tpMailDelete);
//    return 1;
}
