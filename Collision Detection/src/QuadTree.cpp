#include "..\include\QuadTree.h"

Quadtree::Quadtree(float x, float y, float width, float height, int level, int maxLevel)
{
	
}

void Quadtree::addObject(Particle *object)
{
	if (level == maxLevel)
	{
		objects.push_back(object);
		return;
	}

	if (contains(NW, object))
	{
		NW->size++;
		NW->addObject(object);
		return;
	}
	else if (contains(NE, object))
	{
		NE->size++;
		NE->addObject(object);
		return;
	}
	else if (contains(SW, object))
	{
		SW->size++;
		SW->addObject(object);
		return;
	}
	else if (contains(SE, object))
	{
		SE->size++;
		SE->addObject(object);
		return;
	}
	if (contains(this, object))
		objects.push_back(object);
}

void Quadtree::preOrder(Quadtree * quad)
{
	if (quad != nullptr)
	{
		cout << quad;
		preOrder(quad->NW);
		preOrder(quad->NE);
		preOrder(quad->SW);
		preOrder(quad->SE);
	}
}

bool Quadtree::contains(Quadtree * child, Particle * object)
{
	return !(object->getPosition().x < child->x ||
		object->getPosition().y < child->y ||
		object->getPosition().x > child->x + child->width ||
		object->getPosition().y > child->y + child->height ||
		object->getPosition().x - object->getRadius() < child->x ||
		object->getPosition().y - object->getRadius() < child->y ||
		object->getPosition().x + object->getRadius() > child->x + child->width ||
		object->getPosition().y + object->getRadius() > child->y + child->height);

}
