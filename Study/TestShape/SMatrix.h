#pragma once

#include "SVector.h"

struct float4x4
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

class xMatrix : public float4x4
{
public:
	static float g_identity[16];

public:
	xMatrix()
	{
		Identity();
	};
	void Identity()
	{
		_12 = _13 = _14 = 0.0f;
		_21 = _23 = _24 = 0.0f;
		_31 = _32 = _34 = 0.0f;
		_41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}
	xMatrix(const xMatrix& matrix)
	{
		_11 = matrix._11; _12 = matrix._12; _13 = matrix._13; _14 = matrix._14;
		_21 = matrix._21; _22 = matrix._22; _23 = matrix._23; _24 = matrix._24;
		_31 = matrix._31; _32 = matrix._32; _33 = matrix._33; _34 = matrix._34;
		_41 = matrix._41; _42 = matrix._42; _43 = matrix._43; _44 - matrix._44;
	}

	xMatrix operator * (xMatrix const &Matrix)
	{
		xMatrix OutMatrix;
		for (int iColumn = 0; iColumn < 4; iColumn++)
		{
			for (int iRow = 0; iRow < 4; iRow++)
			{
				OutMatrix.m[iRow][iColumn] = m[iRow][0] * Matrix.m[0][iColumn] +
					m[iRow][1] * Matrix.m[1][iColumn] +
					m[iRow][2] * Matrix.m[2][iColumn] +
					m[iRow][3] * Matrix.m[3][iColumn];
			}
		}
		return OutMatrix;
	}
	Vector3 operator * (Vector3 const &vector)
	{
		return Vector3(
			vector.x * 11 + vector.y * 21 + vector.z * 31 + _41,
			vector.x * 12 + vector.y * 22 + vector.z * 32 + _42,
			vector.x * 13 + vector.y * 23 + vector.z * 33 + _43);
	}
	// access grants
	float& operator () (UINT iRow, UINT iCol)
	{
		return m[iRow][iCol];
	}
	float operator () (UINT iRow, UINT iCol) const
	{
		return m[iRow][iCol];
	}
	// casting operators
	operator float* ()
	{
		return (float *)&_11;
	}
	operator const float* () const
	{
		return (const float *)&_11;
	}
	void Translation(const Vector3 &vector)
	{
		_41 = vector.x;
		_42 = vector.y;
		_43 = vector.z;
	}
	void Translation(float x, float y, float z)
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}
	void Scale(const Vector3 &vector)
	{
		_11 = vector.x;
		_22 = vector.y;
		_33 = vector.z;
	}
	void Scale(float x, float y, float z)
	{
		_11 = x;
		_22 = y;
		_33 = z;
	}
	void XRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_22 = fCosTheta;
		_23 = fSinTheta;
		_32 = -fSinTheta;
		_33 = fCosTheta;
	}
	void YRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_13 = -fSinTheta;
		_31 = fSinTheta;
		_33 = fCosTheta;
	}
	void ZRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_12 = -fSinTheta;
		_21 = fSinTheta;
		_22 = fCosTheta;
	}
	// 전치 행렬 계산
	xMatrix Transpose()
	{
		xMatrix matrix;
		matrix._11 = _11; matrix._12 = _21; matrix._13 = _31; matrix._14 = _41;
		matrix._21 = _12; matrix._22 = _22; matrix._23 = _32; matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;
		return matrix;
	}

};