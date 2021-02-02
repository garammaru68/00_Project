#pragma once
#include "SStd.h"
#define SGAME_START int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){Sample demo;
#define SGAME_LOOP	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
#define SGAME_RUN SGAME_START;SGAME_LOOP;

#define SCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define SCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define SCORE_RUN(s,x,y) SCORE_START;SCORE_WIN(s,x,y);
#define SCORE_END }