//----------------------------------------------------------------------------
// Mike Bunger
// ParticleEmitter header
//
// Particle Emitter block loads particles, keeps track of alive and dead,
// draws them
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"
#include "Matrix.h"

class ParticleEmitter
{
public:
	// big four
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&) = default;
	ParticleEmitter& operator = (const ParticleEmitter&) = default;
	~ParticleEmitter() = default;

	//void SpawnParticle();
	//void update();
	void draw();

	// no longer need these function
	//void addParticleToList(Particle *p);
	//void removeParticleFromList(Particle *p);

	// adds randomness to particle movement
	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	// frees up memory
	void GoodBye();

	//my additions
	void loadParticles();
	void activateParticle();
	void myUpdate();

private:
	// new particle start position and velocity
	Vect4D start_position;
	Vect4D start_velocity;
	// particle variances
	Vect4D vel_variance;
	Vect4D pos_variance;

	float spawn_frequency;
	float last_spawn;
	float last_loop;
	float scale_variance;

	//////////////////
	// MY ADDITIONS //
	//////////////////

	// two linked lists to keep track of alive vs dead particles
	// alive particles head pointer
	Particle* activeHead;
	// dead particles head pointer
	Particle* deadHead;
	// heap pointer (where the block loaded particles begin)
	Particle* topHeap;

	// camera transformation matrix
	Matrix transCamera;
};

#endif

// End of File
