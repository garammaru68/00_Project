#pragma once
#include <d3d11_1.h> 
#include <d3d11.h> 
#if !defined(__d3d11_h__) && !defined(__d3d11_x_h__) && !defined(__d3d12_h__) && !defined(__d3d12_x_h__)
#error include d3d11.h or d3d12.h before including SMMath.h
#endif

#if !defined(_XBOX_ONE) || !defined(_TITLE)
 #include <dxgi1_2.h>
#endif

#include <functional>
#include <assert.h>
#include <memory.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#ifndef XM_CONSTEXPR
#define XM_CONSTEXPR
#endif
using namespace DirectX;
namespace SBASIS_EX
{
	struct SVector2;
	struct SVector3;
	struct SVector4;
	struct SDxMatrix;
	struct SQuaternion;
	struct SPlane;

	//------------------------------------------------------------------------------
	// 2D rectangle
	struct SRectangle
	{
		long x;
		long y;
		long width;
		long height;

		// Creators
		SRectangle() noexcept : x(0), y(0), width(0), height(0) {}
		XM_CONSTEXPR SRectangle(long ix, long iy, long iw, long ih) : x(ix), y(iy), width(iw), height(ih) {}
		explicit SRectangle(const RECT& rct) : x(rct.left), y(rct.top), width(rct.right - rct.left), height(rct.bottom - rct.top) {}

		SRectangle(const SRectangle&) = default;
		SRectangle& operator=(const SRectangle&) = default;

		SRectangle(SRectangle&&) = default;
		SRectangle& operator=(SRectangle&&) = default;

		operator RECT() { RECT rct; rct.left = x; rct.top = y; rct.right = (x + width); rct.bottom = (y + height); return rct; }
#ifdef __cplusplus_winrt
		operator Windows::Foundation::Rect() { return Windows::Foundation::Rect(float(x), float(y), float(width), float(height)); }
#endif

		// Comparison operators
		bool operator == (const SRectangle& r) const { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
		bool operator == (const RECT& rct) const { return (x == rct.left) && (y == rct.top) && (width == (rct.right - rct.left)) && (height == (rct.bottom - rct.top)); }

		bool operator != (const SRectangle& r) const { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }
		bool operator != (const RECT& rct) const { return (x != rct.left) || (y != rct.top) || (width != (rct.right - rct.left)) || (height != (rct.bottom - rct.top)); }

		// Assignment operators
		SRectangle& operator=(_In_ const RECT& rct) { x = rct.left; y = rct.top; width = (rct.right - rct.left); height = (rct.bottom - rct.top); return *this; }

		// SRectangle operations
		SVector2 Location() const;
		SVector2 Center() const;

		bool IsEmpty() const { return (width == 0 && height == 0 && x == 0 && y == 0); }

		bool Contains(long ix, long iy) const { return (x <= ix) && (ix < (x + width)) && (y <= iy) && (iy < (y + height)); }
		bool Contains(const SVector2& point) const;
		bool Contains(const SRectangle& r) const { return (x <= r.x) && ((r.x + r.width) <= (x + width)) && (y <= r.y) && ((r.y + r.height) <= (y + height)); }
		bool Contains(const RECT& rct) const { return (x <= rct.left) && (rct.right <= (x + width)) && (y <= rct.top) && (rct.bottom <= (y + height)); }

		void Inflate(long horizAmount, long vertAmount);

		bool Intersects(const SRectangle& r) const { return (r.x < (x + width)) && (x < (r.x + r.width)) && (r.y < (y + height)) && (y < (r.y + r.height)); }
		bool Intersects(const RECT& rct) const { return (rct.left < (x + width)) && (x < rct.right) && (rct.top < (y + height)) && (y < rct.bottom); }

		void Offset(long ox, long oy) { x += ox; y += oy; }

		// Static functions
		static SRectangle Intersect(const SRectangle& ra, const SRectangle& rb);
		static RECT Intersect(const RECT& rcta, const RECT& rctb);

		static SRectangle Union(const SRectangle& ra, const SRectangle& rb);
		static RECT Union(const RECT& rcta, const RECT& rctb);
	};

	//------------------------------------------------------------------------------
	// 2D vector
	struct SVector2 : DirectX::XMFLOAT2
	{
		SVector2() noexcept : XMFLOAT2(0.f, 0.f) {}
		XM_CONSTEXPR explicit SVector2(float x) : XMFLOAT2(x, x) {}
		XM_CONSTEXPR SVector2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit SVector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
		SVector2(XMFLOAT2 V) { x = V.x; y = V.y; }
		SVector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }
		explicit SVector2(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; }

		SVector2(const SVector2&) = default;
		SVector2& operator=(const SVector2&) = default;

		SVector2(SVector2&&) = default;
		SVector2& operator=(SVector2&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat2(this); }

		// Comparison operators
		bool operator == (const SVector2& V) const;
		bool operator != (const SVector2& V) const;

		// Assignment operators
		SVector2& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; return *this; }
		SVector2& operator+= (const SVector2& V);
		SVector2& operator-= (const SVector2& V);
		SVector2& operator*= (const SVector2& V);
		SVector2& operator*= (float S);
		SVector2& operator/= (float S);

		// Unary operators
		SVector2 operator+ () const { return *this; }
		SVector2 operator- () const { return SVector2(-x, -y); }

		// Vector operations
		bool InBounds(const SVector2& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const SVector2& V) const;
		void Cross(const SVector2& V, SVector2& result) const;
		SVector2 Cross(const SVector2& V) const;

		void Normalize();
		void Normalize(SVector2& result) const;

		void Clamp(const SVector2& vmin, const SVector2& vmax);
		void Clamp(const SVector2& vmin, const SVector2& vmax, SVector2& result) const;

		// Static functions
		static float Distance(const SVector2& v1, const SVector2& v2);
		static float DistanceSquared(const SVector2& v1, const SVector2& v2);

		static void Min(const SVector2& v1, const SVector2& v2, SVector2& result);
		static SVector2 Min(const SVector2& v1, const SVector2& v2);

		static void Max(const SVector2& v1, const SVector2& v2, SVector2& result);
		static SVector2 Max(const SVector2& v1, const SVector2& v2);

		static void Lerp(const SVector2& v1, const SVector2& v2, float t, SVector2& result);
		static SVector2 Lerp(const SVector2& v1, const SVector2& v2, float t);

		static void SmoothStep(const SVector2& v1, const SVector2& v2, float t, SVector2& result);
		static SVector2 SmoothStep(const SVector2& v1, const SVector2& v2, float t);

		static void Barycentric(const SVector2& v1, const SVector2& v2, const SVector2& v3, float f, float g, SVector2& result);
		static SVector2 Barycentric(const SVector2& v1, const SVector2& v2, const SVector2& v3, float f, float g);

		static void CatmullRom(const SVector2& v1, const SVector2& v2, const SVector2& v3, const SVector2& v4, float t, SVector2& result);
		static SVector2 CatmullRom(const SVector2& v1, const SVector2& v2, const SVector2& v3, const SVector2& v4, float t);

		static void Hermite(const SVector2& v1, const SVector2& t1, const SVector2& v2, const SVector2& t2, float t, SVector2& result);
		static SVector2 Hermite(const SVector2& v1, const SVector2& t1, const SVector2& v2, const SVector2& t2, float t);

		static void Reflect(const SVector2& ivec, const SVector2& nvec, SVector2& result);
		static SVector2 Reflect(const SVector2& ivec, const SVector2& nvec);

		static void Refract(const SVector2& ivec, const SVector2& nvec, float refractionIndex, SVector2& result);
		static SVector2 Refract(const SVector2& ivec, const SVector2& nvec, float refractionIndex);

		static void Transform(const SVector2& v, const SQuaternion& quat, SVector2& result);
		static SVector2 Transform(const SVector2& v, const SQuaternion& quat);

		static void Transform(const SVector2& v, const SDxMatrix& m, SVector2& result);
		static SVector2 Transform(const SVector2& v, const SDxMatrix& m);
		static void Transform(_In_reads_(count) const SVector2* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector2* resultArray);

		static void Transform(const SVector2& v, const SDxMatrix& m, SVector4& result);
		static void Transform(_In_reads_(count) const SVector2* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector4* resultArray);

		static void TransformNormal(const SVector2& v, const SDxMatrix& m, SVector2& result);
		static SVector2 TransformNormal(const SVector2& v, const SDxMatrix& m);
		static void TransformNormal(_In_reads_(count) const SVector2* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector2* resultArray);

		// Constants
		static const SVector2 Zero;
		static const SVector2 One;
		static const SVector2 UnitX;
		static const SVector2 UnitY;
	};

	// Binary operators
	SVector2 operator+ (const SVector2& V1, const SVector2& V2);
	SVector2 operator- (const SVector2& V1, const SVector2& V2);
	SVector2 operator* (const SVector2& V1, const SVector2& V2);
	SVector2 operator* (const SVector2& V, float S);
	SVector2 operator/ (const SVector2& V1, const SVector2& V2);
	SVector2 operator* (float S, const SVector2& V);

	//------------------------------------------------------------------------------
	// 3D vector
	struct SVector3 : DirectX::XMFLOAT3
	{
		SVector3() noexcept : DirectX::XMFLOAT3(0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit SVector3(float x) : DirectX::XMFLOAT3(x, x, x) {}
		XM_CONSTEXPR SVector3(float _x, float _y, float _z) : DirectX::XMFLOAT3(_x, _y, _z) {}
		SVector3(const float *pArray) : DirectX::XMFLOAT3(pArray) {}
		SVector3(DirectX::XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		SVector3(const DirectX::XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }
		explicit SVector3(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

		SVector3(const SVector3&) = default;
		SVector3& operator=(const SVector3&) = default;

		SVector3(SVector3&&) = default;
		SVector3& operator=(SVector3&&) = default;

		operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }

		float operator [](int i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return 0.0f;
		}

		// Comparison operators
		bool operator == (const SVector3& V) const;
		bool operator != (const SVector3& V) const;

		// Assignment operators
		SVector3& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
		SVector3& operator+= (const SVector3& V);
		SVector3& operator-= (const SVector3& V);
		SVector3& operator*= (const SVector3& V);
		SVector3& operator*= (float S);
		SVector3& operator/= (float S);

		// Unary operators
		SVector3 operator+ () const { return *this; }
		SVector3 operator- () const;

		// Vector operations
		bool InBounds(const SVector3& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const SVector3& V) const;
		void Cross(const SVector3& V, SVector3& result) const;
		SVector3 Cross(const SVector3& V) const;

		void Normalize();
		void Normalize(SVector3& result) const;

		void Clamp(const SVector3& vmin, const SVector3& vmax);
		void Clamp(const SVector3& vmin, const SVector3& vmax, SVector3& result) const;

		// Static functions
		static float Distance(const SVector3& v1, const SVector3& v2);
		static float DistanceSquared(const SVector3& v1, const SVector3& v2);

		static void Min(const SVector3& v1, const SVector3& v2, SVector3& result);
		static SVector3 Min(const SVector3& v1, const SVector3& v2);

		static void Max(const SVector3& v1, const SVector3& v2, SVector3& result);
		static SVector3 Max(const SVector3& v1, const SVector3& v2);

		static void Lerp(const SVector3& v1, const SVector3& v2, float t, SVector3& result);
		static SVector3 Lerp(const SVector3& v1, const SVector3& v2, float t);

		static void SmoothStep(const SVector3& v1, const SVector3& v2, float t, SVector3& result);
		static SVector3 SmoothStep(const SVector3& v1, const SVector3& v2, float t);

		static void Barycentric(const SVector3& v1, const SVector3& v2, const SVector3& v3, float f, float g, SVector3& result);
		static SVector3 Barycentric(const SVector3& v1, const SVector3& v2, const SVector3& v3, float f, float g);

		static void CatmullRom(const SVector3& v1, const SVector3& v2, const SVector3& v3, const SVector3& v4, float t, SVector3& result);
		static SVector3 CatmullRom(const SVector3& v1, const SVector3& v2, const SVector3& v3, const SVector3& v4, float t);

		static void Hermite(const SVector3& v1, const SVector3& t1, const SVector3& v2, const SVector3& t2, float t, SVector3& result);
		static SVector3 Hermite(const SVector3& v1, const SVector3& t1, const SVector3& v2, const SVector3& t2, float t);

		static void Reflect(const SVector3& ivec, const SVector3& nvec, SVector3& result);
		static SVector3 Reflect(const SVector3& ivec, const SVector3& nvec);

		static void Refract(const SVector3& ivec, const SVector3& nvec, float refractionIndex, SVector3& result);
		static SVector3 Refract(const SVector3& ivec, const SVector3& nvec, float refractionIndex);

		static void Transform(const SVector3& v, const SQuaternion& quat, SVector3& result);
		static SVector3 Transform(const SVector3& v, const SQuaternion& quat);

		static void Transform(const SVector3& v, const SDxMatrix& m, SVector3& result);
		static SVector3 Transform(const SVector3& v, const SDxMatrix& m);
		static void Transform(_In_reads_(count) const SVector3* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector3* resultArray);

		static void Transform(const SVector3& v, const SDxMatrix& m, SVector4& result);
		static void Transform(_In_reads_(count) const SVector3* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector4* resultArray);

		static void TransformNormal(const SVector3& v, const SDxMatrix& m, SVector3& result);
		static SVector3 TransformNormal(const SVector3& v, const SDxMatrix& m);
		static void TransformNormal(_In_reads_(count) const SVector3* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector3* resultArray);

		// Constants
		static const SVector3 Zero;
		static const SVector3 One;
		static const SVector3 UnitX;
		static const SVector3 UnitY;
		static const SVector3 UnitZ;
		static const SVector3 Up;
		static const SVector3 Down;
		static const SVector3 Right;
		static const SVector3 Left;
		static const SVector3 Forward;
		static const SVector3 Backward;
	};

	// Binary operators
	SVector3 operator+ (const SVector3& V1, const SVector3& V2);
	SVector3 operator- (const SVector3& V1, const SVector3& V2);
	SVector3 operator* (const SVector3& V1, const SVector3& V2);
	SVector3 operator* (const SVector3& V, float S);
	SVector3 operator/ (const SVector3& V1, const SVector3& V2);
	SVector3 operator* (float S, const SVector3& V);

	//------------------------------------------------------------------------------
	// 4D vector
	struct SVector4 : public XMFLOAT4
	{
		SVector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		XM_CONSTEXPR explicit SVector4(float x) : XMFLOAT4(x, x, x, x) {}
		XM_CONSTEXPR SVector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit SVector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		SVector4(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		SVector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
		explicit SVector4(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		SVector4(const SVector4&) = default;
		SVector4& operator=(const SVector4&) = default;

		SVector4(SVector4&&) = default;
		SVector4& operator=(SVector4&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const SVector4& V) const;
		bool operator != (const SVector4& V) const;

		// Assignment operators
		SVector4& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		SVector4& operator+= (const SVector4& V);
		SVector4& operator-= (const SVector4& V);
		SVector4& operator*= (const SVector4& V);
		SVector4& operator*= (float S);
		SVector4& operator/= (float S);

		// Unary operators
		SVector4 operator+ () const { return *this; }
		SVector4 operator- () const;

		// Vector operations
		bool InBounds(const SVector4& Bounds) const;

		float Length() const;
		float LengthSquared() const;

		float Dot(const SVector4& V) const;
		void Cross(const SVector4& v1, const SVector4& v2, SVector4& result) const;
		SVector4 Cross(const SVector4& v1, const SVector4& v2) const;

		void Normalize();
		void Normalize(SVector4& result) const;

		void Clamp(const SVector4& vmin, const SVector4& vmax);
		void Clamp(const SVector4& vmin, const SVector4& vmax, SVector4& result) const;

		// Static functions
		static float Distance(const SVector4& v1, const SVector4& v2);
		static float DistanceSquared(const SVector4& v1, const SVector4& v2);

		static void Min(const SVector4& v1, const SVector4& v2, SVector4& result);
		static SVector4 Min(const SVector4& v1, const SVector4& v2);

		static void Max(const SVector4& v1, const SVector4& v2, SVector4& result);
		static SVector4 Max(const SVector4& v1, const SVector4& v2);

		static void Lerp(const SVector4& v1, const SVector4& v2, float t, SVector4& result);
		static SVector4 Lerp(const SVector4& v1, const SVector4& v2, float t);

		static void SmoothStep(const SVector4& v1, const SVector4& v2, float t, SVector4& result);
		static SVector4 SmoothStep(const SVector4& v1, const SVector4& v2, float t);

		static void Barycentric(const SVector4& v1, const SVector4& v2, const SVector4& v3, float f, float g, SVector4& result);
		static SVector4 Barycentric(const SVector4& v1, const SVector4& v2, const SVector4& v3, float f, float g);

		static void CatmullRom(const SVector4& v1, const SVector4& v2, const SVector4& v3, const SVector4& v4, float t, SVector4& result);
		static SVector4 CatmullRom(const SVector4& v1, const SVector4& v2, const SVector4& v3, const SVector4& v4, float t);

		static void Hermite(const SVector4& v1, const SVector4& t1, const SVector4& v2, const SVector4& t2, float t, SVector4& result);
		static SVector4 Hermite(const SVector4& v1, const SVector4& t1, const SVector4& v2, const SVector4& t2, float t);

		static void Reflect(const SVector4& ivec, const SVector4& nvec, SVector4& result);
		static SVector4 Reflect(const SVector4& ivec, const SVector4& nvec);

		static void Refract(const SVector4& ivec, const SVector4& nvec, float refractionIndex, SVector4& result);
		static SVector4 Refract(const SVector4& ivec, const SVector4& nvec, float refractionIndex);

		static void Transform(const SVector2& v, const SQuaternion& quat, SVector4& result);
		static SVector4 Transform(const SVector2& v, const SQuaternion& quat);

		static void Transform(const SVector3& v, const SQuaternion& quat, SVector4& result);
		static SVector4 Transform(const SVector3& v, const SQuaternion& quat);

		static void Transform(const SVector4& v, const SQuaternion& quat, SVector4& result);
		static SVector4 Transform(const SVector4& v, const SQuaternion& quat);

		static void Transform(const SVector4& v, const SDxMatrix& m, SVector4& result);
		static SVector4 Transform(const SVector4& v, const SDxMatrix& m);
		static void Transform(_In_reads_(count) const SVector4* varray, size_t count, const SDxMatrix& m, _Out_writes_(count) SVector4* resultArray);

		// Constants
		static const SVector4 Zero;
		static const SVector4 One;
		static const SVector4 UnitX;
		static const SVector4 UnitY;
		static const SVector4 UnitZ;
		static const SVector4 UnitW;
	};

	// Binary operators
	SVector4 operator+ (const SVector4& V1, const SVector4& V2);
	SVector4 operator- (const SVector4& V1, const SVector4& V2);
	SVector4 operator* (const SVector4& V1, const SVector4& V2);
	SVector4 operator* (const SVector4& V, float S);
	SVector4 operator/ (const SVector4& V1, const SVector4& V2);
	SVector4 operator* (float S, const SVector4& V);

	//------------------------------------------------------------------------------
	// 4x4 SDxMatrix (assumes right-handed cooordinates)
	struct SDxMatrix : public XMFLOAT4X4
	{
		SDxMatrix() noexcept
			: XMFLOAT4X4(1.f, 0, 0, 0,
				0, 1.f, 0, 0,
				0, 0, 1.f, 0,
				0, 0, 0, 1.f) {}
		XM_CONSTEXPR SDxMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: XMFLOAT4X4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33) {}
		explicit SDxMatrix(const SVector3& r0, const SVector3& r1, const SVector3& r2)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
				r1.x, r1.y, r1.z, 0,
				r2.x, r2.y, r2.z, 0,
				0, 0, 0, 1.f) {}
		explicit SDxMatrix(const SVector4& r0, const SVector4& r1, const SVector4& r2, const SVector4& r3)
			: XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
				r1.x, r1.y, r1.z, r1.w,
				r2.x, r2.y, r2.z, r2.w,
				r3.x, r3.y, r3.z, r3.w) {}
		SDxMatrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }
		SDxMatrix(const XMFLOAT3X3& M);
		SDxMatrix(const XMFLOAT4X3& M);

		explicit SDxMatrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
		SDxMatrix(CXMMATRIX M) { XMStoreFloat4x4(this, M); }

		SDxMatrix(const SDxMatrix&) = default;
		SDxMatrix& operator=(const SDxMatrix&) = default;

		SDxMatrix(SDxMatrix&&) = default;
		SDxMatrix& operator=(SDxMatrix&&) = default;

		operator XMFLOAT4X4() const { return *this; }

		// Comparison operators
		bool operator == (const SDxMatrix& M) const;
		bool operator != (const SDxMatrix& M) const;

		// Assignment operators
		SDxMatrix& operator= (const XMFLOAT3X3& M);
		SDxMatrix& operator= (const XMFLOAT4X3& M);
		SDxMatrix& operator+= (const SDxMatrix& M);
		SDxMatrix& operator-= (const SDxMatrix& M);
		SDxMatrix& operator*= (const SDxMatrix& M);
		SDxMatrix& operator*= (float S);
		SDxMatrix& operator/= (float S);

		SDxMatrix& operator/= (const SDxMatrix& M);
		// Element-wise divide

		// Unary operators
		SDxMatrix operator+ () const { return *this; }
		SDxMatrix operator- () const;

		// Properties
		SVector3 Up() const { return SVector3(_21, _22, _23); }
		void Up(const SVector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		SVector3 Down() const { return SVector3(-_21, -_22, -_23); }
		void Down(const SVector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		SVector3 Right() const { return SVector3(_11, _12, _13); }
		void Right(const SVector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		SVector3 Left() const { return SVector3(-_11, -_12, -_13); }
		void Left(const SVector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		SVector3 Forward() const { return SVector3(-_31, -_32, -_33); }
		void Forward(const SVector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		SVector3 Backward() const { return SVector3(_31, _32, _33); }
		void Backward(const SVector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

		SVector3 Translation() const { return SVector3(_41, _42, _43); }
		void Translation(const SVector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// SDxMatrix operations
		bool Decompose(SVector3& scale, SQuaternion& rotation, SVector3& translation);

		SDxMatrix Transpose() const;
		void Transpose(SDxMatrix& result) const;

		SDxMatrix Invert() const;
		void Invert(SDxMatrix& result) const;

		float Determinant() const;

		// Static functions
		static SDxMatrix CreateBillboard(const SVector3& object, const SVector3& cameraPosition, const SVector3& cameraUp, _In_opt_ const SVector3* cameraForward = nullptr);

		static SDxMatrix CreateConstrainedBillboard(const SVector3& object, const SVector3& cameraPosition, const SVector3& rotateAxis,
			_In_opt_ const SVector3* cameraForward = nullptr, _In_opt_ const SVector3* objectForward = nullptr);

		static SDxMatrix CreateTranslation(const SVector3& position);
		static SDxMatrix CreateTranslation(float x, float y, float z);

		static SDxMatrix CreateScale(const SVector3& scales);
		static SDxMatrix CreateScale(float xs, float ys, float zs);
		static SDxMatrix CreateScale(float scale);

		static SDxMatrix CreateRotationX(float radians);
		static SDxMatrix CreateRotationY(float radians);
		static SDxMatrix CreateRotationZ(float radians);

		static SDxMatrix CreateFromAxisAngle(const SVector3& axis, float angle);

		static SDxMatrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
		static SDxMatrix CreatePerspective(float width, float height, float nearPlane, float farPlane);
		static SDxMatrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		static SDxMatrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		static SDxMatrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

		static SDxMatrix CreateLookAt(const SVector3& position, const SVector3& target, const SVector3& up);
		static SDxMatrix CreateWorld(const SVector3& position, const SVector3& forward, const SVector3& up);

		static SDxMatrix CreateFromQuaternion(const SQuaternion& quat);

		static SDxMatrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

		static SDxMatrix CreateShadow(const SVector3& lightDir, const SPlane& plane);

		static SDxMatrix CreateReflection(const SPlane& plane);

		static void Lerp(const SDxMatrix& M1, const SDxMatrix& M2, float t, SDxMatrix& result);
		static SDxMatrix Lerp(const SDxMatrix& M1, const SDxMatrix& M2, float t);

		static void Transform(const SDxMatrix& M, const SQuaternion& rotation, SDxMatrix& result);
		static SDxMatrix Transform(const SDxMatrix& M, const SQuaternion& rotation);

		// Constants
		static const SDxMatrix Identity;
	};

	// Binary operators
	SDxMatrix operator+ (const SDxMatrix& M1, const SDxMatrix& M2);
	SDxMatrix operator- (const SDxMatrix& M1, const SDxMatrix& M2);
	SDxMatrix operator* (const SDxMatrix& M1, const SDxMatrix& M2);
	SDxMatrix operator* (const SDxMatrix& M, float S);
	SDxMatrix operator/ (const SDxMatrix& M, float S);
	SDxMatrix operator/ (const SDxMatrix& M1, const SDxMatrix& M2);
	// Element-wise divide
	SDxMatrix operator* (float S, const SDxMatrix& M);


	//-----------------------------------------------------------------------------
	// SPlane
	struct SPlane : public XMFLOAT4
	{
		SPlane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
		XM_CONSTEXPR SPlane(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		SPlane(const SVector3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
		SPlane(const SVector3& point1, const SVector3& point2, const SVector3& point3);
		SPlane(const SVector3& point, const SVector3& normal);
		explicit SPlane(const SVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit SPlane(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		SPlane(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		SPlane(const XMFLOAT4& p) { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
		explicit SPlane(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		SPlane(const SPlane&) = default;
		SPlane& operator=(const SPlane&) = default;

		SPlane(SPlane&&) = default;
		SPlane& operator=(SPlane&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const SPlane& p) const;
		bool operator != (const SPlane& p) const;

		// Assignment operators
		SPlane& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

		// Properties
		SVector3 Normal() const { return SVector3(x, y, z); }
		void Normal(const SVector3& normal) { x = normal.x; y = normal.y; z = normal.z; }

		float D() const { return w; }
		void D(float d) { w = d; }

		// SPlane operations
		void Normalize();
		void Normalize(SPlane& result) const;

		float Dot(const SVector4& v) const;
		float DotCoordinate(const SVector3& position) const;
		float DotNormal(const SVector3& normal) const;

		// Static functions
		static void Transform(const SPlane& plane, const SDxMatrix& M, SPlane& result);
		static SPlane Transform(const SPlane& plane, const SDxMatrix& M);

		static void Transform(const SPlane& plane, const SQuaternion& rotation, SPlane& result);
		static SPlane Transform(const SPlane& plane, const SQuaternion& rotation);
		// Input quaternion must be the inverse transpose of the transformation
	};

	//------------------------------------------------------------------------------
	// SQuaternion
	struct SQuaternion : public XMFLOAT4
	{
		SQuaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR SQuaternion(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		SQuaternion(const SVector3& v, float scalar) : XMFLOAT4(v.x, v.y, v.z, scalar) {}
		explicit SQuaternion(const SVector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit SQuaternion(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		SQuaternion(XMFLOAT4 V) { x = V.x; y = V.y; z = V.z; w = V.w; }
		SQuaternion(const XMFLOAT4& q) { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
		explicit SQuaternion(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		SQuaternion(const SQuaternion&) = default;
		SQuaternion& operator=(const SQuaternion&) = default;

		SQuaternion(SQuaternion&&) = default;
		SQuaternion& operator=(SQuaternion&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }

		// Comparison operators
		bool operator == (const SQuaternion& q) const;
		bool operator != (const SQuaternion& q) const;

		// Assignment operators
		SQuaternion& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		SQuaternion& operator+= (const SQuaternion& q);
		SQuaternion& operator-= (const SQuaternion& q);
		SQuaternion& operator*= (const SQuaternion& q);
		SQuaternion& operator*= (float S);
		SQuaternion& operator/= (const SQuaternion& q);

		// Unary operators
		SQuaternion operator+ () const { return *this; }
		SQuaternion operator- () const;

		// SQuaternion operations
		float Length() const;
		float LengthSquared() const;

		void Normalize();
		void Normalize(SQuaternion& result) const;

		void Conjugate();
		void Conjugate(SQuaternion& result) const;

		void Inverse(SQuaternion& result) const;

		float Dot(const SQuaternion& Q) const;

		// Static functions
		static SQuaternion CreateFromAxisAngle(const SVector3& axis, float angle);
		static SQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static SQuaternion CreateFromRotationMatrix(const SDxMatrix& M);

		static void Lerp(const SQuaternion& q1, const SQuaternion& q2, float t, SQuaternion& result);
		static SQuaternion Lerp(const SQuaternion& q1, const SQuaternion& q2, float t);

		static void Slerp(const SQuaternion& q1, const SQuaternion& q2, float t, SQuaternion& result);
		static SQuaternion Slerp(const SQuaternion& q1, const SQuaternion& q2, float t);

		static void Concatenate(const SQuaternion& q1, const SQuaternion& q2, SQuaternion& result);
		static SQuaternion Concatenate(const SQuaternion& q1, const SQuaternion& q2);

		// Constants
		static const SQuaternion Identity;
	};

	// Binary operators
	SQuaternion operator+ (const SQuaternion& Q1, const SQuaternion& Q2);
	SQuaternion operator- (const SQuaternion& Q1, const SQuaternion& Q2);
	SQuaternion operator* (const SQuaternion& Q1, const SQuaternion& Q2);
	SQuaternion operator* (const SQuaternion& Q, float S);
	SQuaternion operator/ (const SQuaternion& Q1, const SQuaternion& Q2);
	SQuaternion operator* (float S, const SQuaternion& Q);

	//------------------------------------------------------------------------------
	// SColor
	struct SColor : public XMFLOAT4
	{
		SColor() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
		XM_CONSTEXPR SColor(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
		XM_CONSTEXPR SColor(float _r, float _g, float _b, float _a) : XMFLOAT4(_r, _g, _b, _a) {}
		explicit SColor(const SVector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
		explicit SColor(const SVector4& clr) : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
		explicit SColor(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		SColor(XMFLOAT3 V) { x = V.x; y = V.y; z = V.z; }
		SColor(const XMFLOAT4& c) { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
		explicit SColor(const XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		explicit SColor(const DirectX::PackedVector::XMCOLOR& Packed);
		// BGRA Direct3D 9 D3DCOLOR packed color

		explicit SColor(const DirectX::PackedVector::XMUBYTEN4& Packed);
		// RGBA XNA Game Studio packed color

		SColor(const SColor&) = default;
		SColor& operator=(const SColor&) = default;

		SColor(SColor&&) = default;
		SColor& operator=(SColor&&) = default;

		operator XMVECTOR() const { return XMLoadFloat4(this); }
		operator const float*() const { return reinterpret_cast<const float*>(this); }

		// Comparison operators
		bool operator == (const SColor& c) const;
		bool operator != (const SColor& c) const;

		// Assignment operators
		SColor& operator= (const XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
		SColor& operator= (const DirectX::PackedVector::XMCOLOR& Packed);
		SColor& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed);
		SColor& operator+= (const SColor& c);
		SColor& operator-= (const SColor& c);
		SColor& operator*= (const SColor& c);
		SColor& operator*= (float S);
		SColor& operator/= (const SColor& c);

		// Unary operators
		SColor operator+ () const { return *this; }
		SColor operator- () const;

		// Properties
		float R() const { return x; }
		void R(float r) { x = r; }

		float G() const { return y; }
		void G(float g) { y = g; }

		float B() const { return z; }
		void B(float b) { z = b; }

		float A() const { return w; }
		void A(float a) { w = a; }

		// SColor operations
		DirectX::PackedVector::XMCOLOR BGRA() const;
		DirectX::PackedVector::XMUBYTEN4 RGBA() const;

		SVector3 ToVector3() const;
		SVector4 ToVector4() const;

		void Negate();
		void Negate(SColor& result) const;

		void Saturate();
		void Saturate(SColor& result) const;

		void Premultiply();
		void Premultiply(SColor& result) const;

		void AdjustSaturation(float sat);
		void AdjustSaturation(float sat, SColor& result) const;

		void AdjustContrast(float contrast);
		void AdjustContrast(float contrast, SColor& result) const;

		// Static functions
		static void Modulate(const SColor& c1, const SColor& c2, SColor& result);
		static SColor Modulate(const SColor& c1, const SColor& c2);

		static void Lerp(const SColor& c1, const SColor& c2, float t, SColor& result);
		static SColor Lerp(const SColor& c1, const SColor& c2, float t);
	};

	// Binary operators
	SColor operator+ (const SColor& C1, const SColor& C2);
	SColor operator- (const SColor& C1, const SColor& C2);
	SColor operator* (const SColor& C1, const SColor& C2);
	SColor operator* (const SColor& C, float S);
	SColor operator/ (const SColor& C1, const SColor& C2);
	SColor operator* (float S, const SColor& C);

	//------------------------------------------------------------------------------
	// SRay
	class SRay
	{
	public:
		SVector3 position;
		SVector3 direction;

		SRay() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
		SRay(const SVector3& pos, const SVector3& dir) : position(pos), direction(dir) {}

		SRay(const SRay&) = default;
		SRay& operator=(const SRay&) = default;

		SRay(SRay&&) = default;
		SRay& operator=(SRay&&) = default;

		// Comparison operators
		bool operator == (const SRay& r) const;
		bool operator != (const SRay& r) const;

		// SRay operations
		bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const;
		bool Intersects(const BoundingBox& box, _Out_ float& Dist) const;
		bool Intersects(const SVector3& tri0, const SVector3& tri1, const SVector3& tri2, _Out_ float& Dist) const;
		bool Intersects(const SPlane& plane, _Out_ float& Dist) const;
	};

	//------------------------------------------------------------------------------
	// SViewport
	class SViewport
	{
	public:
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;

		SViewport() noexcept :
			x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f) {}
		XM_CONSTEXPR SViewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) :
			x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz) {}
		explicit SViewport(const RECT& rct) :
			x(float(rct.left)), y(float(rct.top)),
			width(float(rct.right - rct.left)),
			height(float(rct.bottom - rct.top)),
			minDepth(0.f), maxDepth(1.f) {}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
		// Direct3D 11 interop
		explicit SViewport(const D3D11_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D11_VIEWPORT() { return *reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		const D3D11_VIEWPORT* Get11() const { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }
		SViewport& operator= (const D3D11_VIEWPORT& vp);
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
		// Direct3D 12 interop
		explicit SViewport(const D3D12_VIEWPORT& vp) :
			x(vp.TopLeftX), y(vp.TopLeftY),
			width(vp.Width), height(vp.Height),
			minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

		operator D3D12_VIEWPORT() { return *reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		const D3D12_VIEWPORT* Get12() const { return reinterpret_cast<const D3D12_VIEWPORT*>(this); }
		SViewport& operator= (const D3D12_VIEWPORT& vp);
#endif

		SViewport(const SViewport&) = default;
		SViewport& operator=(const SViewport&) = default;

		SViewport(SViewport&&) = default;
		SViewport& operator=(SViewport&&) = default;

		// Comparison operators
		bool operator == (const SViewport& vp) const;
		bool operator != (const SViewport& vp) const;

		// Assignment operators
		SViewport& operator= (const RECT& rct);

		// SViewport operations
		float AspectRatio() const;

		SVector3 Project(const SVector3& p, const SDxMatrix& proj, const SDxMatrix& view, const SDxMatrix& world) const;
		void Project(const SVector3& p, const SDxMatrix& proj, const SDxMatrix& view, const SDxMatrix& world, SVector3& result) const;

		SVector3 Unproject(const SVector3& p, const SDxMatrix& proj, const SDxMatrix& view, const SDxMatrix& world) const;
		void Unproject(const SVector3& p, const SDxMatrix& proj, const SDxMatrix& view, const SDxMatrix& world, SVector3& result) const;

		// Static methods
		static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight);
		static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight);
	};


	///////////////////////////////////////// static ///////////////////////////////////////////
	//--------------------------
	// 2D Vector
	//--------------------------

	static float D3DXVec2Length(CONST SVector2 *pV)
	{
		return pV->Length();
	}

	static float D3DXVec2LengthSq(CONST SVector2 *pV)
	{
		return 0.0f;
	}

	static float D3DXVec2Dot(CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return 0.0f;
	}

	// Z component of ((x1,y1,0) cross (x2,y2,0))
	static float D3DXVec2CCW(CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return 0.0f;
	}

	static SVector2* D3DXVec2Add(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return pOut;
	}

	static SVector2* D3DXVec2Subtract(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2)
	static SVector2* D3DXVec2Minimize(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2)
	static SVector2* D3DXVec2Maximize(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2)
	{
		return pOut;
	}

	static SVector2* D3DXVec2Scale(SVector2 *pOut, CONST SVector2 *pV, float s)
	{
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static SVector2* D3DXVec2Lerp(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2,
		float s)
	{
		return pOut;
	}
	static SVector2* D3DXVec2Normalize(SVector2 *pOut, CONST SVector2 *pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static SVector2* D3DXVec2Hermite(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pT1,
		CONST SVector2 *pV2, CONST SVector2 *pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static SVector2* D3DXVec2CatmullRom(SVector2 *pOut, CONST SVector2 *pV0, CONST SVector2 *pV1,
		CONST SVector2 *pV2, CONST SVector2 *pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static SVector2* D3DXVec2BaryCentric(SVector2 *pOut, CONST SVector2 *pV1, CONST SVector2 *pV2,
		CONST SVector2 *pV3, float f, float g)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix.
	static SVector4* D3DXVec2Transform(SVector4 *pOut, CONST SVector2 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 1) by matrix, project result back into w=1.
	static SVector2* D3DXVec2TransformCoord(SVector2 *pOut, CONST SVector2 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, 0, 0) by matrix.
	static SVector2* D3DXVec2TransformNormal(SVector2 *pOut, CONST SVector2 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix.
	static SVector4* D3DXVec2TransformArray(SVector4 *pOut, UINT OutStride, CONST SVector2 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
	static SVector2* D3DXVec2TransformCoordArray(SVector2 *pOut, UINT OutStride, CONST SVector2 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, 0, 0) by matrix.
	static SVector2* D3DXVec2TransformNormalArray(SVector2 *pOut, UINT OutStride, CONST SVector2 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}
	//--------------------------
	// 3D Vector
	//--------------------------

	static  float D3DXVec3Dot(CONST SVector3 *pV1, CONST SVector3 *pV2)
	{
		return pV1->Dot(*pV2);
	}
	static SVector3* D3DXVec3Cross(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2)
	{
		*pOut = pV1->Cross(*pV2);
		return pOut;
	}
	static SVector3* D3DXVec3Normalize(SVector3 *pOut, CONST SVector3 *pV)
	{
		pV->Normalize(*pOut);
		return pOut;
	}
	static SVector3* D3DXVec3TransformCoord(SVector3 *pOut, CONST SVector3 *pV, CONST SDxMatrix *pM)
	{
		*pOut = SVector3::Transform(*pV, *pM);
		return pOut;
	}
	static float D3DXVec3Length(CONST SVector3 *pV)
	{
		return pV->Length();
	}



	static float D3DXVec3LengthSq(CONST SVector3 *pV)
	{
		return 0.0f;
	}


	static SVector3* D3DXVec3Add(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2)
	{
		return pOut;
	}

	static SVector3* D3DXVec3Subtract(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static SVector3* D3DXVec3Minimize(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2) {
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static SVector3* D3DXVec3Maximize(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2)
	{
		return pOut;
	}

	static SVector3* D3DXVec3Scale(SVector3 *pOut, CONST SVector3 *pV, float s)
	{
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static SVector3* D3DXVec3Lerp(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2, float s)
	{
		return pOut;
	}


	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static SVector3* D3DXVec3Hermite(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pT1,
		CONST SVector3 *pV2, CONST SVector3 *pT2, float s)
	{
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static SVector3* D3DXVec3CatmullRom(SVector3 *pOut, CONST SVector3 *pV0, CONST SVector3 *pV1,
		CONST SVector3 *pV2, CONST SVector3 *pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static SVector3* D3DXVec3BaryCentric(SVector3 *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2,
		CONST SVector3 *pV3, float f, float g)
	{
		return pOut;
	}

	// Transform (x, y, z, 1) by matrix.
	static SVector4* D3DXVec3Transform(SVector4 *pOut, CONST SVector3 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static SVector3* D3DXVec3TransformNormal(SVector3 *pOut, CONST SVector3 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}


	// Transform Array (x, y, z, 1) by matrix. 
	static SVector4* D3DXVec3TransformArray(SVector4 *pOut, UINT OutStride, CONST SVector3 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
	static SVector3* D3DXVec3TransformCoordArray(SVector3 *pOut, UINT OutStride, CONST SVector3 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	static SVector3* D3DXVec3TransformNormalArray(SVector3 *pOut, UINT OutStride, CONST SVector3 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}

	// Project vector from object space into screen space
	static SVector3* D3DXVec3Project(SVector3 *pOut, CONST SVector3 *pV, CONST D3D10_VIEWPORT *pViewport,
		CONST SDxMatrix *pProjection, CONST SDxMatrix *pView, CONST SDxMatrix *pWorld)
	{
		return pOut;
	}

	// Project vector from screen space into object space
	static SVector3* D3DXVec3Unproject(SVector3 *pOut, CONST SVector3 *pV, CONST D3D10_VIEWPORT *pViewport,
		CONST SDxMatrix *pProjection, CONST SDxMatrix *pView, CONST SDxMatrix *pWorld)
	{
		return pOut;
	}

	// Project vector Array from object space into screen space
	static SVector3* D3DXVec3ProjectArray(SVector3 *pOut, UINT OutStride, CONST SVector3 *pV, UINT VStride, CONST D3D10_VIEWPORT *pViewport,
		CONST SDxMatrix *pProjection, CONST SDxMatrix *pView, CONST SDxMatrix *pWorld, UINT n)
	{
		return pOut;
	}

	// Project vector Array from screen space into object space
	static SVector3* D3DXVec3UnprojectArray(SVector3 *pOut, UINT OutStride, CONST SVector3 *pV, UINT VStride, CONST D3D10_VIEWPORT *pViewport,
		CONST SDxMatrix *pProjection, CONST SDxMatrix *pView, CONST SDxMatrix *pWorld, UINT n)
	{
		return pOut;
	}

	//--------------------------
	// 4D Vector
	//--------------------------

	static float D3DXVec4Length(CONST SVector4 *pV)
	{
		return pV->Length();
	}


	static float D3DXVec4LengthSq(CONST SVector4 *pV)
	{
		return 0.0f;
	}

	static float D3DXVec4Dot(CONST SVector4 *pV1, CONST SVector4 *pV2)
	{
		return 0.0f;
	}
	static SVector4* D3DXVec4Add(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2)
	{
		return pOut;
	}

	static SVector4* D3DXVec4Subtract(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2)
	{
		return pOut;
	}

	// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
	static SVector4* D3DXVec4Minimize(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2)
	{
		return pOut;
	}

	// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
	static SVector4* D3DXVec4Maximize(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2)
	{
		return pOut;
	}
	static SVector4* D3DXVec4Scale(SVector4 *pOut, CONST SVector4 *pV, float s) {
		return pOut;
	}

	// Linear interpolation. V1 + s(V2-V1)
	static SVector4* D3DXVec4Lerp(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2, float s)
	{
		return pOut;
	}

	// Cross-product in 4 dimensions.
	static SVector4* D3DXVec4Cross(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2,
		CONST SVector4 *pV3)
	{
		return pOut;
	}

	static SVector4* D3DXVec4Normalize(SVector4 *pOut, CONST SVector4 *pV)
	{
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	static SVector4* D3DXVec4Hermite(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pT1,
		CONST SVector4 *pV2, CONST SVector4 *pT2, float s)
	{
		return pOut;
	}

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	static SVector4* D3DXVec4CatmullRom(SVector4 *pOut, CONST SVector4 *pV0, CONST SVector4 *pV1,
		CONST SVector4 *pV2, CONST SVector4 *pV3, float s)
	{
		return pOut;
	}

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	static SVector4* D3DXVec4BaryCentric(SVector4 *pOut, CONST SVector4 *pV1, CONST SVector4 *pV2,
		CONST SVector4 *pV3, float f, float g)
	{
		return pOut;
	}

	// Transform vector by matrix.
	static SVector4* D3DXVec4Transform(SVector4 *pOut, CONST SVector4 *pV, CONST SDxMatrix *pM)
	{
		return pOut;
	}

	// Transform vector array by matrix.
	static SVector4* D3DXVec4TransformArray(SVector4 *pOut, UINT OutStride, CONST SVector4 *pV, UINT VStride, CONST SDxMatrix *pM, UINT n)
	{
		return pOut;
	}


	static SDxMatrix* D3DXMatrixIdentity(SDxMatrix *pOut)
	{
		pOut->Identity(4, 4);
		return pOut;
	}
	static SDxMatrix* D3DXMatrixRotationQuaternion(SDxMatrix *pOut, CONST SQuaternion *pQ)
	{
		*pOut = SDxMatrix::CreateFromQuaternion(*pQ);
		return pOut;
	}
	static SQuaternion* D3DXQuaternionRotationMatrix(SQuaternion *pOut, CONST SDxMatrix *pM)
	{
		*pOut = SQuaternion::CreateFromRotationMatrix(*pM);
		return pOut;
	}

	static float D3DXMatrixDeterminant(CONST SDxMatrix *pM) {
		return pM->Determinant();
	}

	static HRESULT D3DXMatrixDecompose(SVector3 *pOutScale, SQuaternion *pOutRotation,
		SVector3 *pOutTranslation, SDxMatrix *pM)
	{
		if (pM->Decompose(*pOutScale, *pOutRotation, *pOutTranslation) == false)
		{
			return S_FALSE;
		}
		return S_OK;
	}

	static SDxMatrix* D3DXMatrixTranspose(SDxMatrix *pOut, CONST SDxMatrix *pM) {

		return pOut;
	}

	// Matrix multiplication.  The result represents the transformation M2
	// followed by the transformation M1.  (Out = M1 * M2)
	static SDxMatrix*  D3DXMatrixMultiply(SDxMatrix *pOut, CONST SDxMatrix *pM1, CONST SDxMatrix *pM2) {
		*pOut = (*pM1) * (*pM2);
		return pOut;
	}

	// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
	static SDxMatrix*  D3DXMatrixMultiplyTranspose(SDxMatrix *pOut, CONST SDxMatrix *pM1, CONST SDxMatrix *pM2) {

		*pOut = (*pM1) * (*pM2);
		pOut->Transpose();
		return pOut;
	}

	// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
	// be returned.  The determinant of pM is also returned it pfDeterminant
	// is non-NULL.
	static SDxMatrix*  D3DXMatrixInverse(SDxMatrix *pOut, float *pDeterminant, CONST SDxMatrix *pM) {
		pM->Invert(*pOut);
		*pDeterminant = pM->Determinant();
		return pOut;
	}

	// Build a matrix which scales by (sx, sy, sz)
	static SDxMatrix*  D3DXMatrixScaling(SDxMatrix *pOut, float sx, float sy, float sz) {
		*pOut = SDxMatrix::CreateScale(sx, sy, sz);
		return pOut;
	}

	// Build a matrix which translates by (x, y, z)
	static SDxMatrix*  D3DXMatrixTranslation(SDxMatrix *pOut, float x, float y, float z) {
		*pOut = SDxMatrix::CreateTranslation(x, y, z);
		return pOut;
	}

	// Build a matrix which rotates around the X axis
	static SDxMatrix*  D3DXMatrixRotationX(SDxMatrix *pOut, float Angle) {
		*pOut = SDxMatrix::CreateRotationX(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Y axis
	static SDxMatrix*  D3DXMatrixRotationY(SDxMatrix *pOut, float Angle) {
		*pOut = SDxMatrix::CreateRotationY(Angle);
		return pOut;
	}

	// Build a matrix which rotates around the Z axis
	static SDxMatrix*  D3DXMatrixRotationZ(SDxMatrix *pOut, float Angle) {
		*pOut = SDxMatrix::CreateRotationZ(Angle);
		return pOut;
	}

	// Build a matrix which rotates around an arbitrary axis
	static SDxMatrix*  D3DXMatrixRotationAxis(SDxMatrix *pOut, CONST SVector3 *pV, float Angle) {
		*pOut = SDxMatrix::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static SDxMatrix*  D3DXMatrixRotationYawPitchRoll(SDxMatrix *pOut, float Yaw, float Pitch, float Roll) {
		*pOut = SDxMatrix::CreateFromYawPitchRoll(Yaw, Pitch, Roll);
		return pOut;
	}

	// Build transformation matrix.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static SDxMatrix*  D3DXMatrixTransformation(SDxMatrix *pOut, CONST SVector3 *pScalingCenter,
		CONST SQuaternion *pScalingRotation, CONST SVector3 *pScaling,
		CONST SVector3 *pRotationCenter, CONST SQuaternion *pRotation,
		CONST SVector3 *pTranslation)
	{
		return pOut;
	}

	// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	static SDxMatrix*  D3DXMatrixTransformation2D(SDxMatrix *pOut, CONST SVector2* pScalingCenter,
		float ScalingRotation, CONST SVector2* pScaling,
		CONST SVector2* pRotationCenter, float Rotation,
		CONST SVector2* pTranslation) {
		return pOut;
	}

	// Build affine transformation matrix.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static SDxMatrix*  D3DXMatrixAffineTransformation(SDxMatrix *pOut, float Scaling, CONST SVector3 *pRotationCenter,
		CONST SQuaternion *pRotation, CONST SVector3 *pTranslation) {
		return pOut;
	}

	// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
	// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	static SDxMatrix*  D3DXMatrixAffineTransformation2D(SDxMatrix *pOut, float Scaling, CONST SVector2* pRotationCenter,
		float Rotation, CONST SVector2* pTranslation) {
		return pOut;
	}

	// Build a lookat matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixLookAtRH(SDxMatrix *pOut, CONST SVector3 *pEye, CONST SVector3 *pAt, CONST SVector3 *pUp) {
		return pOut;
	}

	// Build a lookat matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixLookAtLH(SDxMatrix *pOut, CONST SVector3 *pEye, CONST SVector3 *pAt, CONST SVector3 *pUp) {
		*pOut = SDxMatrix::CreateLookAt(*pEye, *pAt, *pUp);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveRH(SDxMatrix *pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveLH(SDxMatrix *pOut, float w, float h, float zn, float zf) {
		*pOut = SDxMatrix::CreatePerspective(w, h, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveFovRH(SDxMatrix *pOut, float fovy, float Aspect, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveFovLH(SDxMatrix *pOut, float fovy, float Aspect, float zn, float zf) {
		*pOut = SDxMatrix::CreatePerspectiveFieldOfView(fovy, Aspect, zn, zf);
		return pOut;
	}

	// Build a perspective projection matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveOffCenterRH(SDxMatrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		return pOut;
	}

	// Build a perspective projection matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixPerspectiveOffCenterLH(SDxMatrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = SDxMatrix::CreatePerspectiveOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixOrthoRH(SDxMatrix *pOut, float w, float h, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixOrthoLH(SDxMatrix *pOut, float w, float h, float zn, float zf) {
		*pOut = SDxMatrix::CreateOrthographic(w, h, zn, zf);
		return pOut;
	}

	// Build an ortho projection matrix. (right-handed)
	static SDxMatrix*  D3DXMatrixOrthoOffCenterRH(SDxMatrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		return pOut;
	}

	// Build an ortho projection matrix. (left-handed)
	static SDxMatrix*  D3DXMatrixOrthoOffCenterLH(SDxMatrix *pOut, float l, float r, float b, float t, float zn, float zf) {
		*pOut = SDxMatrix::CreateOrthographicOffCenter(l, r, b, t, zn, zf);
		return pOut;
	}

	// Build a matrix which flattens geometry into a plane, as if casting
	// a shadow from a light.
	static SDxMatrix*  D3DXMatrixShadow(SDxMatrix *pOut, CONST SVector4 *pLight, CONST SPlane *pPlane) {
		//*pOut = SDxMatrix::CreateShadow( *pLight, *pPlane);
		return pOut;
	}

	// Build a matrix which reflects the coordinate system about a plane
	static SDxMatrix*  D3DXMatrixReflect(SDxMatrix *pOut, CONST SPlane *pPlane) {
		*pOut = SDxMatrix::CreateReflection(*pPlane);
		return pOut;
	}

	//--------------------------
	// Quaternion
	//--------------------------

	// inline

	static float D3DXQuaternionLength(CONST SQuaternion *pQ)
	{
		return 0.0f;
	};

	// Length squared, or "norm"
	static float D3DXQuaternionLengthSq(CONST SQuaternion *pQ) {
		return 0.0f;
	};

	static float D3DXQuaternionDot
	(CONST SQuaternion *pQ1, CONST SQuaternion *pQ2) {
		return 0.0f;
	};

	// (0, 0, 0, 1)
	static SQuaternion* D3DXQuaternionIdentity(SQuaternion *pOut) {
		return pOut;
	};

	static BOOL D3DXQuaternionIsIdentity(CONST SQuaternion *pQ) {
		return TRUE;
	};

	// (-x, -y, -z, w)
	static SQuaternion* D3DXQuaternionConjugate(SQuaternion *pOut, CONST SQuaternion *pQ) {
		return pOut;
	};


	// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
	static void D3DXQuaternionToAxisAngle(CONST SQuaternion *pQ, SVector3 *pAxis, float *pAngle) {
	};

	static SQuaternion* D3DXQuaternionRotationAxis(SQuaternion *pOut, CONST SVector3 *pV, FLOAT Angle)
	{
		*pOut = SQuaternion::CreateFromAxisAngle(*pV, Angle);
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	static SQuaternion* D3DXQuaternionRotationYawPitchRoll(SQuaternion *pOut, float Yaw, float Pitch, float Roll) {
		return pOut;
	};

	// Quaternion multiplication.  The result represents the rotation Q2
	// followed by the rotation Q1.  (Out = Q2 * Q1)
	static SQuaternion* D3DXQuaternionMultiply(SQuaternion *pOut, CONST SQuaternion *pQ1,
		CONST SQuaternion *pQ2) {
		return pOut;
	};

	static SQuaternion* D3DXQuaternionNormalize(SQuaternion *pOut, CONST SQuaternion *pQ) {
		return pOut;
	};

	// Conjugate and re-norm
	static SQuaternion* D3DXQuaternionInverse(SQuaternion *pOut, CONST SQuaternion *pQ) {
		return pOut;
	};

	// Expects unit quaternions.
	// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	static SQuaternion* D3DXQuaternionLn(SQuaternion *pOut, CONST SQuaternion *pQ) {
		return pOut;
	};

	// Expects pure quaternions. (w == 0)  w is ignored in calculation.
	// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
	static SQuaternion* D3DXQuaternionExp(SQuaternion *pOut, CONST SQuaternion *pQ) {
		return pOut;
	};

	// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
	// Expects unit quaternions.
	static SQuaternion* D3DXQuaternionSlerp(SQuaternion *pOut, CONST SQuaternion *pQ1,
		CONST SQuaternion *pQ2, float t) {
		return pOut;
	};

	// Spherical quadrangle interpolation.
	// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
	static SQuaternion* D3DXQuaternionSquad(SQuaternion *pOut, CONST SQuaternion *pQ1,
		CONST SQuaternion *pA, CONST SQuaternion *pB,
		CONST SQuaternion *pC, float t) {
		return pOut;
	};

	// Setup control points for spherical quadrangle interpolation
	// from Q1 to Q2.  The control points are chosen in such a way 
	// to ensure the continuity of tangents with adjacent segments.
	static void D3DXQuaternionSquadSetup(SQuaternion *pAOut, SQuaternion *pBOut, SQuaternion *pCOut,
		CONST SQuaternion *pQ0, CONST SQuaternion *pQ1,
		CONST SQuaternion *pQ2, CONST SQuaternion *pQ3) {
	};

	// Barycentric interpolation.
	// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
	static SQuaternion* D3DXQuaternionBaryCentric(SQuaternion *pOut, CONST SQuaternion *pQ1,
		CONST SQuaternion *pQ2, CONST SQuaternion *pQ3,
		float f, float g) {
		return pOut;
	};


	//--------------------------
	// Plane
	//--------------------------

	// inline

	// ax + by + cz + dw
	static float D3DXPlaneDot(CONST SPlane *pP, CONST SVector4 *pV) {
		return 0.0f;
	};
	// ax + by + cz + d
	static float D3DXPlaneDotCoord(CONST SPlane *pP, CONST SVector3 *pV) {
		return 0.0f;
	};
	// ax + by + cz
	static float D3DXPlaneDotNormal(CONST SPlane *pP, CONST SVector3 *pV) {
		return 0.0f;
	};
	static SPlane* D3DXPlaneScale(SPlane *pOut, CONST SPlane *pP, float s) {
		return pOut;
	};


	// Normalize plane (so that |a,b,c| == 1)
	static SPlane* D3DXPlaneNormalize(SPlane *pOut, CONST SPlane *pP) {
		return pOut;
	};

	// Find the intersection between a plane and a line.  If the line is
	// parallel to the plane, NULL is returned.
	static SVector3* D3DXPlaneIntersectLine(SVector3 *pOut, CONST SPlane *pP, CONST SVector3 *pV1,
		CONST SVector3 *pV2) {
		return pOut;
	};

	// Construct a plane from a point and a normal
	static SPlane* D3DXPlaneFromPointNormal(SPlane *pOut, CONST SVector3 *pPoint, CONST SVector3 *pNormal) {
		return pOut;
	};

	// Construct a plane from 3 points
	static SPlane* D3DXPlaneFromPoints(SPlane *pOut, CONST SVector3 *pV1, CONST SVector3 *pV2,
		CONST SVector3 *pV3) {
		return pOut;
	};

	// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
	// M should be the inverse transpose of the transformation desired.
	static SPlane* D3DXPlaneTransform(SPlane *pOut, CONST SPlane *pP, CONST SDxMatrix *pM) {
		return pOut;
	};
	// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
	static SPlane* D3DXPlaneTransformArray(SPlane *pOut, UINT OutStride, CONST SPlane *pP, UINT PStride, CONST SDxMatrix *pM, UINT n) {
		return pOut;
	};



	//--------------------------
	// Color
	//--------------------------

	// inline

	// (1-r, 1-g, 1-b, a)
	static SColor* D3DXColorNegative(SColor *pOut, CONST SColor *pC) {
		return pOut;
	};

	static SColor* D3DXColorAdd(SColor *pOut, CONST SColor *pC1, CONST SColor *pC2) {
		return pOut;
	};

	static SColor* D3DXColorSubtract(SColor *pOut, CONST SColor *pC1, CONST SColor *pC2) {
		return pOut;
	};

	static SColor* D3DXColorScale(SColor *pOut, CONST SColor *pC, float s) {
		return pOut;
	};

	// (r1*r2, g1*g2, b1*b2, a1*a2)
	static SColor* D3DXColorModulate(SColor *pOut, CONST SColor *pC1, CONST SColor *pC2) {
		return pOut;
	};

	// Linear interpolation of r,g,b, and a. C1 + s(C2-C1)
	static SColor* D3DXColorLerp(SColor *pOut, CONST SColor *pC1, CONST SColor *pC2, float s) {
		return pOut;
	};


	// Interpolate r,g,b between desaturated color and color.
	// DesaturatedColor + s(Color - DesaturatedColor)
	static SColor* D3DXColorAdjustSaturation(SColor *pOut, CONST SColor *pC, float s) {
		return pOut;
	};
	// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
	static SColor* D3DXColorAdjustContrast(SColor *pOut, CONST SColor *pC, float c) {
		return pOut;
	};





	//--------------------------
	// Misc
	//--------------------------
		// Calculate Fresnel term given the cosine of theta (likely obtained by
		// taking the dot of two normals), and the refraction index of the material.
	static float D3DXFresnelTerm(float CosTheta, float RefractionIndex)
	{
		return 0.0f;
	};
}

//  ------------------------------------------------------------------------------
// Support for SMMath and Standard C++ Library containers
namespace std
{
	template<> struct less<SBASIS_EX::SRectangle>
	{
		bool operator()(const SBASIS_EX::SRectangle& r1, const SBASIS_EX::SRectangle& r2) const
		{
			return ((r1.x < r2.x)
				|| ((r1.x == r2.x) && (r1.y < r2.y))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width < r2.width))
				|| ((r1.x == r2.x) && (r1.y == r2.y) && (r1.width == r2.width) && (r1.height < r2.height)));
		}
	};

	template<> struct less<SBASIS_EX::SVector2>
	{
		bool operator()(const SBASIS_EX::SVector2& V1, const SBASIS_EX::SVector2& V2) const
		{
			return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
		}
	};

	template<> struct less<SBASIS_EX::SVector3>
	{
		bool operator()(const SBASIS_EX::SVector3& V1, const SBASIS_EX::SVector3& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
		}
	};

	template<> struct less<SBASIS_EX::SVector4>
	{
		bool operator()(const SBASIS_EX::SVector4& V1, const SBASIS_EX::SVector4& V2) const
		{
			return ((V1.x < V2.x)
				|| ((V1.x == V2.x) && (V1.y < V2.y))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
				|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
		}
	};

	template<> struct less<SBASIS_EX::SDxMatrix>
	{
		bool operator()(const SBASIS_EX::SDxMatrix& M1, const SBASIS_EX::SDxMatrix& M2) const
		{
			if (M1._11 != M2._11) return M1._11 < M2._11;
			if (M1._12 != M2._12) return M1._12 < M2._12;
			if (M1._13 != M2._13) return M1._13 < M2._13;
			if (M1._14 != M2._14) return M1._14 < M2._14;
			if (M1._21 != M2._21) return M1._21 < M2._21;
			if (M1._22 != M2._22) return M1._22 < M2._22;
			if (M1._23 != M2._23) return M1._23 < M2._23;
			if (M1._24 != M2._24) return M1._24 < M2._24;
			if (M1._31 != M2._31) return M1._31 < M2._31;
			if (M1._32 != M2._32) return M1._32 < M2._32;
			if (M1._33 != M2._33) return M1._33 < M2._33;
			if (M1._34 != M2._34) return M1._34 < M2._34;
			if (M1._41 != M2._41) return M1._41 < M2._41;
			if (M1._42 != M2._42) return M1._42 < M2._42;
			if (M1._43 != M2._43) return M1._43 < M2._43;
			if (M1._44 != M2._44) return M1._44 < M2._44;

			return false;
		}
	};

	template<> struct less<SBASIS_EX::SPlane>
	{
		bool operator()(const SBASIS_EX::SPlane& P1, const SBASIS_EX::SPlane& P2) const
		{
			return ((P1.x < P2.x)
				|| ((P1.x == P2.x) && (P1.y < P2.y))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z < P2.z))
				|| ((P1.x == P2.x) && (P1.y == P2.y) && (P1.z == P2.z) && (P1.w < P2.w)));
		}
	};

	template<> struct less<SBASIS_EX::SQuaternion>
	{
		bool operator()(const SBASIS_EX::SQuaternion& Q1, const SBASIS_EX::SQuaternion& Q2) const
		{
			return ((Q1.x < Q2.x)
				|| ((Q1.x == Q2.x) && (Q1.y < Q2.y))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
				|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
		}
	};

	template<> struct less<SBASIS_EX::SColor>
	{
		bool operator()(const SBASIS_EX::SColor& C1, const SBASIS_EX::SColor& C2) const
		{
			return ((C1.x < C2.x)
				|| ((C1.x == C2.x) && (C1.y < C2.y))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z < C2.z))
				|| ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z == C2.z) && (C1.w < C2.w)));
		}
	};

	template<> struct less<SBASIS_EX::SRay>
	{
		bool operator()(const SBASIS_EX::SRay& R1, const SBASIS_EX::SRay& R2) const
		{
			if (R1.position.x != R2.position.x) return R1.position.x < R2.position.x;
			if (R1.position.y != R2.position.y) return R1.position.y < R2.position.y;
			if (R1.position.z != R2.position.z) return R1.position.z < R2.position.z;

			if (R1.direction.x != R2.direction.x) return R1.direction.x < R2.direction.x;
			if (R1.direction.y != R2.direction.y) return R1.direction.y < R2.direction.y;
			if (R1.direction.z != R2.direction.z) return R1.direction.z < R2.direction.z;

			return false;
		}
	};

	template<> struct less<SBASIS_EX::SViewport>
	{
		bool operator()(const SBASIS_EX::SViewport& vp1, const SBASIS_EX::SViewport& vp2) const
		{
			if (vp1.x != vp2.x) return (vp1.x < vp2.x);
			if (vp1.y != vp2.y) return (vp1.y < vp2.y);

			if (vp1.width != vp2.width) return (vp1.width < vp2.width);
			if (vp1.height != vp2.height) return (vp1.height < vp2.height);

			if (vp1.minDepth != vp2.minDepth) return (vp1.minDepth < vp2.minDepth);
			if (vp1.maxDepth != vp2.maxDepth) return (vp1.maxDepth < vp2.maxDepth);

			return false;
		}
	};


};

