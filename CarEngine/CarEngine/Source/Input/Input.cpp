#include "Input.h"
#include "Utils/Math.h"

namespace CE
{
    Input::Input(GLFWwindow* window) : window(window)
    {
        glfwSetWindowUserPointer(window, &inputEvents);

        glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
        {
            InputEvents& input = *(InputEvents*)glfwGetWindowUserPointer(glfwWindow);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    input.onKeyPressed.broadcast(key);
                    break;
                }
                case GLFW_RELEASE:
                {
                    input.onKeyReleased.broadcast(key);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
        {
            InputEvents& input = *(InputEvents*)glfwGetWindowUserPointer(glfwWindow);

            glm::dvec2 cursorPositionDouble;
            glfwGetCursorPos(glfwWindow, &cursorPositionDouble.x, &cursorPositionDouble.y);
            glm::vec2 cursorPosition = cursorPositionDouble;

            switch (action)
            {
                case GLFW_PRESS:
                {
                    input.onMouseButtonPressed.broadcast(button, cursorPosition);
                    break;
                }
                case GLFW_RELEASE:
                {
                    input.onMouseButtonReleased.broadcast(button, cursorPosition);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, double xPos, double yPos)
        {
            InputEvents& input = *(InputEvents*)glfwGetWindowUserPointer(glfwWindow);
            input.onMouseMoved.broadcast({xPos, yPos});
        });
    }

    bool Input::isKeyPressed(int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool Input::isKeyReleased(int key)
    {
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }

    Event<Input, int>& Input::getKeyPressedEvent()
    {
        return inputEvents.onKeyPressed;
    }

    Event<Input, int>& Input::getKeyReleasedEvent()
    {
        return inputEvents.onKeyReleased;
    }

    bool Input::isMouseButtonPressed(int button)
    {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    bool Input::isMouseButtonReleased(int button)
    {
        return glfwGetMouseButton(window, button) == GLFW_RELEASE;
    }

    Event<Input, int, glm::vec2>& Input::getMouseButtonPressedEvent()
    {
        return inputEvents.onMouseButtonPressed;
    }

    Event<Input, int, glm::vec2>& Input::getMouseButtonReleasedEvent()
    {
        return inputEvents.onMouseButtonReleased;
    }

    glm::vec2 Input::getMouseCursorPosition()
    {
        glm::dvec2 cursorPosition;
        glfwGetCursorPos(window, &cursorPosition.x, &cursorPosition.y);
        return cursorPosition;
    }

    Event<Input, glm::vec2>& Input::getMouseMovedEvent()
    {
        return inputEvents.onMouseMoved;
    }

    bool Input::isControllerConnected()
    {
        return glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1);
    }

    float Input::getControllerAxis(int axis)
    {
        GLFWgamepadstate state;

        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
            return state.axes[axis];

        return 0.f;
    }

    float Input::getControllerTriggerAxis(int axis)
    {
        GLFWgamepadstate state;

        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        {
            float value = state.axes[axis];
            return Math::inverseLerp(-1.f, 1.f, value);
        }

        return 0.f;
    }

    bool Input::isControllerButtonPressed(int button)
    {
        GLFWgamepadstate state;

        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
            return state.buttons[button] == GLFW_PRESS;

        return false;
    }

    bool Input::isControllerButtonReleased(int button)
    {
        GLFWgamepadstate state;

        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
            return state.buttons[button] == GLFW_RELEASE;

        return false;
    }

    void Input::quitGame()
    {
        glfwSetWindowShouldClose(window, true);
    }
}