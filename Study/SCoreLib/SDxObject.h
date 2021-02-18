#pragma once
#include "SStd.h"
#include "dxtk/WICTextureLoader.h" //../../include/DirectXTK
#include "SimpleMath.h"
#include "SDxState.h"
#include "STextureManager.h"
using namespace DirectX;
using namespace SimpleMath;

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct PNCT_VERTEX
{

};
class SDxObject
{
};

