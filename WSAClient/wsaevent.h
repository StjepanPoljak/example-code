#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

const int BUFFER_SIZE=8192;
const int NEEDED_VERSION_LOBYTE=2;
const int NEEDED_VERSION_HIBYTE=0;
const int MAX_ANALYZE_PARAMS=10;
const int MAX_NAME_LENGTH=12;

string sErrorMessage;
sockaddr_in pInternetAddress;
SOCKET hGenSock;
WSADATA pWinsockData;

typedef struct _SocketInformation
{
    char cReadBuffer[BUFFER_SIZE];
//    char cWriteBuffer[BUFFER_SIZE];
    WSABUF wsaReadBuffer;
//    WSABUF wsaWriteBuffer;
    SOCKET hSocket;
    int iReceived;
    int iSent;
    in_addr *addrIPAddress;
    unsigned short ushPortNumber;
    bool bAuthorized;
    char cUserName[MAX_NAME_LENGTH];
} tSocketInformation, *tpSocketInformation;

typedef struct _MailInformation
{
    char cFrom[50];
    char cMailData[BUFFER_SIZE];
    int iMailSize;
} tMailInformation, *tpMailInformation;

tpMailInformation *tpMailArray=NULL;
int iMailNum, iMailArraySize;

int iEventNum=0;
WSAEVENT wsaEventArray[WSA_MAXIMUM_WAIT_EVENTS];
tpSocketInformation tpSocket[WSA_MAXIMUM_WAIT_EVENTS];

int iEvent;
WSANETWORKEVENTS wsaNetEvent;
string sClientUserName;
bool bRunning, bOnline;
bool bClient, bServer, bATH;
string sMailData;

string sTAdd,sTPrt,sTUNe;
string sDefaultFormat;

int Winsock2Init();
int Winsock2End();
unsigned long GetHostIP(string);
int CreateSocketInformation(SOCKET,in_addr*,unsigned short);
int CreateSocketInformation(SOCKET,unsigned short);
int CreateListenSocket(unsigned short);
int ServerLoop();
int FreeSocketInformation(int);
int SocketClose(SOCKET);
int SendData(string, SOCKET);
int ClientLoop(string,unsigned short,string);
bool IPCheck(string);
int AnalyzeData(string);
void SendToAllButSender(string);
void SendToAll(string);
void SendToSender(string);
string SenderName();
int FindUserName(string);
int SendToUser(string,string);
string DataFormat(string);
int FindStringLength(string);
bool CheckStringEnd(string,int);
int AddMail(string,string,int,string);
int DeleteMail(string,string);
