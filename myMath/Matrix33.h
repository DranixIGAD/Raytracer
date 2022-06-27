#pragma once


class Matrix33
{
public:
	/// row x, column 1. */		/// row x, column 2. */		/// row x, column 3. */
	float x1;					float x2;					float x3;

	/// row y, column 1. */		/// row y, column 2. */		/// row y, column 3. */
	float y1;					float y2;					float y3;
			
	/// row z, column 1. */		/// row z, column 2. */		/// row z, column 3. */
	float z1;					float z2;					float z3;			

public:
	/// Default 0 constructor
	Matrix33() : x1(0), x2(0), x3(0), 
				 y1(0), y2(0), y3(0), 
				 z1(0), z2(0), z3(0) { }

	/// Constructor from components
	Matrix33(float x1, float x2, float x3, 
			 float y1, float y2, float y3, 
			 float z1, float z2, float z3) 
			  : x1(x1), x2(x2), x3(x3), 
				y1(y1), y2(y2), y3(y3), 
				z1(z1), z2(z2), z3(z3) {}

	/// Constructor from component
	explicit Matrix33(float v) : x1(v), x2(v), x3(v), 
								 y1(v), y2(v), y3(v), 
								 z1(v), z2(v), z3(v) {}

	// Constructor for identity
	Matrix33(char mark_for_identity) :	x1(1), x2(0), x3(0),
										y1(0), y2(1), y3(0),
										z1(0), z2(0), z3(1) {}

	/// Returns the value of the given matrix added to this.
	Matrix33 operator+(const Matrix33& v) const
	{
		return Matrix33(x1 + v.x1, x2 + v.x2, x3 + v.x3,
						y1 + v.y1, y2 + v.y2, y3 + v.y3,
						z1 + v.z1, z2 + v.z2, z3 + v.z3);
	}

	/// Returns the value of the given matrix subtracted from this.
	Matrix33 operator-(const Matrix33& v) const
	{
		return Matrix33(x1 - v.x1, x2 - v.x2, x3 - v.x3,
						y1 - v.y1, y2 - v.y2, y3 - v.y3,
						z1 - v.z1, z2 - v.z2, z3 - v.z3);
	}

	/// Returns the value of the given matrix multiplied from this.
	Matrix33 operator*(const Matrix33& v) const
	{
		return Matrix33((x1 * v.x1 + x2 * v.y1 + x3 * v.z1), (x1 * v.x2 + x2 * v.y2 + x3 * v.z2), (x1 * v.x3 + x2 * v.y3 + x3 * v.z3),
						(y1 * v.x1 + y2 * v.y1 + y3 * v.z1), (y1 * v.x2 + y2 * v.y2 + y3 * v.z2), (y1 * v.x3 + y2 * v.y3 + y3 * v.z3),
						(z1 * v.x1 + z2 * v.y1 + z3 * v.z1), (z1 * v.x2 + z2 * v.y2 + z3 * v.z2), (z1 * v.x3 + z2 * v.y3 + z3 * v.z3));
	}

	/// Adds the given matrix to this.
	void operator+=(const Matrix33& v)
	{
		x1 += v.x1; x2 += v.x2; x3 += v.x3;
		y1 += v.y1; y2 += v.y2; y3 += v.y3;
		z1 += v.z1; z2 += v.z2; z3 += v.z3;
	}

	/// Subtracts the given matrix from this
	void operator-=(const Matrix33& v)
	{
		x1 -= v.x1; x2 -= v.x2; x3 -= v.x3;
		y1 -= v.y1; y2 -= v.y2; y3 -= v.y3;
		z1 -= v.z1; z2 -= v.z2; z3 -= v.z3;
	}

	/// Multiplies this matrix by the given matrix
	void operator*=(const Matrix33& v)
	{
		float x1T, x2T, x3T, y1T, y2T, y3T, z1T, z2T, z3T;

		x1T = x1; x2T = x2; x3T = x3;
		y1T = y1; y2T = y2; y3T = y3;
		z1T = z1; z2T = z2; z3T = z3;

		x1 = (x1T * v.x1 + x2T * v.y1 + x3T * v.z1);	x2 = (x1T * v.x2 + x2T * v.y2 + x3T * v.z2);	x3 = (x1T * v.x3 + x2T * v.y3 + x3T * v.z3);
		y1 = (y1T * v.x1 + y2T * v.y1 + y3T * v.z1);	y2 = (y1T * v.x2 + y2T * v.y2 + y3T * v.z2);	y3 = (y1T * v.x3 + y2T * v.y3 + y3T * v.z3);
		z1 = (z1T * v.x1 + z2T * v.y1 + z3T * v.z1);	z2 = (z1T * v.x2 + z2T * v.y2 + z3T * v.z2);	z3 = (z1T * v.x3 + z2T * v.y3 + z3T * v.z3);

	}

	inline float getDeterminant()
	{
		return (x1 * ((y2 * z3) - (y3 * z2)) - x2 * ((y1 * z3) - (y3 * z1)) + x3 * ((y1 * z2) - (y2 * z1)));
	}

	inline void transpose()
	{
		float x1T, x2T, x3T, y1T, y2T, y3T, z1T, z2T, z3T;

		x1T = x1; x2T = x2; x3T = x3;
		y1T = y1; y2T = y2; y3T = y3;
		z1T = z1; z2T = z2; z3T = z3;

		x1 = x1T; x2 = y1T; x3 = z1T;
		y1 = x2T; y2 = y2T; y3 = z2T;
		z1 = x3T; z2 = y3T; z3 = z3T;
	}
};