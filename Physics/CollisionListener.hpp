#pragma once
#include <box2d/box2d.h>

class CollisionListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contactInfo);
	void EndContact(b2Contact* contactInfo);
protected:
};

