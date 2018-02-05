# pragma once
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# include "Shape3D.hpp"

# define __CAMERA__

class Camera
{
private:
	glm::vec3 eye, at, up, initialEye, initialAt, initialUp;
	Shape3D* anchorShape;
public:
	Camera(glm::vec3 e, glm::vec3 a, glm::vec3 u)
	{
		eye = initialEye = e;
		at = initialAt = a;
		up = initialUp = u;
	}

	void setAnchor(Shape3D* anchor)
	{ anchorShape = anchor; }

	glm::mat4 getViewMatrix()
	{ return glm::lookAt(eye, at, up); }

	Shape3D* getAnchor()
	{
		return anchorShape;
	}

	void update()
	{
		if (anchorShape != NULL)
		{
			glm::mat4 TM = anchorShape->getTransformMatrix();
			eye = getPosition(TM) + getUp(TM) * initialEye.y + getOut(TM) * initialEye.z;
			at = getPosition(TM) + getUp(TM) * initialAt.y + getOut(TM) * initialAt.z;
			up = getUp(TM);
		}
	}
};