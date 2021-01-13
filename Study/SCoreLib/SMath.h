#pragma once
#ifndef SBASIS2D_MATH
#define SBASIS2D_MATH
#include <math.h>
#include <time.h>

#define SBASIS_EPSILON	((float) 0.001f)
#define SBASIS_PI		((float) 3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (SBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / SBASIS_PI))
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

namespace SBASIS_CORE_LIB
{
	static const float S_EPSILON = 0.001f;
	static const float S_PI = 3.141592f;
	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct SPoint : float2
	{
		static SPoint zero;
		SPoint() = default;
		SPoint(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		float* get()
		{
			return v;
		}

		void operator = (float* arr)
		{
			x = arr[0];
			y = arr[1];
		}
		float& operator [] (int index)
		{
			if (index == 0) return x;
			return y;
		}
		bool operator == (SPoint p)
		{
			if (fabs(x - p.x) < S_EPSILON)
			{
				if (fabs(y - p.y) < S_EPSILON)
				{
					return true;
				}
			}
			return false;
		}
		bool operator != (SPoint p)
		{
			if (fabs(x - p.x) < S_EPSILON)
			{
				if (fabs(y - p.y) < S_EPSILON)
				{
					return false;
				}
			}
			return true;
		}
		SPoint operator * (float fValue)
		{
			SPoint ret;
			ret.x = x * fValue;
			ret.y = y * fValue;
			return ret;
		}
		SPoint operator + (SPoint p)
		{
			SPoint ret;
			ret.x = x + p.x;
			ret.y = y + p.y;
			return ret;
		}
		SPoint operator - (SPoint p)
		{
			SPoint ret;
			ret.x = x - p.x;
			ret.y = y - p.y;
			return ret;
		}

		float Length()
		{
			return sqrt(x * x + y * y);
		}

		SPoint Normalize()
		{
			float fD = Length();
			if (fD != 0.0f)
			{
				x /= fD;
				y /= fD;
			}
			return SPoint(x, y);
		}

		float Angle(SPoint& v0)
		{
			SPoint vA = Normalize();
			SPoint vB = v0.Normalize();
			float fDot = (vA | vB);
			if (fDot > 1.0f) fDot = 1.0f;
			float fRadian = acos(fDot);
			float fAngle = (float)RadianToDegree(fRadian);
			if (fDot < 0.0f)
			{
				fAngle = 180.0f + (180.0f - fAngle);
			}
			else
			{
				fAngle = fAngle;
			}
			return fAngle;
		}
		// Dot Product
		float operator | (SPoint const &v0)
		{
			return x * v0.x + y * v0.y;
		}
		// Cross Product
		SPoint operator ^ (SPoint const &v0)
		{
			if (*this == v0)
			{
				return v0;
			}
			return SPoint(0, (x * v0.y - y * v0.x));
		}

		bool CCW(SPoint vLook, SPoint vDir)
		{
			//SPoint pTarget, pPos;
			//// Y값은 원점에서 아래로 감소하도록 클라이언트 좌표를 직각 좌표로 변환한다.
			//pTarget.y *= -1.0f;
			//pPos.y *= -1.0f;
			//// 이후 이전 프레임의 벡터와 현재의 벡터의 각을 계산한다.
			//VectoSPointr2 vDir;
			//vDir.x = pTarget.x - pPos.x;
			//vDir.y = pTarget.y - pPos.y;
			//vDir = vDir.Normal();
			//float fAngle = vDir.Angle(vLook);

			// 시계 반대방향으로 움직이면 각도를 뺀다.
			SPoint vA, vB, vC;
			vA.x = vLook.x; vA.y = vLook.y;
			vB.x = vDir.x;	vB.y = vDir.y;
			vC = (vA ^ vB).Normalize();
			float fDot = vC | SPoint(0, -1);
			if (fDot > 0.0f)
			{
				return false;
			}
			return true;
		}
	};
}
namespace S_CORE = SBASIS_CORE_LIB;
using namespace S_CORE;
#endif