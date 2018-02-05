# pragma once
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __SHAPE3D__

class Shape3D
{
private:

	glm::mat4 rotationMatrix, scaleMatrix, translationMatrix;
	Shape3D* anchorShape;
	glm::mat4 anchorMatrix;
	glm::vec3 rotationAxis;
	float radians; 
	bool orbital;
	bool enabled;

public:

	Shape3D(glm::vec3 translate, float rads, bool orbit, bool e)
	{
		rotationMatrix = glm::mat4();  // no initial orientation
		//set cube's  rotation axis and rotation radians
		rotationAxis = glm::vec3(0, 1, 0);
		// rotation speed
		radians = rads;
		// initial placement
		translationMatrix = glm::translate(glm::mat4(), translate);
		anchorMatrix = glm::mat4();
		orbital = orbit;
		enabled = e;
	}

	bool isEnabled()
	{ return enabled; }

	void enable()
	{ enabled = true; }

	void disable()
	{ enabled = false; }

	void setScale(float modelBR, float modelSize)
	{
		glm::vec3 scale = glm::vec3(modelSize / modelBR);
		scaleMatrix = glm::scale(glm::mat4(), scale);
	}

	glm::mat4 getScaleMatrix() {
		return scaleMatrix;
	}

	void setAnchor(Shape3D* anchor, float r)
	{
		anchorShape = anchor;
		radians = r;
		anchorMatrix = anchorShape->getTransformMatrix();
	}

	glm::vec3 getPos()
	{ return getPosition(getTransformMatrix()); }

	glm::mat4 getModelMatrix()
	{ return (getTransformMatrix() * scaleMatrix); }

	void setRotationMatrix(glm::mat4 r)
	{ rotationMatrix = r; }
	
	glm::mat4 getRotationMatrix()
	{ return rotationMatrix; }
	
	void setTranslationMatrix(glm::mat4 t)
	{ translationMatrix = t; }

	glm::mat4 getTranslationMatrix()
	{ return translationMatrix; }

	void controlledWarp(Shape3D* shape, glm::vec3 offset)
	{
		glm::mat4 TM = shape->getTransformMatrix();
		glm::mat4 RM = shape->getRotationMatrix();
		rotationMatrix = rotate(RM, PI, getUp(TM));
		translationMatrix = translate(glm::mat4(), getPosition(TM) + getUp(TM) * offset.y + getOut(TM) * offset.z);
	}

	void controlledRotation(float rads, glm::vec3 axis)
	{ rotationMatrix = glm::rotate(rotationMatrix, rads, axis); }

	void controlledTranslation(float step)
	{ translationMatrix = glm::translate(translationMatrix, getIn(getModelMatrix()) * step); }

	glm::mat4 getTransformMatrix()
	{
		if (orbital) // orbital rotation
			return (anchorMatrix * rotationMatrix * translationMatrix);
		else  // center rotation
			return (anchorMatrix * translationMatrix * rotationMatrix);
	}

	void update()
	{
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		if (anchorShape != NULL)
			anchorMatrix = anchorShape->getTransformMatrix();
	}   
};