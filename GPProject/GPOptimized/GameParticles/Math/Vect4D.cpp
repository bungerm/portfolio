//----------------------------------------------------------------------------
// Mike Bunger
// Vector
//
// Replaced original math with SIMD
//----------------------------------------------------------------------------

#include <math.h>
#include <assert.h>

#include "Vect4D.h"

// copy constructor
Vect4D::Vect4D(const Vect4D& t)
{
	this->_m = _mm_load_ps(&t._m.m128_f32[0]);
}

// constructor (__m128)
Vect4D::Vect4D(const __m128& m)
{
	this->_m = _mm_load_ps(&m.m128_f32[0]);
}

// constructor (4 floats)
Vect4D::Vect4D(const float& tx, const float& ty, const float& tz, const float& tw) :
	x(tx), y(ty), z(tz), w(tw) {}

// assignment operator
Vect4D& Vect4D::operator=(const Vect4D& t)
{
	this->_m = _mm_load_ps(&t._m.m128_f32[0]);
	return *this;
}

// normalize
void Vect4D::norm(Vect4D& out)
{
	__m128 mag = _mm_dp_ps(out._m, out._m, 0x7F);

	if (0.0f < mag.m128_f32[0])
	{
		out._m = _mm_mul_ps(_mm_rsqrt_ps(mag), this->_m);
	}
}

// addition operator
Vect4D Vect4D::operator + (const Vect4D& t) const
{
	Vect4D out;

	out._m = _mm_add_ps(this->_m, t._m);

	return out;
}

// addition equals operator
Vect4D& Vect4D::operator+=(const Vect4D& t)
{
	_m = _mm_add_ps(this->_m, t._m);
	return *this;
}

// subtraction operator
Vect4D Vect4D::operator - (const Vect4D& t) const
{
	Vect4D out;

	out._m = _mm_sub_ps(this->_m, t._m);

	return out;
}

// multiplication operator
Vect4D Vect4D::operator *(const float& scale) const
{
	Vect4D out;

	out._m = _mm_mul_ps(this->_m, _mm_load1_ps(&scale));

	return out;
}

// retrieve an element via index
float& Vect4D::operator[](const int& e)
{
	return this->_m.m128_f32[e];
}

// calculate cross product
void Vect4D::Cross(Vect4D& vin, Vect4D& vout) const
{
	vout.x = (y*vin.z - z * vin.y);
	vout.y = (z*vin.x - x * vin.z);
	vout.z = (x*vin.y - y * vin.x);
	vout.w = 1.0f;

	// tried SIMD, performance loss, seemed overly complicated

	//__m128 tmp0 = _mm_shuffle_ps(vout._m, vout._m, _MM_SHUFFLE(3, 0, 2, 1));
	//__m128 tmp1 = _mm_shuffle_ps(vin._m, vin._m, _MM_SHUFFLE(3, 0, 2, 1));
	//tmp0 = _mm_mul_ps(tmp0, vin._m);
	//tmp1 = _mm_mul_ps(tmp1, vout._m);
	//__m128 tmp2 = _mm_sub_ps(tmp0, tmp1);
	//vout._m = _mm_load_ps(&_mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 0, 2, 1)).m128_f32[0]);
}

// setter
void Vect4D::set(const float& tx, const float& ty, const float& tz, const float& tw)
{
	this->_m = _mm_set_ps(tx, ty, tz, tw);
}

// End of file
