#pragma once
#include <GLFW/glfw3.h>
#include "glm/vec2.hpp"
#include "Core/Event.h"

namespace CE
{
    class Input {

    public:
        explicit Input(GLFWwindow* window);

        bool isKeyPressed(int key);
        bool isKeyReleased(int key);

        Event<Input, int>& getKeyPressedEvent();
        Event<Input, int>& getKeyReleasedEvent();

        bool isMouseButtonPressed(int button);
        bool isMouseButtonReleased(int button);

        Event<Input, int, glm::vec2>& getMouseButtonPressedEvent();
        Event<Input, int, glm::vec2>& getMouseButtonReleasedEvent();

        glm::vec2 getMouseCursorPosition();

        Event<Input, glm::vec2>& getMouseMovedEvent();

        static bool isControllerConnected();

        // returns axis value between -1 and 1
        static float getControllerAxis(int axis);
        // returns axis value between 0 and 1
        static float getControllerTriggerAxis(int axis);

        static bool isControllerButtonPressed(int button);
        static bool isControllerButtonReleased(int button);

        void quitGame();

    private:
        struct InputEvents
        {
            Event<Input, int> onKeyPressed;
            Event<Input, int> onKeyReleased;

            Event<Input, int, glm::vec2> onMouseButtonPressed;
            Event<Input, int, glm::vec2> onMouseButtonReleased;

            Event<Input, glm::vec2> onMouseMoved;
        };

        InputEvents inputEvents;

        GLFWwindow* window = nullptr;
    };
}