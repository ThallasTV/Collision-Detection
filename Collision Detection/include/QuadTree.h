#pragma once
#include <iostream>
#include <vector>
#include <gl/glut.h>
#include <particle.h>

using namespace std;

class Quadtree
{
public:
	Quadtree(float x, float y, float width, float height, int level, int maxLevel);
	~Quadtree();

	void addObject(Particle *object);
	void preOrder(Quadtree *quad);

	int size;

private:
	float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;

	vector<Particle*>objects;

	Quadtree *parent;
	Quadtree *NW;
	Quadtree *NE;
	Quadtree *SW;
	Quadtree *SE;

	//RectangleShape shape;

	bool contains(Quadtree *child, Particle *object);
};