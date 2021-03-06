#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateCircleObstacle(int x, int y, int radius);
	PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreateFlipper(b2Vec2 points[], int size, float angle);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float dens, int rest, int filterIndex, b2BodyType type);
	p2List<PhysBody*>* GetLeftKickers();

	b2RevoluteJoint* CreateFlipperRevoluteJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 setBodyA, float upperAngle, float lowerAngle);
	void BuildLeftKickers();
	void BuildRightKickers();
	PhysBody* CreateKicker(int kickerX, int kickerY, int* points, int size);
	void KickersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

public:
	b2Body* big_ball;
	PhysBody* map;
	PhysBody* map_1;
	PhysBody* map_2;
	PhysBody* map_3;
	PhysBody* map_4;
	PhysBody* map_5;
	PhysBody* map_6;
	PhysBody* map_7;
	PhysBody* map_8;
	PhysBody* map_9;
	PhysBody* map_10;
	PhysBody* map_11;
	PhysBody* map_12;
	PhysBody* map_13;
	b2World* world;
	PhysBody* leftKickers = nullptr;
	PhysBody* rightKickers = nullptr;

private:

	bool debug;
	
	
	b2RevoluteJointDef revolutedef;
	b2RevoluteJoint* revolute_joint;
	b2DistanceJoint* distance_joint;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	

};