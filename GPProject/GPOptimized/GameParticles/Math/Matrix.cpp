//----------------------------------------------------------------------------
// Mike Bunger
// Matrix
//
// Using SIMD for every operation possible
//----------------------------------------------------------------------------

#include <Math.h>
#include <assert.h>

#include "Vect4D.h"
#include "Matrix.h"

// copy constructor
Matrix::Matrix(const Matrix& t)
{
	v0._m = t.v0._m;
	v1._m = t.v1._m;
	v2._m = t.v2._m;
	v3._m = t.v3._m;
}

// assignment operator
Matrix& Matrix::operator = (const Matrix& t)
{
	this->v0._m = t.v0._m;
	this->v1._m = t.v1._m;
	this->v2._m = t.v2._m;
	this->v3._m = t.v3._m;
	return *this;
}

// set the matrix to the identity matrix
void Matrix::setIdentMatrix()
{
	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	v1._m = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

 // set the translation matrix (note: we are row major)
void Matrix::setTransMatrix(Vect4D *t)
{
	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	v1._m = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, t->_m.m128_f32[2], t->_m.m128_f32[1], t->_m.m128_f32[0]);
}

// select a row in the matrix and set it to a vector
void Matrix::set(const int& row, Vect4D *t)
{
	// initialize the rows of the matrix
	switch (row)
	{
	case MATRIX_ROW_0:
		v0._m = _mm_load_ps(&t->_m.m128_f32[0]);
		break;

	case MATRIX_ROW_1:
		v1._m = _mm_load_ps(&t->_m.m128_f32[0]);
		break;

	case MATRIX_ROW_2:
		v2._m = _mm_load_ps(&t->_m.m128_f32[0]);
		break;

	case MATRIX_ROW_3:
		v3._m = _mm_load_ps(&t->_m.m128_f32[0]);
		break;

	default:
		// should never get here, if we are here something bad has happened
		assert(0);
	}
}

// get a specified row of the matrix
void Matrix::get(const int& row, Vect4D *t)
{
	switch (row)
	{
	case MATRIX_ROW_0:
		t->_m = this->v0._m;
		break;

	case MATRIX_ROW_1:
		t->_m = this->v1._m;
		break;

	case MATRIX_ROW_2:
		t->_m = this->v2._m;
		break;

	case MATRIX_ROW_3:
		t->_m = this->v3._m;
		break;

	default:
		assert(0);
	}
}

// matrix multiplication
Matrix& Matrix::operator*(const Matrix& mb)
{
	// the matrix we will be returning
	Matrix out;

	// load the mb matrix
	__m128 copy4 = _mm_load_ps(&mb.v0.x);
	__m128 copy5 = _mm_load_ps(&mb.v1.x);
	__m128 copy6 = _mm_load_ps(&mb.v2.x);
	__m128 copy7 = _mm_load_ps(&mb.v3.x);

	// transpose the mb copy
	_MM_TRANSPOSE4_PS(copy4, copy5, copy6, copy7);

	// load our matrix's first row
	__m128 row = _mm_load_ps(&this->v0.x);

	// multiply the mb copy by our row
	__m128 t0 = _mm_mul_ps(row, copy4);
	__m128 t1 = _mm_mul_ps(row, copy5);
	__m128 t2 = _mm_mul_ps(row, copy6);
	__m128 t3 = _mm_mul_ps(row, copy7);

	// horizontal add all four rows of the row1 * mb matrix, load it as out's first row
	out.v0._m = _mm_load_ps(&_mm_hadd_ps(_mm_hadd_ps(t0, t1), _mm_hadd_ps(t2, t3)).m128_f32[0]);

	// next row (2)
	row = _mm_load_ps(&this->v1.x);

	// multiply
	t0 = _mm_mul_ps(row, copy4);
	t1 = _mm_mul_ps(row, copy5);
	t2 = _mm_mul_ps(row, copy6);
	t3 = _mm_mul_ps(row, copy7);

	// add and load
	out.v1._m = _mm_load_ps(&_mm_hadd_ps(_mm_hadd_ps(t0, t1), _mm_hadd_ps(t2, t3)).m128_f32[0]);

	// next row (3)
	row = _mm_load_ps(&this->v2.x);

	// multiply
	t0 = _mm_mul_ps(row, copy4);
	t1 = _mm_mul_ps(row, copy5);
	t2 = _mm_mul_ps(row, copy6);
	t3 = _mm_mul_ps(row, copy7);

	// add and load
	out.v2._m = _mm_load_ps(&_mm_hadd_ps(_mm_hadd_ps(t0, t1), _mm_hadd_ps(t2, t3)).m128_f32[0]);

	// next row (4)
	row = _mm_load_ps(&this->v3.x);

	// multiply
	t0 = _mm_mul_ps(row, copy4);
	t1 = _mm_mul_ps(row, copy5);
	t2 = _mm_mul_ps(row, copy6);
	t3 = _mm_mul_ps(row, copy7);

	// add and load
	out.v3._m = _mm_load_ps(&_mm_hadd_ps(_mm_hadd_ps(t0, t1), _mm_hadd_ps(t2, t3)).m128_f32[0]);

	// return the finished matrix
	return out;
}

// subtract operator
Matrix& Matrix::operator - (const Matrix& t)
{
	Matrix out;

	out.v0._m = _mm_sub_ps(v0._m, t.v0._m);
	out.v1._m = _mm_sub_ps(v1._m, t.v1._m);
	out.v2._m = _mm_sub_ps(v2._m, t.v2._m);
	out.v3._m = _mm_sub_ps(v3._m, t.v3._m);

	return out;
}

// divide equals operator
Matrix& Matrix::operator/=(float rhs)
{
	__m128 temp = _mm_set1_ps(rhs);
	this->v0._m = _mm_load_ps(&_mm_div_ps(this->v0._m, temp).m128_f32[0]);
	this->v1._m = _mm_load_ps(&_mm_div_ps(this->v1._m, temp).m128_f32[0]);
	this->v2._m = _mm_load_ps(&_mm_div_ps(this->v2._m, temp).m128_f32[0]);
	this->v3._m = _mm_load_ps(&_mm_div_ps(this->v3._m, temp).m128_f32[0]);

	return *this;
}

// calculate the determinant (may be a better way)
// math provided by keenan
float Matrix::Determinant()
{
	// WRITTEN OUT

	// A = { a,b,c,d / e,f,g,h / j,k,l,m / n,o,p,q }
	// A = { 0,1,2,3 / 4,5,6,7 / 8,9,10,11 / 12,13,14,15 }

	// det(A) = a*det( { f,g,h / k,l,m / o,p,q } )
	//			- b*det( { e,g,h / j,l,m / n,p,q } )
	//			+ c*det( { e,f,h / j,k,m / n,o,q } )
	//			- d*det( { e,f,g / j,k,l / n,o,p } )

	// det(A) = (a (f (lq - mp) - g (kq - mo) + h (kp - lo) ) )
	//			- (b (e (lq - mp) - g (jq - mn) + h (jp - ln) ) )
	//			+ (c (e (kq - mo) - f (jq - mn) + h (jo - kn) ) )
	//			- (d (e (kp - lo) - f (jp - ln) + g (jo - kn) ) )

	

	// ta = (lq - mp)
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);
	
	// det(A) = (a (f*ta  - g*tb + h*tc) )
	//			- (b (e*ta - g*td + h*tf) )
	//			+ (c (e*tb - f*td + h*te) )
	//			- (d (e*tc - f*tf + g*te) )
	return ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
			- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
			+ (m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
			- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));

}

// calculate the adjugate (may be a better way)
// math provided by keenan
Matrix Matrix::GetAdjugate()
{
	// WRITTEN OUT

	// matrix = { a,b,c,d / e,f,g,h / j,k,l,m / n,o,p,q }

	// ta = lq - mp
	// tb = kq - mo
	// tc = kp - lo
	// td = gq - hp
	// te = fq - ho
	// tf = fp - go
	// tg = gm - hl
	// th = fm - hk
	// ti = fl - gk
	// tj = jq - mn
	// tk = jp - ln
	// tl = eq - hn
	// tm = ep - gn
	// tn = em - hj
	// to = el - gj
	// tp = jo - kn
	// tq = ek - fj
	// tr = eo - fn

	// a = det( { f,g,h / k,l,m / o,p,q } )
	// a = f(lq - mp) - g(kq - mo) + h(kp - lo)
	// a = f(ta) - g(tb) + h(tc)
	
	// b = -det( { b,c,d / k,l,m / o,p,q } )
	// b = -( b(lq - mp) - c(kq - mo) + d(kp - lo))
	// b = -( b(ta) - c(tb) + d(tc))

	// c = det( { b,c,d / f,g,h / o,p,q } )
	// c = b(gq - hp) - c(fq - ho) + d(fp - go)
	// c = b(td) - c(te) + d(tf)

	// d = -det( { b,c,d / f,g,h / k,l,m } )
	// d = -( b(gm - hl) - c(fm - hk) + d(fl - gk) )
	// d = -( b(tg) - c(th) + d(ti) )

	// e = -det( { e,g,h / j,l,m / n,p,q } )
	// e = - ( e(lq - mp) - g(jq - mn) + h(jp - ln))
	// e = - ( e(ta) - g(tj) + h(tk))
	
	// f = det( { a,c,d / j,l,m / n,p,q } )
	// f = a(lq - mp) - c(jq - mn) + d(jp - ln)
	// f = a(ta) - c(tj) + d(tk)
	
	// g = -det( { a,c,d / e,g,h / n,p,q } )
	// g = - ( a(gq - hp) - c(eq - hn) + d(ep - gn))
	// g = - ( a(td) - c(tl) + d(tm))

	// h = det( { a,c,d / e,g,h / j,l,m } )
	// h = a(gm - hl) - c(em - hj) + d(el - gj)
	// h = a(tg) - c(tn) + d(to)
	
	// j = det( { e,f,h / j,k,m / n,o,q } )
	// j = e(kq - mo) - f(jq - mn) + h(jo - kn)
	// j = e(tb) - f(tj) + h(tp)

	// k = -det( { a,b,d / j,k,m / n,o,q } )
	// k = - ( a(kq - mo) - b(jq - mn) + d(jo - kn))
	// k = - ( a(tb) - b(tj) + d(tp))

	// l = det( { a,b,d / e,f,h / n,o,q } )
	// l = a(fq - ho) - b(eq - hn) + d(eo - fn)
	// l = a(te) - b(tl) + d(tr)

	// m = -det( { a,b,d / e,f,h / j,k,m } )
	// m = -( a(fm - hk) - b(em - hj) + d(ek - fj))
	// m = -( a(th) - b(tn) + d(tq))

	// n = -det( { e,f,g / j,k,l / n,o,p } )
	// n = -( e(kp - lo) - f(jp - ln) + g(jo - kn))
	// n = -( e(tc) - f(tk) + g(tp))

	// o = det( { a,b,c / j,k,l / n,o,p } )
	// o = a(kp - lo) - b(jp - ln) + c(jo - kn)
	// o = a(tc) - b(tk) + c(tp)

	// p = -det( { a,b,c / e,f,g / n,o,p } )
	// p = - ( a(fp - go) - b(ep - gn) + c(eo - fn))
	// p = - ( a(tf) - b(tm) + c(tr))
	
	// q = det( { a,b,c / e,f,g / j,k,l } )
	// q = a(fl - gk) - b(el - gj) + c(ek - fj)
	// q = a(ti) - b(to) + c(tq)

	Matrix tmp;

	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);

	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc)) 
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -((m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);

	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);

	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));

	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);

	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));

	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);

	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -((m0*t4) - (m1*t3) + (m3*t5));

	// unload	M		(4)		DL-FR 
	// load		E		(5)		DLEFR 
	t3 = (m5*m15) - (m7*m13);

	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);

	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);

	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);
	
	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -( (m1*t1) - (m2*t2) + (m3*t3));
	
	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);

	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);

	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));

	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);

	return tmp;
}

// calculate the inverse
// math provided by keenan
void Matrix::Inverse(Matrix &out)
{
	Matrix tmp;
	float det = Determinant();
	if (fabs(det) < 0.0001)
	{
		// do nothing, Matrix is not invertable
	}
	else
	{
		tmp = GetAdjugate();
		tmp /= det;
	}

	out = tmp;
}

// set the matrix to a scale matrix
void Matrix::setScaleMatrix(Vect4D *scale)
{
	// WRITTEN OUT

	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}

	v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, scale->_m.m128_f32[0]);
	v1._m = _mm_set_ps(0.0f, 0.0f, scale->_m.m128_f32[1], 0.0f);
	v2._m = _mm_set_ps(0.0f, scale->_m.m128_f32[2], 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

// set the matrix to a Z rotation matrix
void Matrix::setRotZMatrix(float az)
{
	// WRITTEN OUT

	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}

	v0._m = _mm_set_ps(0.0f, 0.0f, -sin(az), cos(az));
	v1._m = _mm_set_ps(0.0f, 0.0f, cos(az), sin(az));
	v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

// End of file