#pragma once
#include "SStd.h"
struct SMsg
{
	RECT rt;
	wstring msg;
};
class SWrite : public SSingleton<SWrite>
{
private:
	HFONT	m_hFont;
	std::vector<SMsg> m_TextList;
public:
	void Push(SMsg msg);
	void Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	void Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Relase();
public:
	SWrite();
	virtual ~SWrite();
};
#define g_Write SWrite::GetInstance()

