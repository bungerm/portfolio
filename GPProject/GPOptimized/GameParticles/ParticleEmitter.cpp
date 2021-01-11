//----------------------------------------------------------------------------
// Mike Bunger
// Particle Emitter
// 
// Many optimizations:
// Implemented two linked lists to replace stl (Alive and Dead Particles)
// Block loaded the particles in constructor
// Rearranged OpenGL methods and removed invariants from loops
// Changed all doubles to floats
// Removed vestigial drawbuffer code
// Calculate the transform of a particle in one function
// and more!
//----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\OpenGLInterface.h"
#include "Framework.h"

#include "ParticleEmitter.h"
#include "Settings.h"
#define UNUSED_VAR(x) (void *)x
extern PerformanceTimer GlobalTimer;

static const unsigned char squareColors[] =
{
	// ----------------------------
	//  point is actually a quad   
	//  set color on each vertex   
	// ----------------------------
	//    Vert A = Yellow 
	//    Vert B = Yellow
	//    Vert C = Yellow
	//    Vert D = Yellow
	// ----------------------------

	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	// --------------------
	//   Vert A = (x,y,z)
	//   Vert B = (x,y,z)
	//   Vert C = (x,y,z)
	//   Vert D = (x,y,z)
	// --------------------

	-0.015f,  -0.015f, 0.0f, // Size of Triangle
	 0.015f,  -0.015f, 0.0f, // Size of Triangle
	-0.015f,   0.015f, 0.0f, // Size of Triangle
	 0.015f,   0.015f, 0.0f, // Size of Triangle
};

ParticleEmitter::ParticleEmitter() :
	start_position(0.0f, 0.0f, 0.0f),
	start_velocity(0.0f, 1.0f, 0.0f),
	spawn_frequency(0.0000001f),
	vel_variance(1.0f, 4.0f, 0.4f),
	pos_variance(1.0f, 1.0f, 1.0f),
	scale_variance(2.5f),
	activeHead(nullptr),
	deadHead(nullptr),
	topHeap(nullptr)
{
	GlobalTimer.Toc();

	last_spawn = GlobalTimer.TimeInSeconds();
	last_loop = GlobalTimer.TimeInSeconds();

	this->loadParticles();

	// OpenGL goo... don't worrry about this
	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);
}

//ParticleEmitter::~ParticleEmitter()
//{
//	// do nothing
//}

void ParticleEmitter::loadParticles() {
	// create all particles in memory
	//this->deadHead = new Particle[NUM_PARTICLES]();
	//this->topHeap = this->deadHead;
	//Particle* nextPtr = (Particle*)(this->deadHead + 1);
	//this->deadHead->next = nextPtr;


	this->deadHead = (Particle*)_aligned_malloc(NUM_PARTICLES * sizeof(Particle), 16);
	this->topHeap = this->deadHead;

	Particle* nextPtr = deadHead;

	for (int i = 0; i < NUM_PARTICLES - 1; i++) {
		nextPtr->next = (Particle*)(nextPtr + 1);
		nextPtr = nextPtr->next;
	}
	nextPtr->next = nullptr;


	//Particle* currentPtr = new Particle();
	//Particle* nextPtr = new Particle();
	//currentPtr->next = nextPtr;
	//this->deadHead = currentPtr;

	//for (int i = 0; i < NUM_PARTICLES -2; i++) {
	//	currentPtr = nextPtr;
	//	nextPtr = new Particle();
	//	currentPtr->next = nextPtr;
	//}
	//nextPtr->next = nullptr;
}

void ParticleEmitter::activateParticle() {
	// create another particle if there are ones free
	if (this->deadHead != nullptr)
	{
		// create new particle
		Particle* newParticle = this->deadHead;
		this->deadHead = this->deadHead->next;

		// add to active list
		if (this->activeHead == nullptr)
		{ // first on list
			this->activeHead = newParticle;
			newParticle->next = nullptr;
		}
		else
		{ // add to front of list
			newParticle->next = activeHead;
			activeHead = newParticle;
		}

		// initialize the particle
		newParticle->life = 0.0f;
		newParticle->position = start_position;
		newParticle->velocity = start_velocity;
		newParticle->scale.set(2.0f, 2.0f, 2.0f, 2.0f);
		newParticle->rotation = 0.0f;
		newParticle->rotation_velocity = 0.25f;

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);
	}
}

void ParticleEmitter::myUpdate()
{
	// get current time
	GlobalTimer.Toc();
	float current_time = GlobalTimer.TimeInSeconds();

	// spawn particles
	float time_elapsed = current_time - last_spawn;

	// update
	while (spawn_frequency < time_elapsed)
	{
		// spawn a particle
		this->activateParticle();
		// adjust time
		time_elapsed -= spawn_frequency;
		// last time
		last_spawn = current_time;
	}

	// total elapsed
	time_elapsed = current_time - last_loop;

	Particle* last = nullptr;
	Particle* current = this->activeHead;

	// walk the particles
	while (current != nullptr)
	{
		// call every particle and update its position 
		current->Update(time_elapsed);

		// if it's life is greater that the max_life 
		if (current->life > MAX_LIFE)
		{
			if (current == this->activeHead) {
				this->activeHead = this->activeHead->next;
				current->next = this->deadHead;
				this->deadHead = current;
				last = nullptr;
				current = this->activeHead;
			}
			else {
				last->next = current->next;
				current->next = this->deadHead;
				this->deadHead = current;
				current = last->next;
			}
		}
		else {
			last = current;
			current = current->next;
		}
	}

	last_loop = current_time;
}

// CALCULATE THE TRANSFORM OF A PARTICLE IN ONE FUNCTION, NICE!
float* calcTransform(const Vect4D& s, const Matrix& c, const Vect4D& p, const float r) {
	Matrix out;

	out.v0._m = _mm_set_ps(0.0f, 0.0f, s.x*-sin(r)*s.y, s.x*cos(r)*s.x);
	out.v1._m = _mm_set_ps(0.0f, 0.0f, s.y*cos(r)*s.y, s.y*sin(r)*s.x);
	out.v2._m = _mm_set_ps(0.0f, s.z*s.z, 0.0f, 0.0f);
	out.v3._m = _mm_set_ps(1.0f, (c.v3.z+p.z)*s.z,
		((c.v3.x + p.x) * -sin(r) + (c.v3.y + p.y) * cos(r)) * s.y,
		((c.v3.x + p.x) * cos(r) + (c.v3.y + p.y) * sin(r)) * s.x);

	return &out.v0._m.m128_f32[0];
}

void ParticleEmitter::draw()
{
	// get the camera matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, &transCamera.v0.x);

	// iterate throught the list of particles
	Particle* active = activeHead;

	while (active != nullptr)
	{
		// matrix mult method
		//transParticle.setTransMatrix(&(active->position * 0.35));
		//rotParticle.setRotZMatrix(active->rotation);
		//scaleMatrix.setScaleMatrix(&active->scale);
		//tmp = scaleMatrix * transCamera * transParticle * rotParticle * scaleMatrix;
		//glLoadMatrixf(reinterpret_cast<float*>(&(tmp)));


		// set the transformation matrix
		glLoadMatrixf(calcTransform(active->scale, transCamera, active->position * 0.35f, active->rotation));

		// not sure about this
		//active->currMatrix = tmp;

		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		active->diffMatrix = active->currMatrix - active->prevMatrix;
		active = active->next;

		// clears or flushes some internal setting, used in debug, but is need for performance reasons
		// magic...  really it's magic.
				// do not delete these two lines
		GLenum glError = glGetError();
		UNUSED_VAR(glError);
	}
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// --------------------------------------------------------------
	//   Ses it's ugly - I didn't write this so don't bitch at me   |
	//   Sometimes code like this is inside real commerical code    |
	//   ( so now you know how it feels )  |
	//---------------------------------------------------------------

	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.005f; // Var
	float sign = static_cast<float>(rand() % 2);  // Sign 
	float* t_pos = reinterpret_cast<float*>(&pos);
	float* t_var = &pos_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 1.5f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	if (sign == 0)
	{
		var *= -1.0;
	}
	sc = sc * var;
}


void ParticleEmitter::GoodBye()
{
	//Particle *pTmp = this->headParticle;
	//Particle *pDeadMan = nullptr;
	//while (pTmp)
	//{
	//	pDeadMan = pTmp;
	//	pTmp = pTmp->next;
	//	delete pDeadMan;
	//}

	_aligned_free(topHeap);
	//_aligned_free(start_position);

	//Particle* temp = deadHead;
	//while (deadHead != nullptr) {
	//	deadHead = deadHead->next;
	//	delete temp;
	//	temp = deadHead;
	//}

	//temp = activeHead;
	//while (activeHead != nullptr) {
	//	activeHead = activeHead->next;
	//	delete temp;
	//	temp = activeHead;
	//}
}

// End of file
