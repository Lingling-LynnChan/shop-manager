#ifndef _SRC_H_
#define _SRC_H_

#define GBK2GBK
#include<conio.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<winsock2.h>
#include<windows.h>
#define f() fflush(stdin)
#ifndef GBK2GBK
void GBK2UTF8(char* utf8,char* gbk){
	#ifdef _WIN32
	int n=MultiByteToWideChar(CP_ACP,0,gbk,-1,NULL,0);
	wchar_t* wstr=(wchar_t*)malloc((n+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,gbk,-1,wstr,n);
	n=WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_UTF8,0,wstr,-1,utf8,n,NULL,NULL);
	free(wstr);
	#else
	strcpy(utf8,gbk);
	#endif
}
void UTF82GBK(char* gbk,char* utf8){
	#ifdef _WIN32
	int n=MultiByteToWideChar(CP_UTF8,0,utf8,-1,NULL,0);
	wchar_t* wstr=(wchar_t*)malloc((n+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8,0,utf8,-1,wstr,n);
	n=WideCharToMultiByte(CP_ACP,0,wstr,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,wstr,-1,gbk,n,NULL,NULL);
	free(wstr);
	#else
	strcpy(gbk,utf8);
	#endif
}
#else
void GBK2UTF8(char* utf8,char* gbk){
	strcpy(utf8,gbk);
}
void UTF82GBK(char* gbk,char* utf8){
}
#endif
void thread_sleep(long l){
	double time_s=(double)l/1000.0;
	clock_t startTime=clock();
	clock_t nowTime;
	while(nowTime=clock(),(double)(nowTime-startTime)<time_s*CLOCKS_PER_SEC);
}
void HideWindow() {
	HWND hwnd = GetForegroundWindow();
	if (hwnd) {
		// ShowWindow(hwnd, SW_HIDE);//隐藏窗口
	}
}
#define uint unsigned long long
long long getId(){//用雪花算法获取id
	static uint code=0;
    uint tmp=0;
	static uint snowtime=0;
    time_t newtime=time(NULL);
	uint nowtime=time(&newtime);
    if(nowtime!=snowtime){
        code=0;
    }
    snowtime=nowtime;
    for(int i=0;i<41;i++){
        tmp|=(nowtime&(1<<i))<<22;
    }
    for(int i=0;i<12;i++){
        tmp|=code&(1<<i);
    }
    code++;
    long long output=*(long long*)&tmp;
	return output;
}
#undef uint

#endif
