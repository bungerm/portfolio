//----------------------------------------------------------------------------
// Mike Bunger
// ParticleEmitter header
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
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&) = default;
	ParticleEmitter& operator = (const ParticleEmitter&) = default;
	~ParticleEmitter() = default;

	//void SpawnParticle();
	//void update();
	void draw();

	//void addParticleToList(Particle *p);
	//void removeParticleFromList(Particle *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	void GoodBye();

	//my additions
	void loadParticles();
	void activateParticle();
	void myUpdate();

private:

	Vect4D start_position;
	Vect4D start_velocity;
	Vect4D vel_variance;
	Vect4D pos_variance;

	float spawn_frequency;
	float last_spawn;
	float last_loop;
	float scale_variance;

	//my additions
	Particle* activeHead;
	Particle* deadHead;
	Particle* topHeap;

	Matrix transCamera;

	// matrix mult method
	Matrix transParticle;
	Matrix rotParticle;
	Matrix scaleMatrix;
	Matrix tmp;
};

#endif

// End of File
