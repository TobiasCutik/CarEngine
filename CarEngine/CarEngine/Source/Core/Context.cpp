#include <stdexcept>
#include "Context.h"
#include "glad/glad.h"
#include "Utils/Loader.h"

namespace CE
{
    Context::Context(int windowWidth, int windowHeight, const char*& windowTitle, int antiAliasingSamplePoints, bool bUseFullscreen)
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, antiAliasingSamplePoints);

        GLFWmonitor* monitor = nullptr;
        if (bUseFullscreen)
            monitor = glfwGetPrimaryMonitor();

        window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, monitor, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(window);

        // Load GL Extensions (GLAD)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("Failed to initialize GLAD");

        if (glGetError() != GL_NO_ERROR)
            throw std::runtime_error("GL Error");

        glfwSetTime(1.f / 60.f);

        input = std::make_shared<Input>(window);
        world = std::make_unique<World>(input);
    }

    Context::~Context()
    {
        glfwTerminate();
    }

    void Context::run()
    {
        world->start();

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            world->tick(width, height);

            glfwPollEvents();
            glfwSwapBuffers(window);
        }

        world->shutdown();
    }
}