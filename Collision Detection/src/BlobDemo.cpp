/*
 * The Blob demo.
 *
 */
#include <gl/glut.h>
#include "app.h"
#include "coreMath.h"
#include "pcontacts.h"
#include "pworld.h"
#include <stdio.h>
#include <cassert>
#include <QuadTree.h>


const Vector2 Vector2::GRAVITY = Vector2(0,-9.81);
int PLATFORM_COUNT = 5;
int BLOB_COUNT = 6;

bool updatePenetration = true;

/**
 * Platforms are two dimensional: lines on which the 
 * particles can rest. Platforms are also contact generators for the physics.
 */

class Platform : public ParticleContactGenerator
{
public:
    Vector2 start;
    Vector2 end;
	float restitution;
    /**
     * Holds a pointer to the particles we're checking for collisions with. 
     */
    Particle *particle;

    virtual unsigned addContact(
        ParticleContact *contact, 
        unsigned limit
        ) ;
};

unsigned Platform::addContact(ParticleContact *contact, 
                              unsigned limit) 
{
    
	//const static float restitution = 0.8f;
	//const static float restitution = 1.0f;
	unsigned used = 0;
	unsigned i = 0;

	for (unsigned i = 0; i < BLOB_COUNT; i++)
	{
		if (used >= limit) {
			return used;
		}
		Vector2 toParticle = particle[i].getPosition() - start;
		Vector2 lineDirection = end - start;

		float projected = toParticle * lineDirection;
		float platformSqLength = lineDirection.squareMagnitude();
		float squareRadius = particle[i].getRadius()*particle[i].getRadius();

		if (projected <= 0)
		{

			// The blob is nearest to the start point
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particle + i;
				contact->particle[1] = 0;
				contact->penetration = particle[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}

		}
		else if (projected >= platformSqLength)
		{
			// The blob is nearest to the end point
			toParticle = particle[i].getPosition() - end;
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particle + i;
				contact->particle[1] = 0;
				contact->penetration = particle[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}
		}
		else
		{
			// the blob is nearest to the middle.
			float distanceToPlatform = toParticle.squareMagnitude() - projected * projected / platformSqLength;
			if (distanceToPlatform < squareRadius)
			{
				// We have a collision
				Vector2 closestPoint = start + lineDirection * (projected / platformSqLength);

				contact->contactNormal = (particle[i].getPosition() - closestPoint).unit();
				contact->restitution = restitution;
				contact->particle[0] = particle + i;
				contact->particle[1] = 0;
				contact->penetration = particle[i].getRadius() - sqrt(distanceToPlatform);
				used++;
				contact++;
			}
		}
	}
        
        // Check for penetration
        
       
        
        

    return used;
}


class BlobDemo : public Application
{
    Particle *blob;

    Platform *platform;

    ParticleWorld world;


public:
    /** Creates a new demo object. */
    BlobDemo();
    virtual ~BlobDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();
	
};

// Method definitions
BlobDemo::BlobDemo() :world(20, 20)
{
	width = 400; height = 400; 
	nRange = 250.0;
	 
	float margin = 0.95;
	
    // Create the blob storage	
    blob = new Particle[BLOB_COUNT];
	   
    // Create the platform
	platform = new Platform[PLATFORM_COUNT];
	

	platform[0].start = Vector2 ( -300, -300.0 );
	platform[0].end   = Vector2 (  300.0, -300.0 );
	platform[0].restitution = 1;

	platform[1].start = Vector2(-nRange*margin, -nRange*margin);
	platform[1].end = Vector2(nRange*margin, -nRange*margin);
	platform[1].restitution = 1;

	platform[2].start = Vector2(-nRange * margin, nRange * margin);
	platform[2].end = Vector2(nRange*margin, nRange * margin);
	platform[2].restitution = 1;

	platform[3].start = Vector2(-nRange * margin, -nRange * margin);
	platform[3].end = Vector2(-nRange*margin, nRange * margin);
	platform[3].restitution = 1;

	platform[4].start = Vector2(nRange * margin, nRange * margin);
	platform[4].end = Vector2(nRange*margin, -nRange * margin);
	platform[4].restitution = 1;

	/*platform[5].start = Vector2(-300.0, 112.0);
	platform[5].end = Vector2(-300.0, -300.0);
	platform[5].restitution = 1;

	platform[6].start = Vector2(300.0, 112.0);
	platform[6].end = Vector2(300.0, -300.0);
	platform[6].restitution = 1;

	platform[7].start = Vector2(-200.0, -100.0);
	platform[7].end = Vector2(-250.0, -100.0);
	platform[7].restitution = 1;

	platform[8].start = Vector2(150.0, 75.0);
	platform[8].end = Vector2(-125.0, 50.0);
	platform[8].restitution = 1;*/

	//platform[9].start = Vector2(100.0, -100.0);
	//platform[9].end = Vector2(150.0, -100.0);
	//platform[9].restitution = 1;

	/*platform[10].start = Vector2(200.0, -100.0);
	platform[10].end = Vector2(250.0, -100.0);
	platform[10].restitution = 1;*/

	//platform[11].start = Vector2(-300, 112.0);
	//platform[11].end = Vector2(300.0, 112.0);
	//platform[11].restitution = 1;

	for (unsigned i = 0; i < BLOB_COUNT; i++)
	{
		// Create the blob
		blob[i].setPosition(-20.0 + i * 10, 90.0);
		blob[i].setRadius(20);
		blob[i].setVelocity(0, 0);
		blob[i].setDamping(0.9);
		blob[i].setAcceleration(Vector2::GRAVITY * 2.5f * (i + 1));
		blob[i].clearAccumulator();
		blob[i].setMass(100.0f);
		blob[i].clearAccumulator();
		world.getParticles().push_back(blob + i);
	}

	for (unsigned i = 0; i < PLATFORM_COUNT; i++)
	{
		// Make sure the platform knows which particle it should collide with.
		platform[i].particle = blob;
		world.getContactGenerators().push_back(platform + i);
	}

	
 
}


BlobDemo::~BlobDemo()
{
    delete blob;
}

void BlobDemo::display()
{
  Application::display();

 
   glBegin(GL_LINES);
   glColor3f(0,1,1);

   for (unsigned i = 0; i < PLATFORM_COUNT; i++)
   {
	   const Vector2 &p0 = platform[i].start;
	   const Vector2 &p1 = platform[i].end;

	   glVertex2f(p0.x, p0.y);
	   glVertex2f(p1.x, p1.y);
   }
   
   glEnd();
	
   for (unsigned i = 0; i < BLOB_COUNT; i++)
   {
	   glColor3f((i%2)?0:1, (i%2)?1:0, 0);
	   const Vector2 &p = blob[i].getPosition();
	   glPushMatrix();
	   glTranslatef(p.x, p.y, 0);
	   glutSolidSphere(blob[i].getRadius(), 12, 12);
	   glPopMatrix();
   }

   

    

	glutSwapBuffers();
    
}

void BlobDemo::update()
{
    // Recenter the axes
	float duration = timeinterval/1000;
    // Run the simulation
    world.runPhysics(duration);

    Application::update();
}

const char* BlobDemo::getTitle()
{
    return "Blob Demo";
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new BlobDemo();
}