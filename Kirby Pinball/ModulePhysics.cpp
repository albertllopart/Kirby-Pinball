#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);


	//map collisions

	//exterior part
	int map_vertex[66] = 
	{
		4, 177,
	4, 110,
	9, 102,
	9, 97,
	4, 85,
	4, 14,
	8, 6,
	15, 2,
	21, 1,
	101, 1,
	108, 3,
	114, 8,
	118, 14,
	118, 172,
	112, 172,
	112, 14,
	108, 10,
	103, 11,
	110, 15,
	110, 76,
	104, 82,
	104, 87,
	110, 90,
	110, 102,
	104, 105,
	104, 109,
	108, 112,
	108, 176,
	63, 191,
	63, 214,
	49, 214,
	49, 191,
	6, 177
	};

	//bottom left
	int map_vertex_1[20]
	{
		25, 173,
		29, 169,
		19, 164,
		14, 161,
		11, 157,
		11, 122,
		10, 122,
		10, 163,
		18, 171,
		25, 171
	};

	//bottom right

	int map_vertex_2[20]
	{
		102, 163,
		96, 169,
		86, 174,
		86, 172,
		83, 169,
		98, 161,
		101, 157,
		101, 122,
		102, 122,
		102, 163
	};

	//bottom left blue triangle
	int map_vertex_3[12]
	{
		26, 159,
		17, 156,
		17, 131,
		24, 143,
		28, 156,
		26, 159
	};

	//bottom right blue triangle
	int map_vertex_4[12]
	{
		95, 155,
		95, 130,
		88, 143,
		84, 157,
		87, 160,
		95, 155
	};

	//inverted big L
	int map_vertex_5[36]
	{
		19, 87,
		17, 90,
		14, 90,
		10, 86,
		10, 19,
		16, 11,
		24, 9,
		40, 9,
		50, 12,
		55, 18,
		41, 18,
		37, 10,
		27, 10,
		23, 18,
		16, 18,
		12, 25,
		12, 83,
		19, 87
	};

	//central map 
	int map_vertex_6[34]
	{
		55, 90,
		53, 87,
		60, 83,
		60, 46,
		62, 44,
		64, 46,
		64, 51,
		66, 51,
		66, 57,
		64, 57,
		64, 59,
		66, 59,
		66, 65,
		64, 65,
		64, 84,
		57, 90,
		55, 90
	};

	//pink stuff right
	int map_vertex_7[14]
	{
		95, 18,
		99, 18,
		103, 24,
		103, 28,
		99, 34,
		93, 34,
		93, 18
	};

	//pink stuff right 2
	int map_vertex_8[10]
	{
		87, 19,
		87, 34,
		84, 34,
		84, 19,
		87, 19
	};

	//pink stuff left 2
	int map_vertex_9[10]
	{
		78, 34,
		78, 19,
		75, 19,
		75, 34,
		78, 34
	};

	//pink stuff left
	int map_vertex_10[10]
	{
		69, 19,
		69, 34,
		66, 34,
		66, 19,
		69, 19
	};

	//top semicircle
	int map_vertex_11[20]
	{
		29, 16,
		30, 13,
		32, 12,
		34, 13,
		35, 16,
		36, 16,
		36, 13,
		32, 10,
		28, 13,
		28, 16
	};

	//right semicircle
	int map_vertex_12[12]
	{
		106, 94,
		111, 95,
		111, 101,
		106, 102,
		108, 98,
		106, 94
	};

	//weird little things right
	int map_vertex_13[10]
	{
		108, 72,
		108, 51,
		110, 51,
		110, 72,
		108, 72
	};

	//chains
	map = CreateChain(10, 10, map_vertex, 66);
	map = CreateChain(10, 10, map_vertex_1, 20);
	//map = CreateChain(10, 10, map_vertex_2, 22);
	//map = CreateChain(10, 10, map_vertex_3, 12);
	//map = CreateChain(10, 10, map_vertex_4, 12);
	//map = CreateChain(10, 10, map_vertex_5, 36);
	//map = CreateChain(10, 10, map_vertex_6, 34);
	map = CreateChain(10, 10, map_vertex_7, 14);
	//map = CreateChain(10, 10, map_vertex_8, 10);
	//map = CreateChain(10, 10, map_vertex_9, 10);
	//map = CreateChain(10, 10, map_vertex_10, 10);
	map = CreateChain(10, 10, map_vertex_11, 20);
	//map = CreateChain(10, 10, map_vertex_12, 12);
	//map = CreateChain(10, 10, map_vertex_13, 10);


	//setting it all to static
	map->body->SetType(b2_staticBody);
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 0.3f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleObstacle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	b2Body* found = nullptr;//todo 1
	b2Vec2 mouse_position = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())//itera cossos
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())//itera fixtures
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && b != big_ball) {

				if (f->TestPoint(mouse_position))
					found = b;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	/*if (found != nullptr)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = found;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * found->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

		//distance
		b2DistanceJointDef distance_def;
		distance_def.Initialize(big_ball, found, big_ball->GetWorldCenter(), found->GetWorldCenter());
		distance_def.dampingRatio = 0.5f;
		distance_def.frequencyHz = 2.0f;
		distance_joint = (b2DistanceJoint*)world->CreateJoint(&distance_def);

		//motor
		b2MotorJointDef motor_def;
		motor_def.Initialize(big_ball, found);

		motor_def.maxForce = 50.0f;
		motor_def.maxTorque = 0.0f;


	}*/


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	//mouse joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
	{
		b2Vec2 anchorA = mouse_joint->GetBodyB()->GetPosition();
		mouse_joint->SetTarget(mouse_position);
		b2Vec2 anchorB = mouse_joint->GetTarget();
		App->renderer->DrawLine(METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y), METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y), 255, 0, 0);
	}
	//distance joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && distance_joint != nullptr) {
		b2Vec2 anchorA = distance_joint->GetBodyA()->GetPosition();
		b2Vec2 anchorB = distance_joint->GetBodyB()->GetPosition();
		App->renderer->DrawLine(METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y), METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y), 255, 0, 0);
	}


	// TODO 4: If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		if(mouse_joint != nullptr)
		{
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}