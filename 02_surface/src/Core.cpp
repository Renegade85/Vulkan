#include "Core.h"
#include <vector>
#include <stdexcept>

void Core::initCore(void)
{
    XPlatformLibHandle lib = nullptr;

    if (GLFW_FALSE == glfwInit())
    {
        /* TODO replace with custom exception??? */
        throw std::logic_error("");
    }

    lib = XPlatformLoadLibrary(VULKAN_LIBRARY_NAME);
    if (nullptr == lib)
    {
        /* TODO replace with custom exception??? */
        throw std::logic_error("");
    }

    m_library                               = lib;
    m_vkGetInstanceProcAddress              = (PFN_vkGetInstanceProcAddr) XPlatformLoadProc(m_library, "vkGetInstanceProcAddr");
    m_vkEnumerateInstanceVersion            = (PFN_vkEnumerateInstanceVersion) m_vkGetInstanceProcAddress(nullptr, "vkEnumerateInstanceVersion");
    m_vkEnumerateInstanceExtensionProperties= (PFN_vkEnumerateInstanceExtensionProperties) m_vkGetInstanceProcAddress(nullptr, "vkEnumerateInstanceExtensionProperties");
    m_vkEnumerateInstanceLayerProperties    = (PFN_vkEnumerateInstanceLayerProperties) m_vkGetInstanceProcAddress(nullptr, "vkEnumerateInstanceLayerProperties");
    m_vkCreateInstance                      = (PFN_vkCreateInstance) m_vkGetInstanceProcAddress(nullptr, "vkCreateInstance");

    if (nullptr == m_vkEnumerateInstanceVersion)
    {
        m_vulkanApiVersion = VK_API_VERSION_1_0;
    }
    else
    {
        m_vulkanApiVersion = VK_API_VERSION_1_1;
    }
}

void Core::deinitCore(void)
{
    closeLibrary(&m_library);
    glfwTerminate();
}

void Core::closeLibrary(XPlatformLibHandle * lib)
{
    if (nullptr != lib)
    {
        XPlatformCloseLib(*lib);
        lib = nullptr;
    }
}

GLFWwindow * Core::createWindow(int width, int height, const char * title)
{
    GLFWwindow * window;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (nullptr == window)
    {
        glfwTerminate();
        return window;
    }

    glfwMakeContextCurrent(window);
    
    return window;
}

void Core::destroyWindow(GLFWwindow * window)
{
    if (nullptr != window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

VkInstance Core::initVulkan(std::vector<const char *> & requiredLayers, std::vector<const char *> & requiredExtensions)
{
    /* TODO pass vector as reference or pointer? Investigate pros/cons/preferred approach */
    VkInstance vulkanInstance;

    /* TODO provide as input parameter??? */
    VkApplicationInfo ai;
    ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ai.pNext = nullptr;
    ai.pApplicationName = "";
    ai.applicationVersion = 0u;
    ai.pEngineName = "";
    ai.engineVersion = 0u;
    ai.apiVersion = getVulkanApiVersion();

    const char * extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
    VkInstanceCreateInfo ici;
    ici.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pNext                   = nullptr;
    ici.flags                   = 0;
    ici.pApplicationInfo        = &ai;
    ici.enabledLayerCount       = requiredLayers.size();
    ici.ppEnabledLayerNames     = requiredLayers.data();
    ici.enabledExtensionCount   = requiredExtensions.size();
    ici.ppEnabledExtensionNames = requiredExtensions.data();

    m_vkCreateInstance(&ici, nullptr, &vulkanInstance);

    m_vkDestroyInstance = (PFN_vkDestroyInstance) m_vkGetInstanceProcAddress(vulkanInstance, "vkDestroyInstance");
    m_vkPlatformCreateSurface = (PFN_vkPlatformCreateSurface) m_vkGetInstanceProcAddress(vulkanInstance, CORE_PLATFORM_CREATE_SURFACE_NAME);

    return vulkanInstance;
}

void Core::setEnabledLayerNames(std::vector<const char *> & layerNames)
{
    m_layerNames = &layerNames;
}

void Core::getInstanceExtensionProperties(const char * pLayerName, std::vector<VkExtensionProperties> & pProperties)
{
    uint32_t propertiesCount;
    m_vkEnumerateInstanceExtensionProperties(pLayerName, &propertiesCount, nullptr);
    
    pProperties.resize(propertiesCount);
    m_vkEnumerateInstanceExtensionProperties(pLayerName, &propertiesCount, pProperties.data());
}

void Core::getInstanceLayerProperties(std::vector<VkLayerProperties> & pProperties)
{
    uint32_t propertiesCount;
    m_vkEnumerateInstanceLayerProperties(&propertiesCount, nullptr);

    pProperties.resize(propertiesCount);
    m_vkEnumerateInstanceLayerProperties(&propertiesCount, pProperties.data());
}

VkSurfaceKHR Core::createSurface(VkInstance instance, GLFWwindow * handle)
{
    VkSurfaceKHR surface = 0u;
    #if defined VK_USE_PLATFORM_WIN32_KHR
    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = 0;
    surfaceInfo.hinstance = GetModuleHandleA(nullptr);
    surfaceInfo.hwnd = glfwGetWin32Window(handle);
    #elif defined VK_USE_PLATFORM_WAYLAND_KHR
    VkWaylandSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = 0;
    surfaceInfo.display = glfwGetWaylandDisplay();
    surfaceInfo.surface = glfwGetWaylandWindow(m_window);
    #elif defined VK_USE_PLATFORM_XCB_KHR
    VkXcbSurfaceCreateInfoKHR = surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = 0;
    surfaceInfo.connection = XGetXCBConnection(glfwGetX11Display());
    surfaceInfo.window = glfwGetX11Window(m_window);
    #elif defined VK_USE_PLATFORM_XLIB_KHR
    VkXlibSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = 0;
    surfaceInfo.display = glfwGetX11Display();
    surfaceInfo.window = glfwGetX11Window();
    #elif defined VK_USE_PLATFORM_MACOS_MVK
    #endif

    m_vkPlatformCreateSurface(instance, &surfaceInfo, nullptr, &surface);

    return surface;
}
