#pragma once
#ifndef __INCLUDES465__
#include "../includes465/include465.hpp"
#define __INCLUDES465__
#endif

#include <array>
#include "Shape3D.hpp"

#define __SMARTMISSILE__

class SmartMissile
{
private:
	Shape3D* missile;
	Shape3D** targetArray;
	Shape3D* target;
	int nTargets;
	bool active;
	int lifetime;
	float speed;
	float radiusSum;

public:
	SmartMissile(Shape3D* shape, Shape3D* t[], int tSize, float s, float rS)
	{
		missile = shape;
		speed = s;
		lifetime = 0;
		active = false;
		targetArray = t;
		nTargets = tSize;
		radiusSum = rS;
	}

	void setTarget(Shape3D* newTarget)
	{ target = newTarget; }
	
	void activate()
	{
		active = true;
		missile->enable();
	}

	bool isActive()
	{ return active; }

	void destroyMissile()
	{
		lifetime = 0;
		active = false;
		missile->setTranslationMatrix(glm::mat4());
		missile->setRotationMatrix(glm::mat4());
		missile->disable();
	}

	void update()
	{
		if (active)
		{
			if (lifetime++ < 2000)
			{
				if (lifetime > 200)
				{
					if (target == NULL)
					{
						bool targetDetected = targetArray[0]->isEnabled();
						float min = distance(missile->getPos(), targetArray[0]->getPos());
						int minTarget = 0;
						for (int i = 1; i < nTargets; i++)
						{
							float current = distance(missile->getPos(), targetArray[i]->getPos());
							if (current < min)
							{
								if (targetArray[i]->isEnabled())
								{
									min = current;
									minTarget = i;
									targetDetected = true;
								}
							}
						}
						if (targetDetected)
							setTarget(targetArray[minTarget]);
					}

					if (distance(missile->getPos(), target->getPos()) > radiusSum)
					{
						glm::vec3 missileLookAt = normalize(getIn(missile->getModelMatrix()));
						glm::vec3 missilePos = missile->getPos();
						glm::vec3 targetPos = target->getPos();
						glm::vec3 targetLookAt = normalize(targetPos - missilePos);
						glm::vec3 axis = normalize(cross(targetLookAt, missileLookAt));
						float axisDirection = axis.x + axis.y + axis.z;
						float aCos = acos(dot(targetLookAt, missileLookAt));
						float radians;

						if (axisDirection >= 0)
							radians = aCos;
						else
							radians = 2 * PI - aCos;

						if (target != NULL)
						{
							if (colinear(targetLookAt, missileLookAt, 0.1f))
							{
								if (length(targetLookAt - missileLookAt) > 0.5f)
									missile->controlledRotation(PI, axis);
							}
							else
								missile->controlledRotation(radians, axis);
						}
					}
					else
					{
						target->disable();
						destroyMissile();
					}
				}
				missile->controlledTranslation(speed);
			}
			else
				destroyMissile();
		}
	}

};
