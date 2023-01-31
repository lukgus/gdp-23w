#include "CylinderShape.h"

CylinderShape::CylinderShape(const Vector3& halfExtents)
	: iShape(ShapeTypeCylinder)
	, m_HalfExtents(halfExtents)
{ }

CylinderShape::~CylinderShape()
{ }

const Vector3& CylinderShape::GetHalfExtents() const
{
	return m_HalfExtents;
}

CylinderShape* CylinderShape::Cast(iShape* shape)
{
	return dynamic_cast<CylinderShape*>(shape);
}