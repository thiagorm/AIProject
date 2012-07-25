#ifndef _Agent_
#define _Agent_

#include "Vector2D.h"

const float MAX_FORCE = 0.05f;
const float MAX_SPEED = 0.8f;

class Agent
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D force;
	
	Agent();
	Agent(Vector2D new_position, Vector2D new_velocity);

	Vector2D calculate_steering(Vector2D target);
	//Vector2D TruncateOver(Vector2D vector, float max);
	//Vector2D update_seek(Agent entity, Vector2D target);

};

#endif