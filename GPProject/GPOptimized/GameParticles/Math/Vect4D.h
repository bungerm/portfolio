//----------------------------------------------------------------------------
// Mike Bunger
// Vector header
//----------------------------------------------------------------------------

// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"
#include <xmmintrin.h>
#include <smmintrin.h>


// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;

	Vect4D() = default;
	Vect4D(const Vect4D&);
	Vect4D& operator = (const Vect4D&);
	~Vect4D() = default;

	Vect4D(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);

	Vect4D(const __m128& m);

	void norm(Vect4D &out);
	void set(const float& tx, const float& ty, const float& tz, const float& tw = 1.0f);

	Vect4D operator + (const Vect4D& t) const;
	Vect4D operator - (const Vect4D& t) const;
	Vect4D operator * (const float& scale) const;

	//HERE!!!/////
	Vect4D& operator += (const Vect4D& t);

	void Cross(Vect4D &vin, Vect4D &vout) const;

	float& operator[](const int& e);

public:
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

