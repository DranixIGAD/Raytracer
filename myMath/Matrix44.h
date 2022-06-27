#pragma once
#include "Vector3.h"

class Matrix44
{
public:
	/// row x, column 1. */		/// row x, column 2. */		/// row x, column 3. */		/// row x, column 4. */
	float x1;					float x2;					float x3;					float x4;

	/// row y, column 1. */		/// row y, column 2. */		/// row y, column 3. */		/// row y, column 3. */
	float y1;					float y2;					float y3;					float y4;

	/// row z, column 1. */		/// row z, column 2. */		/// row z, column 3. */		/// row z, column 4. */
	float z1;					float z2;					float z3;					float z4;

	/// row a, column 1. */		/// row a, column 2. */		/// row a, column 3. */		/// row a, column 4  */
	float a1;					float a2;					float a3;					float a4;

public:
	/// Default 0 constructor
	Matrix44() : x1(0), x2(0), x3(0), x4(0),
				 y1(0), y2(0), y3(0), y4(0),
				 z1(0), z2(0), z3(0), z4(0),
				 a1(0), a2(0), a3(0), a4(0) { }

	/// Constructor from components
	Matrix44(float x1, float x2, float x3, float x4,
			 float y1, float y2, float y3, float y4,
			 float z1, float z2, float z3, float z4,
			 float a1, float a2, float a3, float a4)
			  : x1(x1), x2(x2), x3(x3), x4(x4),
				y1(y1), y2(y2), y3(y3), y4(y4),
				z1(z1), z2(z2), z3(z3), z4(z4),
				a1(a1), a2(a2), a3(a3), a4(a4) {}

	/// Constructor from component
	explicit Matrix44(float v) : x1(v), x2(v), x3(v), x4(v),
								 y1(v), y2(v), y3(v), y4(v),
								 z1(v), z2(v), z3(v), z4(v),
								 a1(v), a2(v), a3(v), a4(v) {}

	// copy constructor
	Matrix44(const Matrix44& copy) : x1(copy.x1), x2(copy.x2), x3(copy.x3), x4(copy.x4),
									 y1(copy.y1), y2(copy.y2), y3(copy.y3), y4(copy.y4),
									 z1(copy.z1), z2(copy.z2), z3(copy.z3), z4(copy.z4),
									 a1(copy.a1), a2(copy.a2), a3(copy.a3), a4(copy.a4) {}


	// Constructor for identity
	Matrix44(char /*mark_for_identity*/) :  x1(1), x2(0), x3(0), x4(0),
										y1(0), y2(1), y3(0), y4(0),
										z1(0), z2(0), z3(1), z4(0),
										a1(0), a2(0), a3(0), a4(1) {}

	// Create a look at matrix
	
	Matrix44(const Vector3& from, const Vector3& to) : x4(0), y4(0), z4(0),
													   a1(0), a2(0), a3(0), a4(1)
	{
		Vector3 forward = from - to;
		
		forward.Normalize();
		
		Vector3 temp(0,1,0);
		Vector3 right = temp.Cross(forward);
		
		Vector3 up = forward.Cross(right);
				
		x1 = right.x;	x2 = up.x;	x3 = forward.x;
		y1 = right.y;	y2 = up.y;	y3 = forward.y;
		z1 = right.z ;	z2 = up.z;	z3 = forward.z;
	}

	// Change this matrix to a rotation matrix around the x axis
	void makeXRotation(const float angle)
	{
		x1 = 1;		x2 = 0;				x3 = 0;				x4 = 0;
		y1 = 0;		y2 =cosf(angle);	y3 = -sinf(angle);	y4 = 0;
		z1 = 0;		z2 = sinf(angle);	z3 = cosf(angle);	z4 = 0;
		a1 = 0;		a2 = 0;				a3 = 0;				a4 = 1;
	}
	 
	// Change this matrix to a rotation matrix around the y axis
	void makeYRotation(float angle)
	{
		x1 = cosf(angle);	x2 = 0;		x3 = sinf(angle);	x4 = 0;
		y1 = 0;				y2 = 1;		y3 = 0;				y4 = 0;
		z1 = -sinf(angle);	z2 = 0;		z3 = cosf(angle);	z4 = 0;
		a1 = 0;				a2 = 0;		a3 = 0;				a4 = 1;
	}
	 
	// Change this matrix to a rotation matrix around the z axis
	void makeZRotation(float angle)
	{
		x1 = cosf(angle);	x2 = -sinf(angle);	x3 = 0;		x4 = 0;
		y1 = sinf(angle);	y2 = cosf(angle);	y3 = 0;		y4 = 0;
		z1 = 0;				z2 = 0;				z3 = 1;		z4 = 0;
		a1 = 0;				a2 = 0;				a3 = 0;		a4 = 1;
	}	 

	// change this matrix to a translation matrix
	void makeTranslation(float x_trans, float y_trans, float z_trans)
	{
		x1 = 1;		x2 = 0;		x3 = 0;		x4 = x_trans;
		y1 = 0;		y2 = 1;		y3 = 0;		y4 = y_trans;
		z1 = 0;		z2 = 0;		z3 = 1;		z4 = z_trans;
		a1 = 0;		a2 = 0;		a3 = 0;		a4 = 1;
	}
	
	// returns a vector multiplied by this matrix
	Vector3 operator*(const Vector3& vec3)
	{
		return Vector3(x1 * vec3.x + x2 * vec3.y + x3 * vec3.z + x4 * a4, y1 * vec3.x + y2 * vec3.y + y3 * vec3.z + y4 * a4, z1 * vec3.x + z2 * vec3.y + z3 * vec3.z + z4 * a4);
	}

	// same as above but allows a4 to be a set number for this calculation only
	Vector3 times(const Vector3& vec3, float temp_a4)
	{
		return Vector3(x1 * vec3.x + x2 * vec3.y + x3 * vec3.z + x4 * temp_a4, y1 * vec3.x + y2 * vec3.y + y3 * vec3.z + y4 * temp_a4, z1 * vec3.x + z2 * vec3.y + z3 * vec3.z + z4 * temp_a4);
	}

	/// Returns the value of the given matrix added to this.
	Matrix44 operator+(const Matrix44& v) const
	{
		return Matrix44(x1 + v.x1, x2 + v.x2, x3 + v.x3, x4 + v.x4,
						y1 + v.y1, y2 + v.y2, y3 + v.y3, y4 + v.y4,
						z1 + v.z1, z2 + v.z2, z3 + v.z3, z4 + v.z4,
						a1 + v.a1, a2 + v.a2, a3 + v.a3, a4 + v.a4);
	}

	/// Returns the value of the given matrix subtracted from this.
	Matrix44 operator-(const Matrix44& v) const
	{
		return Matrix44(x1 - v.x1, x2 - v.x2, x3 - v.x3, x4 - v.x4,
						y1 - v.y1, y2 - v.y2, y3 - v.y3, y4 - v.y4,
						z1 - v.z1, z2 - v.z2, z3 - v.z3, z4 - v.z4,
						a1 - v.a1, a2 - v.a2, a3 - v.a3, a4 - v.a4);
	}

	/// Returns the value of the given matrix multiplied from this.
	Matrix44 operator*(const Matrix44& v) const
	{
		return Matrix44((x1 * v.x1 + x2 * v.y1 + x3 * v.z1 + x4 * v.a1), (x1 * v.x2 + x2 * v.y2 + x3 * v.z2 + x4 * v.a2), (x1 * v.x3 + x2 * v.y3 + x3 * v.z3 + x4 * v.a3), (x1 * v.x4 + x2 * v.y4 + x3 * v.z4 + x4 * v.a4),
						(y1 * v.x1 + y2 * v.y1 + y3 * v.z1 + y4 * v.a1), (y1 * v.x2 + y2 * v.y2 + y3 * v.z2 + y4 * v.a2), (y1 * v.x3 + y2 * v.y3 + y3 * v.z3 + y4 * v.a3), (y1 * v.x4 + y2 * v.y4 + y3 * v.z4 + y4 * v.a4),
						(z1 * v.x1 + z2 * v.y1 + z3 * v.z1 + z4 * v.a1), (z1 * v.x2 + z2 * v.y2 + z3 * v.z2 + z4 * v.a2), (z1 * v.x3 + z2 * v.y3 + z3 * v.z3 + z4 * v.a3), (z1 * v.x4 + z2 * v.y4 + z3 * v.z4 + z4 * v.a4),
						(a1 * v.x1 + a2 * v.y1 + a3 * v.z1 + a4 * v.a1), (a1 * v.x2 + a2 * v.y2 + a3 * v.z2 + a4 * v.a2), (a1 * v.x3 + a2 * v.y3 + a3 * v.z3 + a4 * v.a3), (a1 * v.x4 + a2 * v.y4 + a3 * v.z4 + a4 * v.a4));
	}

	/// Adds the given matrix to this.
	void operator+=(const Matrix44& v)
	{
		x1 += v.x1; x2 += v.x2; x3 += v.x3; x4 += v.x4;
		y1 += v.y1; y2 += v.y2; y3 += v.y3; y4 += v.y4;
		z1 += v.z1; z2 += v.z2; z3 += v.z3; z4 += v.z4;
		a1 += v.a1; a2 += v.a2; a3 += v.a3; a4 += v.a4;
	}

	/// Subtracts the given matrix from this
	void operator-=(const Matrix44& v)
	{
		x1 -= v.x1; x2 -= v.x2; x3 -= v.x3; x4 -= v.x4;
		y1 -= v.y1; y2 -= v.y2; y3 -= v.y3; y4 -= v.y4;
		z1 -= v.z1; z2 -= v.z2; z3 -= v.z3; z4 -= v.z4;
		a1 -= v.a1; a2 -= v.a2; a3 -= v.a3; a4 -= v.a4;
	}

	/// Multiplies this matrix by the given matrix
	void operator*=(const Matrix44& v)
	{
		const float x1T = x1; const float x2T = x2; const float x3T = x3; const float x4T = x4;
		const float y1T = y1; const float y2T = y2; const float y3T = y3; const float y4T = y4;
		const float z1T = z1; const float z2T = z2; const float z3T = z3; const float z4T = z4;
		const float a1T = a1; const float a2T = a2; const float a3T = a3; const float a4T = a4;

		x1 = (x1T * v.x1 + x2T * v.y1 + x3T * v.z1 + x4T * v.a1);	x2 = (x1T * v.x2 + x2T * v.y2 + x3T * v.z2 + x4T * v.a2);	x3 = (x1T * v.x3 + x2T * v.y3 + x3T * v.z3 + x4T * v.a3); x4 = (x1T * v.x4 + x2T * v.y4 + x3T * v.z4 + x4T * v.a4);
		y1 = (y1T * v.x1 + y2T * v.y1 + y3T * v.z1 + y4T * v.a1);	y2 = (y1T * v.x2 + y2T * v.y2 + y3T * v.z2 + y4T * v.a2);	y3 = (y1T * v.x3 + y2T * v.y3 + y3T * v.z3 + y4T * v.a3); y4 = (y1T * v.x4 + y2T * v.y4 + y3T * v.z4 + y4T * v.a4);
		z1 = (z1T * v.x1 + z2T * v.y1 + z3T * v.z1 + z4T * v.a1);	z2 = (z1T * v.x2 + z2T * v.y2 + z3T * v.z2 + z4T * v.a2);	z3 = (z1T * v.x3 + z2T * v.y3 + z3T * v.z3 + z4T * v.a3); z4 = (z1T * v.x4 + z2T * v.y4 + z3T * v.z4 + z4T * v.a4);
		a1 = (a1T * v.x1 + a2T * v.y1 + a3T * v.z1 + a4T * v.a1);	a2 = (a1T * v.x2 + a2T * v.y2 + a3T * v.z2 + a4T * v.a2);	a3 = (a1T * v.x3 + a2T * v.y3 + a3T * v.z3 + a4T * v.a3); a4 = (a1T * v.x4 + a2T * v.y4 + a3T * v.z4 + a4T * v.a4);

	}

	inline float getDeterminant() const
	{
		return x1 * y2 * z3 * a4 - x1 * y2 * z4 * a3 - x1 * y3 * z2 * a4 + x1 * y3 * z4 * a2 + x1 * y4 * z2 * a3 - x1 * y4 * z3 * a2 - x2 * y1 * z3 * a4 + x2 * y1 * z4 * a3 + x2 * y3 * z1 * a4 - x2 * y3 * z4 * a1 - x2 * y4 * z1 * a3 + x2 * y4 * z3 * a1 + x3 * y1 * z2 * a4 - x3 * y1 * z4 * a2 - x3 * y2 * z1 * a4 + x3 * y2 * z4 * a1 + x3 * y4 * z1 * a2 - x3 * y4 * z2 * a1 - x4 * y1 * z2 * a3 + x4 * y1 * z3 * a2 + x4 * y2 * z1 * a3 - x4 * y2 * z3 * a1 - x4 * y3 * z1 * a2 + x4 * y3 * z2 * a1;
	}

	inline void transpose()
	{
		const float x1T = x1; const float x2T = x2; const float x3T = x3; const float x4T = x4;
		const float y1T = y1; const float y2T = y2; const float y3T = y3; const float y4T = y4;
		const float z1T = z1; const float z2T = z2; const float z3T = z3; const float z4T = z4;
		const float a1T = a1; const float a2T = a2; const float a3T = a3; const float a4T = a4;

		x1 = x1T; x2 = y1T; x3 = z1T; x4 = a1T;
		y1 = x2T; y2 = y2T; y3 = z2T; y4 = a2T;
		z1 = x3T; z2 = y3T; z3 = z3T; z4 = a3T;
		a1 = x4T; a2 = y4T; a3 = z4T; a4 = a4T;
	}

};

