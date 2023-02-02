#include "SphereShape.h"

SphereShape::SphereShape(float radius)
	: iShape(ShapeType::ShapeTypeSphere)
	, m_Radius(radius)
{ }

SphereShape::~SphereShape()
{ }

SphereShape* SphereShape::Cast(iShape* shape)
{
	return dynamic_cast<SphereShape*>(shape);
}

float SphereShape::GetRadius() const
{
	return m_Radius;
}