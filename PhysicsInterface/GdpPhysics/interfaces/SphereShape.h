#pragma once

#include "iShape.h"

class SphereShape : public iShape
{
public:
	SphereShape(float radius);
	virtual ~SphereShape();

	float GetRadius() const;

	static SphereShape* Cast(iShape* shape);

protected:
	SphereShape(ShapeType shapeType)
		: iShape(shapeType) {}

private:
	float m_Radius;
	SphereShape(const SphereShape&) : iShape(ShapeTypeSphere) {}
	SphereShape& operator=(const SphereShape&) {
		return *this;
	}
};
