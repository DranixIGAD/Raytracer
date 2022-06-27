#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../myMath/Vector2.h"
#include "../myMath/Vector3.h"
#include "../myMath/Matrix33.h"
#include "../myMath/Matrix44.h"

#include "../Application/Ray.cpp"
#include "../Application/Sphere.cpp"
#include "../Application/Box.cpp"
#include "../Application/Plane.cpp"
#include "../Application/Surface.cpp"


TEST_CASE("Testing Vector2 functionality")
{
	SUBCASE("Construction")
	{
		/// Default constructor
		const Vector2 v0;
		CHECK(v0.x == 0.0f);
		CHECK(v0.y == 0.0f);

		/// Constructor from components
		const Vector2 v1(9.1f, -3.0f);
		CHECK(v1.x == 9.1f);
		CHECK(v1.y == -3.0f);

		/// Constructor from component
		const Vector2 v2(4.3f);
		CHECK(v2.x == 4.3f);
		CHECK(v2.y == 4.3f);

		/// Copy construction
		Vector2 v3(v2);
		CHECK(v2.x == v3.x);
		CHECK(v2.y == v3.y);

		/// Zero all the components of the vector
		v3.Clear();
		CHECK(v0.x == v3.x);
		CHECK(v0.y == v3.y);
	}

	SUBCASE("Mathematical operators")
	{
		const Vector2 v0(2.3f, 3.7f);
		const Vector2 v1(-6.6f, 1.2f);
		const float factor = 3.5f;
		Vector2 v2;

		/// Returns the value of the given vector added to this.
		v2 = v0 + v1;
		CHECK(v2.x == v0.x + v1.x);
		CHECK(v2.y == v0.y + v1.y);

		/// Returns the value of the given vector subtracted from this.
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y);

		SUBCASE("Vector and scalar multiplication.")
		{
			/// Returns a copy of this vector scaled the given value.
			v2 = v1 * factor;
			CHECK(v2.x == v1.x * factor);
			CHECK(v2.y == v1.y * factor);

			/// Returns a new vector being the result of a float value multiplied on right hand side with a vector
			v2 = factor * v0;
			CHECK(v2.x == v0.x * factor);
			CHECK(v2.y == v0.y * factor);
		}

		/// Returns a copy of this vector scaled the given value.
		v2 = v0 / factor;
		CHECK(v2.x == v0.x / factor);
		CHECK(v2.y == v0.y / factor);

		SUBCASE("Dot product calculation.")
		{
			/// Calculates and returns the dot product of this vector with the given vector.
			const float dot = v0 * v1;
			CHECK(-10.74f == dot);

			const float calculatedDot = v0.Dot(v1);
			CHECK(dot == calculatedDot);
		}

		/// Adds the given vector to this.
		SUBCASE("Adds the given vector to this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 += v0;
			CHECK(v3.x == v0.x + 3.0f);
			CHECK(v3.y == v0.y + -4.0f);
		}

		/// Subtracts the given vector from this
		SUBCASE("Subtracts the given vector from this.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 -= v1;
			CHECK(v3.x == 3.0f - v1.x);
			CHECK(v3.y == -4.0f - v1.y);
		}

		/// Multiplies this vector by the given scalar
		SUBCASE("Multiplies this vector by the given scalar.")
		{
			Vector2 v3(3.0f, -4.0f);
			v3 *= factor;
			CHECK(v3.x == 3.0f * factor);
			CHECK(v3.y == -4.0f * factor);
		}
	}

	SUBCASE("Logical operators")
	{
		/// Checks if the two vectors have identical components
		const float x = 2.93f;
		const float y = 4.39f;
		Vector2 v0(x, y);
		Vector2 v1(x, y);
		Vector2 v2(y, x);

		CHECK(v0 == v1);
		CHECK(v2 != v0);
	}

	SUBCASE("Length operations")
	{
		Vector2 v0(3.0f, 4.0f);

		/// Gets the magnitude of this vector
		const float vectorLenght = v0.Magnitude();
		CHECK(vectorLenght == 5.0f);

		/// Gets the squared magnitude of this vector
		const float vectorLenghtSq = v0.SquareMagnitude();
		CHECK(vectorLenghtSq == 25.0f);

		/// Limits the size of the vector to the given maximum
		v0.Trim(2.5f);
		CHECK(2.5f == v0.Magnitude());

		/// Returns the normalised version of a vector
		Vector2 v1 = v0;
		Vector2 v2 = v1.Unit();
		CHECK(v1 == v0);
		CHECK(1.0f == v2.Magnitude());
			
		/// Turns a non-zero vector into a vector of unit length
		v0.Normalize();
		CHECK(1.0f == v0.Magnitude());
	}
}

TEST_CASE("Testing Vector3 functionality")
{
	SUBCASE("Construction")
	{
		/// Default constructor
		const Vector3 v0;
		CHECK(v0.x == 0.0f);
		CHECK(v0.y == 0.0f);
		CHECK(v0.z == 0.0f);

		/// Constructor from components
		const Vector3 v1(9.1f, -3.0f, 2.7f);
		CHECK(v1.x == 9.1f);
		CHECK(v1.y == -3.0f);
		CHECK(v1.z == 2.7f);

		/// Constructor from components
		const Vector3 v2(4.3f);
		CHECK(v2.x == 4.3f);
		CHECK(v2.y == 4.3f);
		CHECK(v2.z == 4.3f);

		/// Copy construction
		Vector3 v3(v2);
		CHECK(v2.x == v3.x);
		CHECK(v2.y == v3.y);
		CHECK(v2.z == v3.z);

		/// Zero all the components of the vector
		v3.Clear();
		CHECK(v0.x == v3.x);
		CHECK(v0.y == v3.y);
		CHECK(v0.z == v3.z);

	}

	SUBCASE("Mathematical operators")
	{
		/// Returns a copy of this vector scaled the inverse of the value f
		/// Returns the negative this vector f

		const Vector3 v0(2.3f, 3.7f, -1.4f);
		const Vector3 v1(-6.6f, 1.2f, 4.1f);
		const float factor = 3.5f;
		Vector3 v2;

		/// Returns the value of the given vector added to this.
		v2 = v0 + v1;
		CHECK(v2.x == v0.x + v1.x);
		CHECK(v2.y == v0.y + v1.y);
		CHECK(v2.z == v0.z + v1.z);

		/// Returns the value of the given vector subtracted from this.
		v2 = v0 - v1;
		CHECK(v2.x == v0.x - v1.x);
		CHECK(v2.y == v0.y - v1.y);
		CHECK(v2.z == v0.z - v1.z);

		/// Returns a copy of this vector scaled the given value.
		v2 = v1 * factor;
		CHECK(v2.x == v1.x * factor);
		CHECK(v2.y == v1.y * factor);
		CHECK(v2.z == v1.z * factor);

		/// Adds the given vector to this.
		SUBCASE("Adds the given vector to this.")
		{
			Vector3 v3(3.0f, -4.0f, 2.0f);
			v3 += v0;
			CHECK(v3.x == v0.x + 3.0f);
			CHECK(v3.y == v0.y + -4.0f);
			CHECK(v3.z == v0.z + 2.0f);
		}

		/// Subtracts the given vector from this
		SUBCASE("Subtracts the given vector from this.")
		{
			Vector3 v3(3.0f, -4.0f, 2.0f);
			v3 -= v1;
			CHECK(v3.x == 3.0f - v1.x);
			CHECK(v3.y == -4.0f - v1.y);
			CHECK(v3.z == 2.0f - v1.z);
		}

		/// Multiplies this vector by the given scalar
		SUBCASE("Multiplies this vector by the given scalar.")
		{
			Vector3 v3(3.0f, -4.0f, 2.0f);
			v3 *= factor;
			CHECK(v3.x == 3.0f * factor);
			CHECK(v3.y == -4.0f * factor);
			CHECK(v3.z == 2.0f * factor);
		}


		SUBCASE("Vector products")
		{
			/// Calculates and returns the dot product of this with the given vector
			const float dot = v0 * v1;
			CHECK(-16.48f == dot);

			const float calculatedDot = v0.Dot(v1);
			CHECK(dot == calculatedDot);

			/// Calculates and returns the cross product of this vector with the given vector
			const Vector3 crossProduct = v0.Cross(v1);
			CHECK(crossProduct.x == (v0.y * v1.z) - (v0.z * v1.y));
			CHECK(crossProduct.y == (v0.z * v1.x) - (v0.x * v1.z));
			CHECK(crossProduct.z == (v0.x * v1.y) - (v0.y * v1.x));

		}

		SUBCASE("Length operations")
		{
			Vector3 v0(2.0f, 3.0f, 4.0f);

			/// Gets the magnitude of this vector
			const float vectorLenght = v0.Magnitude();
			CHECK(vectorLenght == 5.38516474f);

			/// Gets the squared magnitude of this vector
			const float vectorLenghtSq = v0.SquareMagnitude();
			CHECK(vectorLenghtSq == 29.0f);

			/// Limits the size of the vector to the given maximum
			v0.Trim(2.5f);
			CHECK(2.5f == v0.Magnitude());

			/// Returns the normalised version of a vector
			Vector3 v1 = v0;
			Vector3 v2 = v1.Unit();
			CHECK(v1 == v0);
			CHECK(1.0f == v2.Magnitude());

			/// Turns a non-zero vector into a vector of unit length
			v0.Normalize();
			CHECK(1.0f == v0.Magnitude());

		}
	}

	SUBCASE("Logical operations")
	{
		/// Checks if the two vectors have identical components
		/// Checks if the two vectors have non-identical components

		/// Checks if the two vectors have identical components
		const float x = 2.93f;
		const float y = 4.39f;
		const float z = 3.52f;
		Vector3 v0(x, y, z);
		Vector3 v1(x, y, z);
		Vector3 v2(y, z, x);

		CHECK(v0 == v1);
		CHECK(v2 != v0);

	}
}

TEST_CASE("Testing Matrix33 functionality")
{
	SUBCASE("Construction")
	{
		/// Default constructor
		const Matrix33 m0;
		CHECK(m0.x1 == 0);	CHECK(m0.x2 == 0);	CHECK(m0.x3 == 0);
		CHECK(m0.y1 == 0);	CHECK(m0.y2 == 0);	CHECK(m0.y3 == 0);
		CHECK(m0.z1 == 0);	CHECK(m0.z2 == 0);	CHECK(m0.z3 == 0);

		/// Constructor from components
		const Matrix33 m1(9.1f, -3.0f, 2.7f,
						  4.6f, 3.2f, -1.3f,
						  2.5f, -6.9f, 1.4f);
		CHECK(m1.x1 == 9.1f);	CHECK(m1.x2 == -3.0f);	CHECK(m1.x3 == 2.7f);
		CHECK(m1.y1 == 4.6f);	CHECK(m1.y2 == 3.2f);	CHECK(m1.y3 == -1.3f);
		CHECK(m1.z1 == 2.5f);	CHECK(m1.z2 == -6.9f);	CHECK(m1.z3 == 1.4f);

		/// Constructor from components
		const Matrix33 m2(4.3f);
		CHECK(m2.x1 == 4.3f);	CHECK(m2.x2 == 4.3f);	CHECK(m2.x3 == 4.3f);
		CHECK(m2.y1 == 4.3f);	CHECK(m2.y2 == 4.3f);	CHECK(m2.y3 == 4.3f);
		CHECK(m2.z1 == 4.3f);	CHECK(m2.z2 == 4.3f);	CHECK(m2.z3 == 4.3f);

		/// Copy construction
		Matrix33 m3(m2);
		CHECK(m2.x1 == m3.x1); CHECK(m2.x2 == m3.x2); CHECK(m2.x3 == m3.x3);
		CHECK(m2.y1 == m3.y1); CHECK(m2.y2 == m3.y2); CHECK(m2.y3 == m3.y3);
		CHECK(m2.z1 == m3.z1); CHECK(m2.z2 == m3.z2); CHECK(m2.z3 == m3.z3);

		/// Construct a new identity matrix
		const Matrix33 mId('x');
		CHECK(mId.x1 == 1);	CHECK(mId.x2 == 0);	CHECK(mId.x3 == 0);
		CHECK(mId.y1 == 0);	CHECK(mId.y2 == 1);	CHECK(mId.y3 == 0);
		CHECK(mId.z1 == 0);	CHECK(mId.z2 == 0);	CHECK(mId.z3 == 1);
	}

	SUBCASE("Mathematical operations")
	{
		Matrix33 m0(-1, 4, -6,
					 8, 5, 16,
					 2, 8,  5);

		Matrix33 m1(12, 7, 6,
					 8, 0, 5,
					 3, 2, 4);

		Matrix33 m3(m0);
		Matrix33 m4(m0);
		Matrix33 m5(m0);

		/// Matrix addition
		Matrix33 m2 = m0 + m1;
		CHECK(m2.x1 == 11);	CHECK(m2.x2 == 11);	CHECK(m2.x3 ==  0);
		CHECK(m2.y1 == 16);	CHECK(m2.y2 ==  5);	CHECK(m2.y3 == 21);
		CHECK(m2.z1 ==  5);	CHECK(m2.z2 == 10); CHECK(m2.z3 ==  9);

		m3 += m1;
		CHECK(m3.x1 == 11);	CHECK(m3.x2 == 11);	CHECK(m3.x3 == 0);
		CHECK(m3.y1 == 16);	CHECK(m3.y2 == 5);	CHECK(m3.y3 == 21);
		CHECK(m3.z1 == 5);	CHECK(m3.z2 == 10); CHECK(m3.z3 == 9);

		/// Matrix subtraction
		m2 = m0 - m1;
		CHECK(m2.x1 == -13); CHECK(m2.x2 == -3); CHECK(m2.x3 == -12);
		CHECK(m2.y1 ==   0); CHECK(m2.y2 ==  5); CHECK(m2.y3 ==  11);
		CHECK(m2.z1 ==  -1); CHECK(m2.z2 ==  6); CHECK(m2.z3 ==   1);

		m4 -= m1;
		CHECK(m4.x1 == -13); CHECK(m4.x2 == -3); CHECK(m4.x3 == -12);
		CHECK(m4.y1 ==   0); CHECK(m4.y2 ==  5); CHECK(m4.y3 ==  11);
		CHECK(m4.z1 ==  -1); CHECK(m4.z2 ==  6); CHECK(m4.z3 ==   1);

		/// Matrix multiplication
		m2 = m0 * m1;
		CHECK(m2.x1 == 2);		CHECK(m2.x2 == -19);	CHECK(m2.x3 == -10);
		CHECK(m2.y1 == 184);	CHECK(m2.y2 == 88);		CHECK(m2.y3 == 137);
		CHECK(m2.z1 == 103);	CHECK(m2.z2 == 24);		CHECK(m2.z3 == 72);

		m5 *= m1;
		CHECK(m5.x1 ==   2);	CHECK(m5.x2 == -19);	CHECK(m5.x3 == -10);
		CHECK(m5.y1 == 184);	CHECK(m5.y2 ==  88);	CHECK(m5.y3 == 137);
		CHECK(m5.z1 == 103);	CHECK(m5.z2 ==  24);	CHECK(m5.z3 ==  72);


		SUBCASE("Inversion")
		{
			/// Get the determinant of this matrix
			Matrix33 m0(6, 1, 1,
						4,-2, 5,
						2, 8, 7);
			float m0D = m0.getDeterminant();
			CHECK(m0D == -306.0f);

			/// Inverts this matrix

			/// Transposes this matrix
			Matrix33 m2(3, 1, 9,
						2, 7, 1,
						9, 4, 4);
			m2.transpose();
			CHECK(m2.x1 == 3);	CHECK(m2.x2 == 2);	CHECK(m2.x3 == 9);
			CHECK(m2.y1 == 1);	CHECK(m2.y2 == 7);	CHECK(m2.y3 == 4);
			CHECK(m2.z1 == 9);	CHECK(m2.z2 == 1);	CHECK(m2.z3 == 4);
		}
	}

}

TEST_CASE("Testing Matrix44 construction") // matrix is split up because of "too many bytes for one testcase" warning
{
	/// Default constructor
	Matrix44* m0 = new Matrix44();
	CHECK(m0->x1 == 0);	CHECK(m0->x2 == 0);	CHECK(m0->x3 == 0); CHECK(m0->x4 == 0);
	CHECK(m0->y1 == 0);	CHECK(m0->y2 == 0);	CHECK(m0->y3 == 0); CHECK(m0->y4 == 0);
	CHECK(m0->z1 == 0);	CHECK(m0->z2 == 0);	CHECK(m0->z3 == 0); CHECK(m0->z4 == 0);
	CHECK(m0->a1 == 0);	CHECK(m0->a2 == 0);	CHECK(m0->a3 == 0); CHECK(m0->a4 == 0);
	delete m0;

	/// Constructor from components
	Matrix44* m1 = new Matrix44(9.1f, -3.0f, 2.7f, 2.4f,
		4.6f, 3.2f, -1.3f, 2.5f,
		2.5f, -6.9f, 1.4f, 6.1f,
		4.0f, -2.7f, 3.5f, 1.9f);
	CHECK(m1->x1 == 9.1f);	CHECK(m1->x2 == -3.0f);	CHECK(m1->x3 == 2.7f);	CHECK(m1->x4 == 2.4f);
	CHECK(m1->y1 == 4.6f);	CHECK(m1->y2 == 3.2f);	CHECK(m1->y3 == -1.3f);	CHECK(m1->y4 == 2.5f);
	CHECK(m1->z1 == 2.5f);	CHECK(m1->z2 == -6.9f);	CHECK(m1->z3 == 1.4f);	CHECK(m1->z4 == 6.1f);
	CHECK(m1->a1 == 4.0f);	CHECK(m1->a2 == -2.7f);	CHECK(m1->a3 == 3.5f);	CHECK(m1->a4 == 1.9f);
	delete m1;

	/// Constructor from components
	Matrix44* m2 = new Matrix44(4.3f);
	CHECK(m2->x1 == 4.3f);	CHECK(m2->x2 == 4.3f);	CHECK(m2->x3 == 4.3f);	CHECK(m2->x4 == 4.3f);
	CHECK(m2->y1 == 4.3f);	CHECK(m2->y2 == 4.3f);	CHECK(m2->y3 == 4.3f);	CHECK(m2->y4 == 4.3f);
	CHECK(m2->z1 == 4.3f);	CHECK(m2->z2 == 4.3f);	CHECK(m2->z3 == 4.3f);	CHECK(m2->x4 == 4.3f);
	CHECK(m2->a1 == 4.3f);	CHECK(m2->a2 == 4.3f);	CHECK(m2->a3 == 4.3f);	CHECK(m2->a4 == 4.3f);

	/// Copy construction
	Matrix44* m3 = new Matrix44(*m2);
	CHECK(m2->x1 == m3->x1);	CHECK(m2->x2 == m3->x2);	CHECK(m2->x3 == m3->x3);	CHECK(m2->x4 == m3->x4);
	CHECK(m2->y1 == m3->y1);	CHECK(m2->y2 == m3->y2);	CHECK(m2->y3 == m3->y3);	CHECK(m2->y4 == m3->y4);
	CHECK(m2->z1 == m3->z1);	CHECK(m2->z2 == m3->z2);	CHECK(m2->z3 == m3->z3);	CHECK(m2->z4 == m3->z4);
	CHECK(m2->a1 == m3->a1);	CHECK(m2->a2 == m3->a2);	CHECK(m2->a3 == m3->a3);	CHECK(m2->a4 == m3->a4);
	delete m2;
	delete m3;
	
	/// Construct a new identity matrix
	Matrix44* mId = new Matrix44('x');
	CHECK(mId->x1 == 1);	CHECK(mId->x2 == 0);	CHECK(mId->x3 == 0); CHECK(mId->x4 == 0);
	CHECK(mId->y1 == 0);	CHECK(mId->y2 == 1);	CHECK(mId->y3 == 0); CHECK(mId->y4 == 0);
	CHECK(mId->z1 == 0);	CHECK(mId->z2 == 0);	CHECK(mId->z3 == 1); CHECK(mId->z4 == 0);
	CHECK(mId->a1 == 0);	CHECK(mId->a2 == 0);	CHECK(mId->a3 == 0); CHECK(mId->a4 == 1);
	delete mId;
	
	/// Creates a look at matrix, usually a view matrix 
	Matrix44* lookat = new Matrix44(Vector3(0.0f, 7.0f, 1.0f), Vector3(0.0f, 0.0f, -9.0f));
	CHECK(lookat->x1 == 0.819231927f);	CHECK(lookat->x2 == 0.0f);			CHECK(lookat->x3 == 0.0f);			CHECK(lookat->x4 == 0.0f);
	CHECK(lookat->y1 == 0.0f);			CHECK(lookat->y2 == 0.671140969f);	CHECK(lookat->y3 == 0.573462367f);	CHECK(lookat->y4 == 0.0f);
	CHECK(lookat->z1 == 0.0f);			CHECK(lookat->z2 == -0.469798684f);	CHECK(lookat->z3 == 0.819231927f);	CHECK(lookat->z4 == 0.0f);
	CHECK(lookat->a1 == 0.0f);			CHECK(lookat->a2 == 0.0f);			CHECK(lookat->a3 == 0.0f);			CHECK(lookat->a4 == 1.0f);
	delete lookat;
	
}

TEST_CASE("make matrices")
{
	/// Creates a translation matrix
	Matrix44* trans = new Matrix44();
	trans->makeTranslation(2.0f, 7.0f, 9.0f);
	CHECK(trans->x1 == 1);	CHECK(trans->x2 == 0);	CHECK(trans->x3 == 0); CHECK(trans->x4 == 2.0f);
	CHECK(trans->y1 == 0);	CHECK(trans->y2 == 1);	CHECK(trans->y3 == 0); CHECK(trans->y4 == 7.0f);
	CHECK(trans->z1 == 0);	CHECK(trans->z2 == 0);	CHECK(trans->z3 == 1); CHECK(trans->z4 == 9.0f);
	CHECK(trans->a1 == 0);	CHECK(trans->a2 == 0);	CHECK(trans->a3 == 0); CHECK(trans->a4 == 1);
	delete trans;

	/// Creates a rotation matrix around the x axis
	Matrix44* rotX = new Matrix44();
	rotX->makeXRotation(2.0f);
	CHECK(rotX->x1 == 1);	CHECK(rotX->x2 == 0);		CHECK(rotX->x3 == 0);		 CHECK(rotX->x4 == 0);
	CHECK(rotX->y1 == 0);	CHECK(rotX->y2 == cosf(2)); CHECK(rotX->y3 == -sinf(2)); CHECK(rotX->y4 == 0);
	CHECK(rotX->z1 == 0);	CHECK(rotX->z2 == sinf(2)); CHECK(rotX->z3 == cosf(2));	 CHECK(rotX->z4 == 0);
	CHECK(rotX->a1 == 0);	CHECK(rotX->a2 == 0);		CHECK(rotX->a3 == 0);		 CHECK(rotX->a4 == 1);
	delete rotX;

	/// Creates a rotation matrix around the y axis
	Matrix44* rotY = new Matrix44();
	rotY->makeYRotation(3);
	CHECK(rotY->x1 == cosf(3));	CHECK(rotY->x2 == 0);	CHECK(rotY->x3 == sinf(3)); CHECK(rotY->x4 == 0);
	CHECK(rotY->y1 == 0);		CHECK(rotY->y2 == 1);	CHECK(rotY->y3 == 0);		CHECK(rotY->y4 == 0);
	CHECK(rotY->z1 == -sinf(3)); CHECK(rotY->z2 == 0);	CHECK(rotY->z3 == cosf(3));	CHECK(rotY->z4 == 0);
	CHECK(rotY->a1 == 0);		CHECK(rotY->a2 == 0);	CHECK(rotY->a3 == 0);		CHECK(rotY->a4 == 1);
	delete rotY;

	/// Creates a rotation matrix around the z axis
	Matrix44* rotZ = new Matrix44();
	rotZ->makeZRotation(4);
	CHECK(rotZ->x1 == cosf(4));	CHECK(rotZ->x2 == -sinf(4)); CHECK(rotZ->x3 == 0);	CHECK(rotZ->x4 == 0);
	CHECK(rotZ->y1 == sinf(4));	CHECK(rotZ->y2 == cosf(4));	CHECK(rotZ->y3 == 0);	CHECK(rotZ->y4 == 0);
	CHECK(rotZ->z1 == 0);		CHECK(rotZ->z2 == 0);		CHECK(rotZ->z3 == 1);	CHECK(rotZ->z4 == 0);
	CHECK(rotZ->a1 == 0);		CHECK(rotZ->a2 == 0);		CHECK(rotZ->a3 == 0);	CHECK(rotZ->a4 == 1);
	delete rotZ;
}


TEST_CASE("Testing Matrix44 functionality")
{	
	SUBCASE("Mathematical operations")
	{
		Matrix44 m0(-1, 4, -6, 4,
					 8, 5, 16, 1,
					 2, 8,  5, 1,
					 9, 2,  5, 7);

		Matrix44 m1(12, 7, 6, 3,
					 8, 0, 5, 8,
					 3, 2, 4, 5,
					 3, 7, 1, 4);

		Matrix44 m2(m0);
		Matrix44 m3(m0);
		Matrix44 m4(m0);
		Matrix44 m5(m0);

		/// Matrix addition
		m2 = m0 + m1;
		CHECK(m2.x1 == 11);	CHECK(m2.x2 == 11);	CHECK(m2.x3 == 0);  CHECK(m2.x4 == 7);
		CHECK(m2.y1 == 16);	CHECK(m2.y2 == 5);	CHECK(m2.y3 == 21); CHECK(m2.y4 == 9);
		CHECK(m2.z1 == 5);	CHECK(m2.z2 == 10); CHECK(m2.z3 == 9);  CHECK(m2.z4 == 6);
		CHECK(m2.a1 == 12); CHECK(m2.a2 == 9);  CHECK(m2.a3 == 6);  CHECK(m2.a4 == 11);

		m3 += m1;
		CHECK(m3.x1 == 11);	CHECK(m3.x2 == 11);	CHECK(m3.x3 == 0);  CHECK(m3.x4 == 7);
		CHECK(m3.y1 == 16);	CHECK(m3.y2 == 5);	CHECK(m3.y3 == 21); CHECK(m3.y4 == 9);
		CHECK(m3.z1 == 5);	CHECK(m3.z2 == 10); CHECK(m3.z3 == 9);  CHECK(m3.z4 == 6);
		CHECK(m3.a1 == 12); CHECK(m3.a2 == 9);  CHECK(m3.a3 == 6);  CHECK(m3.a4 == 11);

		/// Matrix subtraction
		m2 = m0 - m1;
		CHECK(m2.x1 == -13);CHECK(m2.x2 == -3);	CHECK(m2.x3 == -12);CHECK(m2.x4 == 1);
		CHECK(m2.y1 == 0);	CHECK(m2.y2 == 5);	CHECK(m2.y3 == 11); CHECK(m2.y4 == -7);
		CHECK(m2.z1 == -1);	CHECK(m2.z2 == 6);  CHECK(m2.z3 == 1);  CHECK(m2.z4 == -4);
		CHECK(m2.a1 == 6);  CHECK(m2.a2 == -5); CHECK(m2.a3 == 4);  CHECK(m2.a4 == 3);

		m4 -= m1;
		CHECK(m4.x1 == -13);CHECK(m4.x2 == -3);	CHECK(m4.x3 == -12);CHECK(m4.x4 == 1);
		CHECK(m4.y1 == 0);	CHECK(m4.y2 == 5);	CHECK(m4.y3 == 11); CHECK(m4.y4 == -7);
		CHECK(m4.z1 == -1);	CHECK(m4.z2 == 6);  CHECK(m4.z3 == 1);  CHECK(m4.z4 == -4);
		CHECK(m4.a1 == 6);  CHECK(m4.a2 == -5); CHECK(m4.a3 == 4);  CHECK(m4.a4 == 3);

		/// Matrix multiplication
		m2 = m0 * m1;
		CHECK(m2.x1 == 14);  CHECK(m2.x2 == 9);	 CHECK(m2.x3 == -6); CHECK(m2.x4 == 15);
		CHECK(m2.y1 == 187); CHECK(m2.y2 == 95); CHECK(m2.y3 == 138);CHECK(m2.y4 == 148);
		CHECK(m2.z1 == 106); CHECK(m2.z2 == 31); CHECK(m2.z3 == 73); CHECK(m2.z4 == 99);
		CHECK(m2.a1 == 160); CHECK(m2.a2 == 122);CHECK(m2.a3 == 91); CHECK(m2.a4 == 96);

		m5 *= m1;
		CHECK(m5.x1 == 14);  CHECK(m5.x2 == 9);	 CHECK(m5.x3 == -6); CHECK(m5.x4 == 15);
		CHECK(m5.y1 == 187); CHECK(m5.y2 == 95); CHECK(m5.y3 == 138);CHECK(m5.y4 == 148);
		CHECK(m5.z1 == 106); CHECK(m5.z2 == 31); CHECK(m5.z3 == 73); CHECK(m5.z4 == 99);
		CHECK(m5.a1 == 160); CHECK(m5.a2 == 122);CHECK(m5.a3 == 91); CHECK(m5.a4 == 96);


		SUBCASE("Inversion")
		{
			/// Get the determinant of this matrix
			Matrix44 m0(-1, 4, -6, 4,
						 8, 5, 16, 1,
						 2, 8,  5, 1,
						 9, 2,  5, 7);
			float m0D = m0.getDeterminant();
			CHECK(m0D == 842);
			
			/// Inverts this matrix
			
			/// Transposes this matrix
			Matrix44 m1(12, 7, 6, 3,
						 8, 0, 5, 8,
						 3, 2, 4, 5,
						 3, 7, 1, 4);
			m1.transpose();
			CHECK(m1.x1 == 12);	CHECK(m1.x2 == 8);	CHECK(m1.x3 == 3);	CHECK(m1.x4 == 3);
			CHECK(m1.y1 == 7);	CHECK(m1.y2 == 0);	CHECK(m1.y3 == 2);	CHECK(m1.y4 == 7);
			CHECK(m1.z1 == 6);	CHECK(m1.z2 == 5);	CHECK(m1.z3 == 4);	CHECK(m1.z4 == 1);
			CHECK(m1.a1 == 3);	CHECK(m1.a2 == 8);	CHECK(m1.a3 == 5);	CHECK(m1.a4 == 4);

		}
	}

	SUBCASE("Transformation")
	{
		Matrix44 trans;
		trans.makeTranslation(3.0f, -2.0f, 5.5f);
		
		/// Transform the given vector by this matrix.
		Vector3 point = trans * Vector3(2.0, 6.0f, 6.0f);
		CHECK(point.x == 5.0f);
		CHECK(point.y == 4.0f);
		CHECK(point.z == 11.5f);

		
		/// Transform the direction vector of this matrix
		Vector3 direction = trans.times(Vector3(2.0, 6.0f, 6.0f),0);
		CHECK(direction.x == 2.0f);
		CHECK(direction.y == 6.0f);
		CHECK(direction.z == 6.0f);		
	}
}

TEST_CASE("Testing Surface intersections")
{
	//Sphere* orb = new Sphere(1.0f, 2.0f, -9.0f, 1.0f);
	//Ray case1(Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 2.0f, 0.0f));
	//Ray case2(Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 5.0f, 0.0f));

	SUBCASE("Spheres")
	{
		Sphere orb(1.0f, 2.0f, -9.0f, 1.0f);
		Ray case1(Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 2.0f, 0.0f));
		Ray case2(Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 5.0f, 0.0f));

		CHECK(orb.Hit(case1));
		CHECK(!orb.Hit(case2));		
	}
	
	SUBCASE("boxes")
	{
		Box beam(2.0f, 1.0f, 1.0f);
		Ray case1(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 10.0f));
		Ray case2(Vector3(0.0f, 0.0f, -1.0f), Vector3(-3.0f, 0.0f, 0.0f));

		CHECK(beam.Hit(case1));
		CHECK(!beam.Hit(case2));
	}
	
	SUBCASE("Planes")
	{
		Plane groundPlane;
		Ray case1(Vector3(0.0f, -1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
		Ray case2(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));

		CHECK(groundPlane.Hit(case1));
		CHECK(!groundPlane.Hit(case2));
	}
}