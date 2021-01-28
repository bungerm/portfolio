//----------------------------------------------------------------------------
// Mike Bunger
// Vector header
//
// 4 dimensional Vect4D class
//----------------------------------------------------------------------------

#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"
// SIMD includes
#include <xmmintrin.h>
#include <smmintrin.h>


// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;

	// big four
	Vect4D() = default;
	Vect4D(const Vect4D&);
	Vect4D& operator = (const Vect4D&);
	~Vect4D() = default;

	// constructor (floats)
	Vect4D(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);

	// constructor (__m128)
	Vect4D(const __m128& m);

	// normalize
	void norm(Vect4D &out);
	// setter
	void set(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);

	// operators
	Vect4D operator + (const Vect4D& t) const;
	Vect4D operator - (const Vect4D& t) const;
	Vect4D operator * (const float& scale) const;
	Vect4D& operator += (const Vect4D& t);

	// cross product
	void Cross(Vect4D &vin, Vect4D &vout) const;

	// index accessor
	float& operator[](const int& e);

public:

	// the union struct trick
	// allows polymorphism
	// and performance gains? unsure
	union
	{
		__m128	_m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#endif  

// End of file

