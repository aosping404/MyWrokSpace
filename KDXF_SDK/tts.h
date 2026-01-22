#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <errno.h>
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"
using namespace std;
#pragma comment(lib,"winmm.lib")
#ifdef _WIN64
#pragma comment(lib,"./libs/msc_x64.lib")//x64
#else
#pragma comment(lib,"./libs/msc.lib")//x86
#endif
class tts
{
public: 
	bool ttsplay(const char* text,const char* wavfile = "≤‚ ‘.wav");
};

