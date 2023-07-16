#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_INPUTS_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_INPUTS_HPP_
#include "includes.hpp"

class Inputs {
	private:
		static GLFWwindow* window;
	public:
		static void getSixAxis(glm::vec3& m, glm::vec3& r, bool& fire);
		static void handleGamePad(int id, glm::vec3& m, glm::vec3& r, bool& fire);
		static bool isKeyPressed(int keymask);
		static void setWindow(GLFWwindow* w);
};

#endif //__DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_INPUTS_HPP_