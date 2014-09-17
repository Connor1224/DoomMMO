/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_Vec2f_H
#define Navigation_Vec2f_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/base/endianness.h"
#include <math.h>


namespace Kaim
{

/// This class defines a two-dimensional vector whose coordinates are stored using
/// floating-point numbers.
class Vec2f
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	// ---------------------------------- Creation & Initialization ----------------------------------

	/// Creates a vector with coordinates (0,0). 
	Vec2f() : x(0.0f), y(0.0f) {}

	/// Creates a vector with the specified coordinates.
	/// \param _x			The size of the vector on the X axis.
	/// \param _y			The size of the vector on the Y axis. 
	Vec2f(KyFloat32 _x, KyFloat32 _y) : x(_x), y(_y) {}

	/// Creates a vector with the specified coordinates.
	/// \param coords		An array of two floating-point numbers that contains the size of the
	/// 					vector along the X and Y axes respectively. 
	explicit Vec2f(KyFloat32* coords) { Set(coords); }

	/// Sets the coordinates of the vector to match the specified values.
	/// \param _x			The size of the vector on the X axis.
	/// \param _y			The size of the vector on the Y axis. 
	KY_INLINE void Set(KyFloat32 _x, KyFloat32 _y) { x = _x; y = _y; }

	/// Sets the coordinates of the vector to match the specified values.
	/// \param coords		An array of two floating-point numbers that contains the size of the
	/// 					vector along the X and Y axes respectively. 
	KY_INLINE void Set(KyFloat32* coords) { x = coords[0]; y = coords[1]; }


	// -------------------------- Operators --------------------------------

	/// Retrieves the size of the vector around one of its axes. Use [0] for the X axis, or [1] for
	/// the Y axis. 
	KY_INLINE KyFloat32& operator[](KyInt32 i) { return (&x)[i]; }

	/// Retrieves the size of the vector around one of its axes. Use [0] for the X axis, or [1] for
	/// the Y axis. 
	KY_INLINE KyFloat32 operator[](KyInt32 i) const { return (&x)[i]; }

	/// Returns true if this object contains the same coordinates as v. 
	KY_INLINE bool operator==(const Vec2f& v) const { return x == v.x && y == v.y; }

	/// Returns true if this object contains at least one different coordinate from v. 
	KY_INLINE bool operator!=(const Vec2f& v) const { return x != v.x || y != v.y; }

	/// Returns true if the size of this vector along both the X and Y axes is less than that of v. 
	KY_INLINE bool operator<(const Vec2f& v) const;

	/// Multiplies both the X and Y coordinates of this vector by the specified value. 
	KY_INLINE Vec2f& operator*=(KyFloat32 s)    { x *= s; y *= s;     return *this; }

	/// Divides both the X and Y coordinates of this vector by the specified value. 
	KY_INLINE Vec2f& operator/=(KyFloat32 d)    { return operator*=(1.0f / d);      }

	/// Adds the X coordinate of v to the X coordinate of this vector, and adds the Y coordinate of v
	/// to the Y coordinate of this vector. 
	KY_INLINE Vec2f& operator+=(const Vec2f& v) { x += v.x; y += v.y; return *this; }

	/// Subtracts the X coordinate of v from the X coordinate of this vector, and subtracts the Y coordinate of v
	/// from the Y coordinate of this vector. 
	KY_INLINE Vec2f& operator-=(const Vec2f& v) { x -= v.x; y -= v.y; return *this; }

	/// Multiplies both the X and Y coordinates of the vector by the specified value. 
	KY_INLINE Vec2f operator*(KyFloat32 s)const { return Vec2f(x * s, y * s); }

	/// Divides both the X and Y coordinates of the vector by the specified value. 
	KY_INLINE Vec2f operator/(KyFloat32 d) const { return operator*(1.0f / d); }

	/// Adds the X coordinate of v to the X coordinate of this vector, and adds the Y coordinate of v
	/// to the Y coordinate of this vector. 
	KY_INLINE Vec2f operator+(const Vec2f& v) const { return Vec2f(x + v.x, y + v.y); }

	/// Subtracts the X coordinate of v from the X coordinate of this vector, and subtracts the Y coordinate of v
	/// from the Y coordinate of this vector. 
	KY_INLINE Vec2f operator-(const Vec2f& v) const { return Vec2f(x - v.x, y - v.y); }

	/// Negates the X and Y coordinates of this vector, effectively flipping it around the origin. 
	KY_INLINE Vec2f operator-() const { return Vec2f(-x, -y); }

	/// Returns the dot product of this vector and v. 
	KY_INLINE KyFloat32 operator*(const Vec2f& v) const { return x * v.x + y * v.y; } // DOT PRODUCT

	/// Returns the magnitude on the Z axis of the cross product between this vector and v. 
	KY_INLINE KyFloat32 operator^(const Vec2f& v) const { return x * v.y - y * v.x; } // CROSS PRODUCT


	// -------------------------- Main interface --------------------------------

	/// Returns the square of the magnitude of the vector. 
	KY_INLINE KyFloat32 GetSquareLength() const { return x * x + y * y; }

	/// Returns the magnitude of the vector. 
	KY_INLINE KyFloat32 GetLength() const { return sqrtf(x * x + y * y); }

	/// Normalizes the vector, making it one unit in length without changing its orientation.
	/// \return The previous magnitude of this vector before normalization. 
	KY_INLINE KyFloat32 Normalize();

	/// Normalizes the vector, making it one unit in length without changing its orientation.
	/// This method does not change this object. Instead, it stores the computed normal in
	/// the normalized parameter.
	/// \param[out] normalized			Stores the normalized vector computed from this vector.
	/// \return The magnitude of this vector before normalization. 
	KY_INLINE KyFloat32 GetNormalized(Vec2f& normalized) const;

	/// Rotates this vector 90 degrees clockwise (negating the X coordinate). 
	KY_INLINE Vec2f PerpCW() const { return Vec2f(y, -x); }

	/// Rotates this vector 90 degrees counter-clockwise (negating the Y coordinate). 
	KY_INLINE Vec2f PerpCCW() const { return Vec2f(-y, x); }


	// -------------------------- Static methods --------------------------------

	/// Returns the normalized orientation of the X axis. 
	static KY_INLINE Vec2f UnitX() { return Vec2f(1.0f, 0.0f); }

	/// Returns the normalized orientation of the Y axis. 
	static KY_INLINE Vec2f UnitY() { return Vec2f(0.0f, 1.0f); }


	// ---------------------------------- Public Data Members ----------------------------------

	KyFloat32 x; ///< The size of the vector along the X axis. 
	KyFloat32 y; ///< The size of the vector along the Y axis. 
};

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped.
inline void SwapEndianness(Endianness::Target e, Vec2f& self)
{
	SwapEndianness(e, self.x);
	SwapEndianness(e, self.y);
}

// ----------------------------------- global functions -----------------------------------
/// Multiplies the X and Y coordinates of v by s.  
KY_INLINE Vec2f operator*(KyFloat32 s, const Vec2f& v) { return Vec2f(v.x * s, v.y * s); }

/// Returns the dot product of v1 and v2.  
KY_INLINE KyFloat32 DotProduct(const Vec2f& v1, const Vec2f& v2) { return (v1.x * v2.x + v1.y * v2.y); }

/// Returns the magnitude on the Z axis of the cross product between this vector and v.  
KY_INLINE KyFloat32 CrossProduct (const Vec2f& v1, const Vec2f& v2) { return v1 ^ v2; }


/// Returns the square of the distance between v1 and v2.  
KY_INLINE KyFloat32 SquareDistance(const Vec2f& v1, const Vec2f& v2)
{
	const KyFloat32 dx = v2.x - v1.x;
	const KyFloat32 dy = v2.y - v1.y;
	return dx * dx + dy * dy;
}

/// Returns the distance between v1 and v2.  
KY_INLINE KyFloat32 Distance(const Vec2f& v1, const Vec2f& v2)
{
	return sqrtf(SquareDistance(v1, v2));
}

// ----------------------------------- inline implementation -----------------------------------

KY_INLINE  bool Vec2f::operator<(const Vec2f& v) const
{
	if (x < v.x) return true;
	if (x > v.x) return false;
	return (y < v.y);
}


KY_INLINE KyFloat32 Vec2f::Normalize()
{
	const KyFloat32 length = GetLength();
	if (length != 0.0f)
		operator/=(length);
	return length;
}


KY_INLINE KyFloat32 Vec2f::GetNormalized(Vec2f& normalized) const
{
	// don't call Vec2f::Normalize() to avoid LHS
	const KyFloat32 length = GetLength();
	if (length != 0.0f)
	{
		const KyFloat32 invLength = 1.f / length;
		normalized.Set(x * invLength, y * invLength);
		return length;
	}
	else
	{
		normalized.Set(0.f, 0.f);
		return 0.f;
	}
}

} // namespace Kaim

#endif
