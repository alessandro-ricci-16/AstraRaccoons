#include "../../../headers/engine/base/Inputs.hpp"

GLFWwindow* Inputs::window = nullptr;
void Inputs::setWindow(GLFWwindow* w) {
    window = w;
}

void Inputs::getSixAxis(glm::vec3& mov, glm::vec3& rot, bool& fire) {
    glm::vec3 m, r;
    m = r = glm::vec3(0.0f);

    r.x = -isKeyPressed(GLFW_KEY_UP) + isKeyPressed(GLFW_KEY_DOWN);
    r.y = -isKeyPressed(GLFW_KEY_LEFT) + isKeyPressed(GLFW_KEY_RIGHT);
    r.z = -isKeyPressed(GLFW_KEY_E) + isKeyPressed(GLFW_KEY_Q);

    m.x = -isKeyPressed(GLFW_KEY_A) + isKeyPressed(GLFW_KEY_D);
    m.y = -isKeyPressed(GLFW_KEY_F) + isKeyPressed(GLFW_KEY_R);
    m.z = -isKeyPressed(GLFW_KEY_S) + isKeyPressed(GLFW_KEY_W);

    // Rotate with mouse input
    static double old_xpos = 0, old_ypos = 0;
    double xpos, ypos;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window, &xpos, &ypos);
    double m_dx = glm::clamp(xpos - old_xpos, -MOUSE_RES, MOUSE_RES);
    double m_dy = glm::clamp(ypos - old_ypos, -MOUSE_RES, MOUSE_RES);
    old_xpos = xpos;
    old_ypos = ypos;

    // glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    if (fabs(m_dx) > 0.01f || fabs(m_dy) > 0.01f) {
        r.y = m_dx / MOUSE_RES;
        r.x = m_dy / MOUSE_RES;
    }

    fire = isKeyPressed(GLFW_KEY_SPACE) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    handleGamePad(GLFW_JOYSTICK_1, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_2, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_3, m, r, fire);
    handleGamePad(GLFW_JOYSTICK_4, m, r, fire);

    mov = m;
    rot = r;
}

bool Inputs::isKeyPressed(int keymask) {
    return glfwGetKey(window, keymask);
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
                   (bool)state.buttons[GLFW_GAMEPAD_BUTTON_B] |
                   fabs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]) > deadZone;
        }
    }
}
