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

	leftKickers = new p2List<PhysBody*>;
	rightKickers = new p2List<PhysBody*>;


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
		25, 172,
		29, 168,
		19, 163,
		14, 160,
		11, 156,
		11, 121,
		10, 121,
		10, 162,
		18, 170,
		25, 170
	};

	//bottom right

	int map_vertex_2[16]
	{
		101, 121,
		101, 157,
		96, 162,
		83, 168,
		87, 173,
		97, 168,
		102, 161,
		102, 121,
	};

	//bottom left blue triangle
	int map_vertex_3[12]
	{
		26, 158,
		17, 155,
		17, 130,
		24, 142,
		28, 155,
		26, 158
	};

	//bottom right blue triangle
	int map_vertex_4[12]
	{
		95, 154,
		95, 129,
		88, 142,
		84, 156,
		87, 159,
		95, 154
	};

	//inverted big L
	int map_vertex_5[36]
	{
		19, 86,
		17, 89,
		14, 89,
		10, 85,
		10, 18,
		16, 10,
		24, 8,
		40, 8,
		50, 11,
		55, 17,
		41, 17,
		37, 9,
		27, 9,
		23, 17,
		16, 17,
		12, 24,
		12, 82,
		19, 86
	};

	//central map 
	int map_vertex_6[34]
	{
		55, 89,
		53, 86,
		60, 82,
		60, 45,
		62, 43,
		64, 45,
		64, 50,
		66, 50,
		66, 56,
		64, 56,
		64, 58,
		66, 58,
		66, 64,
		64, 64,
		64, 83,
		57, 89,
		55, 89
	};

	//pink stuff right
	int map_vertex_7[14]
	{
		95, 18,
		99, 18,
		103, 23,
		103, 27,
		99, 33,
		93, 33,
		93, 17
	};

	//pink stuff right 2
	int map_vertex_8[10]
	{
		87, 18,
		87, 33,
		84, 33,
		84, 18,
		87, 18
	};

	//pink stuff left 2
	int map_vertex_9[10]
	{
		78, 32,
		78, 18,
		75, 18,
		75, 33,
		78, 33
	};

	//pink stuff left
	int map_vertex_10[10]
	{
		69, 18,
		69, 33,
		66, 33,
		66, 18,
		69, 18
	};

	//top semicircle
	int map_vertex_11[20]
	{
		29, 15,
		30, 12,
		32, 11,
		34, 12,
		35, 15,
		36, 15,
		36, 12,
		32, 9,
		28, 12,
		28, 15
	};

	//right semicircle
	int map_vertex_12[12]
	{
		106, 93,
		111, 94,
		111, 100,
		106, 101,
		108, 97,
		106, 93
	};

	//weird little things right
	int map_vertex_13[10]
	{
		108, 71,
		108, 50,
		110, 50,
		110, 71,
		108, 71
	};

	//chains
	map = CreateChain(10, 10, map_vertex, 66);
	map_1 = CreateChain(10, 10, map_vertex_1, 20);
	map_2 = CreateChain(10, 10, map_vertex_2, 16);  
	map_3 = CreateChain(10, 10, map_vertex_3, 10);
	map_4 = CreateChain(10, 10, map_vertex_4, 10);
	map_5 = CreateChain(10, 10, map_vertex_5, 34);
	map_6 = CreateChain(10, 10, map_vertex_6, 32);
	map_7 = CreateChain(10, 10, map_vertex_7, 14);
	map_8 = CreateChain(10, 10, map_vertex_8, 8);
	map_9 = CreateChain(10, 10, map_vertex_9, 8);
	map_10 = CreateChain(10, 10, map_vertex_10, 8);
	map_11 = CreateChain(10, 10, map_vertex_11, 20);
	map_12 = CreateChain(10, 10, map_vertex_12, 10);
	map_13 = CreateChain(10, 10, map_vertex_13, 8);


	//setting it all to static
	map->body->SetType(b2_staticBody);
	map_1->body->SetType(b2_staticBody);
	map_2->body->SetType(b2_staticBody);
	map_3->body->SetType(b2_staticBody);
	map_4->body->SetType(b2_staticBody);
	map_5->body->SetType(b2_staticBody);
	map_6->body->SetType(b2_staticBody);
	map_7->body->SetType(b2_staticBody);
	map_8->body->SetType(b2_staticBody);
	map_9->body->SetType(b2_staticBody);
	map_10->body->SetType(b2_staticBody);
	map_11->body->SetType(b2_staticBody);
	map_12->body->SetType(b2_staticBody);
	map_13->body->SetType(b2_staticBody);

	//kickers
	BuildLeftKickers(leftKickers);
	BuildRightKickers(rightKickers);

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
	fixture.restitution = 0.2f;

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

PhysBody* ModulePhysics::CreateFlipper(b2Vec2 points[], int size, float angle) {
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.angle = angle * DEGTORAD;

	b2Body* b = App->physics->world->CreateBody(&body);
	b2PolygonShape flipper;
	flipper.Set(points, size);

	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.shape = &flipper;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateFlipperRevoluteJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 setBodyA, float upperAngle, float lowerAngle) {

	b2RevoluteJointDef jointDef;
	jointDef.bodyA = bodyA;
	jointDef.bodyB = bodyB;
	jointDef.collideConnected = false;

	b2Vec2 setA = setBodyA;
	b2Vec2 setB = bodyB->GetLocalCenter();

	jointDef.localAnchorA.Set(setA.x, setA.y);
	jointDef.localAnchorB.Set(setB.x, setB.y);

	jointDef.enableLimit = true;
	jointDef.upperAngle = upperAngle * DEGTORAD;
	jointDef.lowerAngle = lowerAngle * DEGTORAD;

	b2RevoluteJoint* revoluteJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

	return revoluteJoint;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
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

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, float dens, int rest, int filterIndex, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape shape;


	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.Set(p, size / 2);


	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = dens;
	fixture.restitution = rest;
	fixture.filter.groupIndex = filterIndex;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = 0;
	pbody->height = 0;

	return pbody;
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

PhysBody * ModulePhysics::CreateKicker(int kickerX, int kickerY, int* points, int size)
{
	return CreatePolygon(kickerX, kickerY, points, size, 100, 0, -2, b2_dynamicBody);
}

void ModulePhysics::BuildLeftKickers(p2List<PhysBody*>* leftKickers)
{
	/*int kicker1[10] = {
		49, 180,
		49, 175,
		30, 167,
		27, 173,
		36, 177
	};*/


	PhysBody* k = CreateRectangle(50, 186, 21, 5, b2_dynamicBody); //50/186/17/5/dynamic
	PhysBody* k2 = CreateRectangle(47, 182, 1, 1, b2_staticBody); //44/182/1/1/static

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(-0.04,-0.03);    //-0.08,-0.03  (in green thw 100% working combinations in case you wanna test)
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(0.1);
	revolutedef.upperAngle = (1.9);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	leftKickers->add(k);

}

void ModulePhysics::BuildRightKickers(p2List<PhysBody*>* rightKickers) 
{

	PhysBody* k = CreateRectangle(75, 186, 21, 5, b2_dynamicBody);
	PhysBody* k2 = CreateRectangle(88, 182, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.092, -0.035); //0.08/-0.035
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(0.1);
	revolutedef.upperAngle = (0.2);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	rightKickers->add(k);
}

//kickers force
void ModulePhysics::KickersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl) {
	if (rl == LEFT)
	{
		p2List_item<PhysBody*>* item = leftKickers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
	}
	else if (rl == RIGHT) {
		p2List_item<PhysBody*>* item = rightKickers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
	}
}

p2List<PhysBody*>* ModulePhysics::GetLeftKickers()
{
	return leftKickers;
}