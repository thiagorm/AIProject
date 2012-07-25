#include <limits>
#include <stdio.h>
#include <math.h>
#include "Agent.h"


Agent::Agent(Vector2D position, Vector2D velocity)
{
	this->position = position;
	this->velocity = velocity;
}

Agent::Agent()
{
}

inline Vector2D Vec2DNormalize(Vector2D v)
{
  Vector2D vec;

  float vector_length = v.length();

  //if (vector_length > std::numeric_limits<float>::epsilon())
  //{
    vec.x = v.x / vector_length;
    vec.y = v.y / vector_length;

	//printf("vec --- %f\n", vec.x);
  //}

  return vec;
}

Vector2D Agent::calculate_steering(Vector2D target)
{
	Vector2D desired_velocity;
	Vector2D teste;

	desired_velocity = Vec2DNormalize(target - this->position) * MAX_SPEED;
	//printf("Position --- %f\n", target.x);
	//printf("Desired --- %f\n", desired_velocity.x);
	teste = desired_velocity - this->velocity;
	//printf("Velocity --- %f\n", this->velocity.x);
	//printf("Desired 2 --- %f\n", teste.x);
	return (desired_velocity - this->velocity);
}
