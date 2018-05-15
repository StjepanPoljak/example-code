#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

int Add(int,int);
void ErrorHandle(string);
void MessageHandle(string);
DWORD WINAPI ServerThread(LPVOID lpParameter);
DWORD WINAPI ClientThread(LPVOID lpParameter);
DWORD dwThread;
void ParseString(string);

