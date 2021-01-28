#include "Sample.h"

bool Sample::Init()
{
	m_Device.Init();
	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::PreRender()
{
	m_Device.PreRender();
	return true;
}
bool Sample::Render()
{
	m_Device.Render();
	return true;
}
bool Sample::PostRender()
{
	m_Device.PostRender();
	return true;
}
bool Sample::Release()
{
	return true;
}