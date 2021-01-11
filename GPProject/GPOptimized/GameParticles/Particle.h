//----------------------------------------------------------------------------
// Mike Bunger
// Particle header
//
// Tried using the C union struct trick, no performance gains
//----------------------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vect4D.h"
#include "Matrix.h"

class Particle : public Align16
{
public:
	friend class ParticleEmitter;

	Particle() = default;
	Particle(const Particle&);
	Particle& operator = (const Particle&);
	~Particle() = default;

	void Update(const float& time_elapsed);
	//void CopyDataOnly(Particle *p);

private:
	Particle* next;
	//Particle* prev;

	float	life;
	float	rotation;
	float	rotation_velocity;

	Matrix prevMatrix;
	Matrix diffMatrix;
	Matrix currMatrix;

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;

	//union {
	//	struct {

	//	};

	//	struct {
	//		Vect4D	prev_Row0;
	//		Vect4D	prev_Row1;
	//		Vect4D  prev_Row2;
	//		Vect4D  prev_Row3;

	//		Vect4D  diff_Row0;
	//		Vect4D  diff_Row1;
	//		Vect4D  diff_Row2;
	//		Vect4D  diff_Row3;

	//		Vect4D	curr_Row0;
	//		Vect4D	curr_Row1;
	//		Vect4D  curr_Row2;
	//		Vect4D  curr_Row3;

	//	};
	//};
};


#endif 

// End of File
