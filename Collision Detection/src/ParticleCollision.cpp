#include <ParticleCollision.h>
#include <particle.h>



unsigned ParticleCollision::addContact(ParticleContact * contact, unsigned limit)
{
	unsigned used = 0;

	for (int i = 0; i < simpleParticle->size() - 1; i++)
	{
		if (used >= limit) return used;
		// runs a second for loop from the first loop + 1 as this will check the next circles in the array  
		for (int k = i + 1; k < simpleParticle->size(); k++)
		{
			Particle *particle1 = (*simpleParticle)[i];
			Particle *particle2 = (*simpleParticle)[k];
			//sends the particles into a function called checkCollision
			if (checkCollision(particle1, particle2))
			{
				//cout << "there was a collision between " << i <<  " & " << j << " " << endl;
				//gets the contactNormal which is the first particles position - the second particles position and normalises the results
				contact->contactNormal = (particle1->getPosition() - particle2->getPosition()).unit();
				//resitution is the bounce back after colliding which will be a default 0.8f 
				contact->restitution = 1.0f;
				//holds what is in particles particles i
				contact->particle[0] = particle1;
				// holds what is in particles j to check between the particles[0] & particles[1]
				contact->particle[1] = particle2;
				// checks the penetration of the particles colliding by getting the radius of particle[i] - the magnitude of both particles.positions
				contact->penetration = particle1->getRadius() - (particle1->getPosition() - (particle2->getPosition())).magnitude();
				used++;
				contact++;
			}
					}

	}
	
	return used;
}

bool ParticleCollision::checkCollision(Particle *firstParticle, Particle *secondParticle)
{
	//position of first particle - position of second particle is assigned to a vector.
	checkCol = firstParticle->getPosition() - secondParticle->getPosition();

	distance = checkCol.magnitude();

	//if distance is less than the radius of first particle + second particle, 
	//report true and a collision occured.
	if (distance < firstParticle->getRadius() + secondParticle->getRadius())
		return true;
	else
		return false;

	
}
