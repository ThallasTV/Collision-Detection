
#include <cstdlib>
#include <pworld.h>

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
:
resolver(iterations),
maxContacts(maxContacts)
{
    contacts = new ParticleContact[maxContacts];
    calculateIterations = (iterations == 0);
	pCollision.simpleParticle = &particles;
}

ParticleWorld::~ParticleWorld()
{
    delete[] contacts;
}

unsigned ParticleWorld::generateContacts()
{
    unsigned limit = maxContacts;
	unsigned used = 0;
    ParticleContact *nextContact = contacts;

	used = pCollision.addContact(nextContact, limit);
	limit -= used;
	nextContact += used;
	
    for (ContactGenerators::iterator platformContact = contactGenerators.begin();
        platformContact != contactGenerators.end();
        platformContact++)
    {
        used = (*platformContact)->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // We've run out of contacts to fill. This means we're missing
        // contacts.
        if (limit <= 0)
			break;
    }

    // Return the number of contacts used.
    return maxContacts - limit;
}

void ParticleWorld::integrate(float duration)
{
    for (Particles::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        // Remove all forces from the accumulator
        (*p)->integrate(duration);
    }
}

void ParticleWorld::runPhysics(float duration)
{

    // Then integrate the objects
    integrate(duration);

    // Generate contacts
    unsigned usedContacts = generateContacts();

    // And process them
    if (usedContacts)
    {
        if (calculateIterations) resolver.setIterations(usedContacts * 2);
        resolver.resolveContacts(contacts, usedContacts, duration);
    }
}

ParticleWorld::Particles& ParticleWorld::getParticles()
{
    return particles;
}

ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerators()
{
    return contactGenerators;
}
