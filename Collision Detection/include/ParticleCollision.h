#pragma once
#include <pcontacts.h>
#include <list>
#include <iostream>
#include <vector>
#include <gl/glut.h>


using namespace std;

class ParticleCollision : public ParticleContactGenerator
{
	
public:
	Vector2 checkCol;
	float distance;
	bool checkCollision(Particle *, Particle *);
	int blob_count;
	virtual unsigned addContact(
		ParticleContact *contact,
		unsigned limit
	);
	vector<Particle*> *simpleParticle;
};
