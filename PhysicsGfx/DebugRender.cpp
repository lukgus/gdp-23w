#include "DebugRender.h"

#include "DebugRender.h"

#include "GL.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RGB(v) v.r, v.g, v.b

namespace gdp
{
	void DrawLine(const glm::vec3& color, const glm::vec3& a, const glm::vec3& b)
	{
		glBegin(GL_LINES);
		glColor3f(RGB(color));
		glVertex3f(RGB(a));
		glVertex3f(RGB(b));
		glEnd();
	}
}