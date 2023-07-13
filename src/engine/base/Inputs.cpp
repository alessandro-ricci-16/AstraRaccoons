#include "../../../headers/engine/base/Inputs.hpp"

GLFWwindow* Inputs::window = nullptr;
void Inputs::setWindow(GLFWwindow* w) {
    window = w;
}

void Inputs::getSixAxis(glm::vec3& mov, glm::vec3& rot, bool& fire) {
    glm::vec3 m, r;
    m = r = glm::vec3(0.0f);

    r.x = -glfwGetKey(window, GLFW_KEY_UP) + glfwGetKey(window, GLFW_KEY_DOWN);
    r.y = -glfwGetKey(window, GLFW_KEY_LEFT) + glfwGetKey(window, GLFW_KEY_RIGHT);
    r.z = -glfwGetKey(window, GLFW_KEY_E) + glfwGetKey(window, GLFW_KEY_Q);

    m.x = -glfwGetKey(window, GLFW_KEY_A) + glfwGetKey(window, GLFW_KEY_D);
    m.y = -glfwGetKey(window, GLFW_KEY_F) + glfwGetKey(window, GLFW_KEY_R);
    m.z = -glfwGetKey(window, GLFW_KEY_S) + glfwGetKey(window, GLFW_KEY_W);

    /*if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        r.y = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        r.y = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP)) {
        r.x = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        r.x = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        r.z = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        r.z = -1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        m.x = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        m.x = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        m.z = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        m.z = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_R)) {
        m.y = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_F)) {
        m.y = -1.0f;
    }*/

    fire = glfwGetKey(window, GLFW_KEY_SPACE) |
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    handleGamePad(GLFW_JOYSTICK_1, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_2, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_3, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_4, m, r, fire);

    mov = m;
    rot = r;
}

void Inputs::handleGamePad(int id, glm::vec3& m, glm::vec3& r,
    bool& fire) {
    const float deadZone = 0.1f;

    if (glfwJoystickIsGamepad(id)) {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(id, &state)) {
            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]) > deadZone) {
                m.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
            }
            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]) > deadZone) {
                m.z -= state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
            }
            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]) > deadZone) {
                m.y -= state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
            }
            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]) > deadZone) {
                m.y += state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
            }

            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]) > deadZone) {
                r.y += state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
            }
            if (fabs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]) > deadZone) {
                r.x += state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
            }
            r.z += state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] ? 1.0f : 0.0f;
            r.z -=
                state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] ? 1.0f : 0.0f;
            fire = fire | (bool)state.buttons[GLFW_GAMEPAD_BUTTON_A] |
                (bool)state.buttons[GLFW_GAMEPAD_BUTTON_B];
        }
    }
}