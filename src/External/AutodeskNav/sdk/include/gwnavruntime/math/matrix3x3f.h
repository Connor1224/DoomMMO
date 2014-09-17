/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_Matrix3x3f_H
#define Navigation_Matrix3x3f_H


#include "gwnavruntime/math/vec3f.h"


namespace Kaim
{

/// This class represents a three-dimensional 6 freedom degrees unit box.
class Matrix3x3f
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	// ---------------------------------- Constructors ----------------------------------

	/// Creates a new identity Matrix3x3f.
	Matrix3x3f() { SetIdentity(); }

	/// Construct the matrix with elements set as explain in Set() method.
	Matrix3x3f(
		KyFloat32 xx, KyFloat32 xy, KyFloat32 xz,
		KyFloat32 yx, KyFloat32 yy, KyFloat32 yz,
		KyFloat32 zx, KyFloat32 zy, KyFloat32 zz)
	{
		Set(
			xx, xy, xz,
			yx, yy, yz,
			zx, zy, zz
			);
	}

	/// Construct the matrix with elements set as explain in SetRotation() method.
	Matrix3x3f(const Vec3f& axis, KyFloat32 angleRad)
	{
		SetRotation(axis, angleRad);
	}


	// ---------------------------------- Static Functions ----------------------------------

	static KY_INLINE Matrix3x3f Identity()
	{
		return Matrix3x3f(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}


	// ---------------------------------- Main API Functions ----------------------------------

	/// Set the values of the matrix explicitly (row major).
	/// \param xx		Top     left
	/// \param xy		Top     Middle
	/// \param xz		Top     Right
	/// \param yx		Middle  Left
	/// \param yy		Middle  Middle
	/// \param yz		Middle  Right
	/// \param zx		Bottom  Left
	/// \param zy		Bottom  Middle
	/// \param zz		Bottom  Right
	KY_INLINE void Set(
		KyFloat32 xx, KyFloat32 xy, KyFloat32 xz,
		KyFloat32 yx, KyFloat32 yy, KyFloat32 yz,
		KyFloat32 zx, KyFloat32 zy, KyFloat32 zz)
	{
		m_rows[0].Set(xx, xy, xz);
		m_rows[1].Set(yx, yy, yz);
		m_rows[2].Set(zx, zy, zz);
	}

	KY_INLINE void SetIdentity() { *this = Identity(); }

	/// Set as angleRad rotation matrix around axis.
	void SetRotation(const Vec3f& axis, KyFloat32 angleRad);

	/// Returns the product of this matrix and v. 
	KY_INLINE Vec3f operator*(const Vec3f& v) const { return Vec3f(m_rows[0] * v, m_rows[1] * v, m_rows[2] * v); }

	/// Returns the product of this matrix and m. 
	Matrix3x3f operator*(const Matrix3x3f& m) const;

	bool operator!=(const Matrix3x3f& other)
	{
		return (
			(m_rows[0] != other.m_rows[0]) ||
			(m_rows[1] != other.m_rows[1]) ||
			(m_rows[2] != other.m_rows[2]));
	}

	bool operator==(const Matrix3x3f& other) { return !(*this != other); }


	// ---------------------------------- Public Data Members ----------------------------------

	/// Data storage for the matrix, each vector is a row of the matrix.
	Vec3f m_rows[3];
};

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped. 
KY_INLINE void SwapEndianness(Endianness::Target e, Matrix3x3f& self)
{
	SwapEndianness(e, self.m_rows[0]);
	SwapEndianness(e, self.m_rows[1]);
	SwapEndianness(e, self.m_rows[2]);
}


} // namespace Kaim

#endif // Navigation_Matrix3x3f_H
