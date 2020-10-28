#pragma once
#include "SStd.h"
#define SGAME_START int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){Sample demo;
#define SGAME_RUN	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
#define GAMERUN SGAME_START;SGAME_RUN;