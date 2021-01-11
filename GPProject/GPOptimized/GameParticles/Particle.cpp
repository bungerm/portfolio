//----------------------------------------------------------------------------
// Mike Bunger
// Particle
//
// Experimented with constructor, assignment operator, update loop
// Commented out code is optimization trial and error
//----------------------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

//Particle::Particle()
//{
//	// construtor
//	this->next = nullptr;
//	this->life = 0.0f;
//	this->rotation = 0.0f;
//	this->rotation_velocity = 0.25f;
//
//	this->position.set(0.0f, 0.0f, 0.0f);
//	this->velocity.set(0.0f, 0.0f, 0.0f);
//	this->scale.set(1.0f, 1.0f, 1.0f);
//	//this->prev = nullptr;
//}

//Particle::~Particle()
//{
//	// nothing to do
//}

//void Particle::CopyDataOnly(Particle *p)
//{
//	// copy the data only
//	// this way of copying data is more efficient that element by element
//	this->position = p->position;
//	this->velocity = p->velocity;
//	this->scale = p->scale;
//	this->rotation = p->rotation;
//	this->rotation_velocity = p->rotation_velocity;
//	this->life = p->life;
//
//}

//Particle::Particle() : 
//	next(nullptr), life(0), rotation(0), rotation_velocity(0), prevMatrix(Matrix()), currMatrix(Matrix()),
//	diffMatrix(Matrix()), position(Vect4D()), velocity(Vect4D()), scale(Vect4D())
//{
//}

Particle::Particle(const Particle& p)
{
	this->next = p.next;
	this->life = p.life;
	this->rotation = p.rotation;
	this->rotation_velocity = p.rotation_velocity;
	this->prevMatrix = p.prevMatrix;
	this->currMatrix = p.currMatrix;
	this->diffMatrix = p.diffMatrix;
	this->position = p.position;
	this->velocity = p.velocity;
	this->scale = p.scale;
}

Particle& Particle::operator=(const Particle& p)
{
	this->next = p.next;
	this->life = p.life;
	this->rotation = p.rotation;
	this->rotation_velocity = p.rotation_velocity;
	this->prevMatrix = p.prevMatrix;
	this->currMatrix = p.currMatrix;
	this->diffMatrix = p.diffMatrix;
	this->position = p.position;
	this->velocity = p.velocity;
	this->scale = p.scale;
	return *this;
}

void Particle::Update(const float& time_elapsed)
{
	prevMatrix = currMatrix;

	float MatrixScale = diffMatrix.Determinant();

	// serious math below - magic secret sauce
	life += time_elapsed;
	position += (velocity * (time_elapsed));

	Vect4D axis(1, 0, 0);
	Vect4D v(0, 50, 0);
	position.Cross(axis, v);
	v.norm(v);
	position += v * 0.07f * life;

	if (MatrixScale > 1.0)
	{
		MatrixScale = 1.0f / MatrixScale;
	};

	
	rotation += (MatrixScale + rotation_velocity) * time_elapsed * 4.0f;
}


// End of file
