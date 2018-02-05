#pragma once
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# include "Shape3D.hpp"

# define __MOVEMENTCONTROL__

#define PITCH	0
#define YAW		1
#define ROLL	2

class MovementControl
{
private:
	Shape3D* shape;
	const glm::vec3 axis[3] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };
	float radians = 0.02f;
	float stepDistance[3] = { 10,50,200 };
	int step, pitch, roll, yaw;
	int speed = 0;
	int rotateType = 0;
public:
	MovementControl(Shape3D* s)
	{ shape = s; step = pitch = roll = yaw = 0; }

	void changeSpeed()
	{ speed = (speed + 3 + 1) % 3; }

	void setMove(int i)
	{ step = i;}

	void setPitch(int i)
	{ pitch = i; }

	void setRoll(int i)
	{ roll = i; }

	void setYaw(int i)
	{ yaw = i; }

	void update()
	{
		if(pitch != 0)
			shape->controlledRotation(pitch * radians, axis[PITCH]);
		if (yaw != 0)
			shape->controlledRotation(yaw * radians, axis[YAW]);
		if (roll != 0)
			shape->controlledRotation(roll * radians, axis[ROLL]);
		shape->controlledTranslation(step * stepDistance[speed]);
		//step = pitch = yaw = roll = 0;
	}
};