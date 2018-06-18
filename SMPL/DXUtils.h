#pragma once

#include <d3d11_1.h>
#include <iostream>
#include <algorithm>
#include <Eigen/Eigen>

namespace smpl
{
	#define SAFE_RELEASE(p) do { if (p) { (p)->Release(); (p) = nullptr; } } while(0)

	#define VALIDATE(x, wstr) \
	do { HRESULT hr = (x); if( !SUCCEEDED(hr) ) \
	{ std::wcerr << "[ERROR] " << wstr << "\n"; } } while(0)

	#define V_RETURN(x, wstr) \
	do { HRESULT hr = (x); if( !SUCCEEDED(hr) ) \
	{ std::wcerr << "[ERROR] " << wstr << "\n"; return hr; } } while(0)

	union float2
	{
		struct { float x; float y; };
		float data[2];
		float2() : x(0.0f), y(0.0f) {};
		float2(float x, float y) : x(x), y(y) {};
		float2(const Eigen::Vector2f& v) : x(v.x()), y(v.y()) {};
		Eigen::Vector2f ToEigen() const { return Eigen::Vector2f(x, y); }
		float operator [](int i) const { return data[i]; }
		float& operator [](int i) { return data[i]; }
	};

	union float3
	{
		struct { float x; float y; float z; };
		float data[3];
		float3() : x(0.0f), y(0.0f), z(0.0f) {};
		float3(float x, float y, float z) : x(x), y(y), z(z) {};
		float3(const Eigen::Vector3f& v) : x(v.x()), y(v.y()), z(v.z()) {};
		Eigen::Vector3f ToEigen() const { return Eigen::Vector3f(x, y, z); }
		float3& operator=(float3 other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
			return *this;
		}
		float operator [](int i) const { return data[i]; }
		float& operator [](int i) { return data[i]; }
		
		friend std::ostream& operator<<(std::ostream& os, float3& other)
		{
			os << other.x << " " << other.y << " " << other.z;
			return os;
		}
	};

	union float4
	{
		struct { float x; float y; float z; float w; };
		float data[4];
		float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
		float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		float4(const Eigen::Vector4f& v) : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {};
		Eigen::Vector4f ToEigen() const { return Eigen::Vector4f(x, y, z, w); }
		float operator [](int i) const { return data[i]; }
		float& operator [](int i) { return data[i]; }
	};

	union int2
	{
		struct { int x; int y; };
		int data[2];
		int2() : x(0), y(0) {};
		int2(int x, int y) : x(x), y(y) {};
		int2(const Eigen::Vector2i& v) : x(v.x()), y(v.y()) {};
		Eigen::Vector2i ToEigen() const { return Eigen::Vector2i(x, y); }
		int operator [](int i) const { return data[i]; }
		int& operator [](int i) { return data[i]; }
	};

	union int3
	{
		struct { int x; int y; int z; };
		int data[3];
		int3() : x(0), y(0), z(0) {};
		int3(int x, int y, int z) : x(x), y(y), z(z) {};
		int3(const Eigen::Vector3i& v) : x(v.x()), y(v.y()), z(v.z()) {};
		Eigen::Vector3i ToEigen() const { return Eigen::Vector3i(x, y, z); }
		int operator [](int i) const { return data[i]; }
		int& operator [](int i) { return data[i]; }
	};

	union int4
	{
		struct { int x; int y; int z; int w; };
		int data[4];
		int4() : x(0), y(0), z(0), w(0) {};
		int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {};
		int4(const Eigen::Vector4i& v) : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {};
		Eigen::Vector4i ToEigen() const { return Eigen::Vector4i(x, y, z, w); }
		int operator [](int i) const { return data[i]; }
		int& operator [](int i) { return data[i]; }
	};

	typedef unsigned int uint;

	union uint2
	{
		struct { unsigned int x; unsigned int y; };
		unsigned int data[2];
		uint2() : x(0u), y(0u) {};
		uint2(unsigned int x, unsigned int y) : x(x), y(y) {};
		uint2(const Eigen::Vector2i& v) : x(v.x()), y(v.y()) {};
		Eigen::Vector2i ToEigen() const { return Eigen::Vector2i(x, y); }
		uint operator [](int i) const { return data[i]; }
		uint& operator [](int i) { return data[i]; }
	};

	union uint3
	{
		struct { unsigned int x; unsigned int y; unsigned int z; };
		unsigned int data[3];
		uint3() : x(0u), y(0u), z(0u) {};
		uint3(unsigned int x, unsigned int y, unsigned int z) : x(x), y(y), z(z) {};
		uint3(const Eigen::Vector3i& v) : x(v.x()), y(v.y()), z(v.z()) {};
		Eigen::Vector3i ToEigen() const { return Eigen::Vector3i(x, y, z); }
		uint operator [](int i) const { return data[i]; }
		uint& operator [](int i) { return data[i]; }
	};

	union uint4
	{
		struct { unsigned int x; unsigned int y; unsigned int z; unsigned int w; };
		unsigned int data[4];
		uint4() : x(0u), y(0u), z(0u), w(0u) {};
		uint4(unsigned int x, unsigned int y, unsigned int z, unsigned int w) : x(x), y(y), z(z), w(w) {};
		uint4(const Eigen::Vector4i& v) : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {};
		Eigen::Vector4i ToEigen() const { return Eigen::Vector4i(x, y, z, w); }
		uint operator [](int i) const { return data[i]; }
		uint& operator [](int i) { return data[i]; }
	};

	union float2x2
	{
		struct
		{
			float m00; float m01;
			float m10; float m11;
		};
		float data[4];
		float2x2() : m00(0.0f), m01(0.0f), m10(0.0f), m11(0.0f) {};
		float2x2(float m00, float m01, float m10, float m11) : m00(m00), m01(m01), m10(m10), m11(m11) {};
		float2x2(const Eigen::Matrix2f& m) { memcpy(data, m.data(), 4 * sizeof(float)); };
		void SetIdentity() { m00 = m11 = 1.0f;  m01 = m10 = 0.0f; }
		Eigen::Matrix2f ToEigen() const { Eigen::Matrix2f res; memcpy(res.data(), data, 4 * sizeof(float)); return res; }
	};

	union float3x3
	{
		struct
		{
			float m00; float m01; float m02;
			float m10; float m11; float m12;
			float m20; float m21; float m22;
		};
		float data[9];
		float3x3() : m00(0.0f), m01(0.0f), m02(0.0f), m10(0.0f), m11(0.0f), m12(0.0f), m20(0.0f), m21(0.0f), m22(0.0f) {};
		float3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) :
			m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22)
		{
		};
		float3x3(const Eigen::Matrix4f& m) { memcpy(data, m.data(), 16 * sizeof(float)); };
		void SetIdentity() { m00 = m11 = m22 = 1.0f;  m01 = m02 = m10 = m12 = m20 = m21 = 0.0f; }
		Eigen::Matrix3f ToEigen() const { Eigen::Matrix3f res; memcpy(res.data(), data, 9 * sizeof(float)); return res; }
	};

	union float4x4
	{
		struct
		{
			float m00; float m01; float m02; float m03;
			float m10; float m11; float m12; float m13;
			float m20; float m21; float m22; float m23;
			float m30; float m31; float m32; float m33;
		};
		float data[16];
		float4x4() : m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f), m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f),
			m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f), m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f)
		{
		};
		float4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) :
			m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33)
		{
		};
		float4x4(const Eigen::Matrix4f& m) { memcpy(data, m.data(), 16 * sizeof(float)); };
		void SetIdentity() { m00 = m11 = m22 = m33 = 1.0f;  m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f; }
		Eigen::Matrix4f ToEigen() const { Eigen::Matrix4f res; memcpy(res.data(), data, 16 * sizeof(float)); return res; }
	};
} // smpl