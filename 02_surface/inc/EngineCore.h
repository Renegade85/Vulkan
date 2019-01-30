#ifndef _ENGINE_CORE_GUARD
#define _ENGINE_CORE_GUARD

#include "Core.h"

#define WINDOW_DEFAULT_WIDTH    800
#define WINDOW_DEFAULT_HEIGHT   600



class EngineCore : public Core
{
    private:
    /* Window related attributes */
    GLFWwindow * m_window       = nullptr;
    int          m_windowWidth  = 800;
    int          m_windowHeight = 600;
    const char * m_windowTitle  = ""; 
    VkSurfaceKHR m_surface      = VK_NULL_HANDLE;

    VkInstance   m_instance     = VK_NULL_HANDLE;

    VkBool32 init(void);
    void mainLoop(void);
    void cleanup(void);

    public:
    /* Constructors */
    EngineCore(void);

    /* Window functions */
    void setWindowDimensions(int width, int height);
    void setWindowTitle(const char * newTitle);

    VkBool32 isRequiredExtensionAvailable(std::vector<const char *> & requiredExtensions);
    void run(void);
};

#endif /* Header guard */