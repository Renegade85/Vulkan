#ifndef _VULKAN_CORE_GUARD
#define _VULKAN_CORE_GUARD 1

//#define VK_USE_PLATFORM_WIN32_KHR
//#define VK_USE_PLATFORM_WAYLAND_KHR
//#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
//#define VK_USE_PLATFORM_MACOS_MVK

#if defined VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32 
#elif defined VK_USE_PLATFORM_XLIB_KHR || defined VK_USE_PLATFORM_XCB_KHR
#define GLFW_EXPOSE_NATIVE_X11
#elif defined VK_USE_PLATFORM_WAYLAND_KHR
#define GLFW_EXPOSE_NATIVE_WAYLAND
#endif

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <vector>

#if defined VK_USE_PLATFORM_WIN32_KHR
#define XPlatformLoadLibrary                    LoadLibraryA
#define XPlatformCloseLib                       FreeLibrary
#define XPlatformLibHandle                      HMODULE
#define XPlatformLoadProc                       GetProcAddress
#define VULKAN_LIBRARY_NAME                     "vulkan-1.dll"
#define CORE_PLATFORM_SURFACE_EXTENSION_NAME    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#define PFN_vkPlatformCreateSurface             PFN_vkCreateWin32SurfaceKHR
#define CORE_PLATFORM_CREATE_SURFACE_NAME       "vkCreateWin32SurfaceKHR"
#elif defined VK_USE_PLATFORM_WAYLAND_KHR || defined VK_USE_PLATFORM_XCB_KHR || defined VK_USE_PLATFORM_XLIB_KHR
#include <dlfcn.h>
#define XPlatformLoadLibrary(path)              dlopen((path), RTLD_LAZY | RTLD_LOCAL)
#define XPlatformCloseLib                       dlclose
#define XPlatformLibHandle                      void *
#define XPlatformLoadProc                       dlsym
#define VULKAN_LIBRARY_NAME                     "libvulkan.so.1"
#if defined VK_USE_PLATFORM_WAYLAND_KHR
#define CORE_PLATFORM_SURFACE_EXTENSION_NAME    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#define PFN_vkPlatformCreateSurface             PFN_vkCreateWaylandSurfaceKHR
#define CORE_PLATFORM_CREATE_SURFACE_NAME       "vkCreateWaylandSurfaceKHR"
#elif defined VK_USE_PLATFORM_XLIB_KHR
#define CORE_PLATFORM_SURFACE_EXTENSION_NAME    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#define PFN_vkPlatformCreateSurface             PFN_vkCreateXlibSurfaceKHR
#define CORE_PLATFORM_CREATE_SURFACE_NAME       "vkCreateXlibSurfaceKRH"
#elif defined VK_USE_PLATFORM_XCB_KHR
#define CORE_PLATFORM_SURFACE_EXTENSION_NAME    VK_KHR_XCB_SURFACE_EXTENSION_NAME
#define PFN_vkPlatformCreateSurface             PFN_vkCreateXcbSurfaceKHR
#define CORE_PLATFORM_CREATE_SURFACE_NAME       "vkCreateXcbSurfaceKRH"
#endif
#elif defined VK_USE_PLATFORM_MACOS_MVK
#define XPlatformLoadLibrary                    dlopen
#define XPlatformCloseLib                       dlclose
#define XPlatformLibHandle                      void *
#define XPlatformLoadProc                       dlsym
#define VULKAN_LIBRARY_NAME                     "libvulkan.dylib"
#define CORE_PLATFORM_SURFACE_EXTENSION_NAME    VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#define PFN_vkPlatformCreateSurface             PFN_vkCreateMacOSSurfaceMVK
#define CORE_PLATFORM_CREATE_SURFACE_NAME       "vkCreateMacOSSurfaceMVK"
#endif

class Core
{
    public:

    private:
    XPlatformLibHandle m_library = nullptr;
    uint32_t m_vulkanApiVersion = 0u;

    std::vector<const char *> * m_layerNames = nullptr;

    PFN_vkGetInstanceProcAddr                m_vkGetInstanceProcAddress             = nullptr;

    /* Instance level functions */
    PFN_vkEnumerateInstanceVersion           m_vkEnumerateInstanceVersion           = nullptr;
    PFN_vkEnumerateInstanceExtensionProperties m_vkEnumerateInstanceExtensionProperties = nullptr;
    PFN_vkEnumerateInstanceLayerProperties   m_vkEnumerateInstanceLayerProperties   = nullptr;
    PFN_vkCreateInstance                     m_vkCreateInstance                     = nullptr;

    PFN_vkDestroyInstance                    m_vkDestroyInstance                    = nullptr;
    PFN_vkPlatformCreateSurface              m_vkPlatformCreateSurface              = nullptr;   

    void closeLibrary(XPlatformLibHandle * lib);

    protected:
    void initCore(void);
    void deinitCore(void);
    GLFWwindow * createWindow(int width, int height, const char * title);
    void destroyWindow(GLFWwindow * window);

    VkInstance initVulkan(std::vector<const char *> & requiredLayers, std::vector<const char *> & requiredExtensions);
    uint32_t getVulkanApiVersion(void) { return m_vulkanApiVersion;}
    void setEnabledLayerNames(std::vector<const char *> & layerNames);
    void getInstanceExtensionProperties(const char * pLayerName, std::vector<VkExtensionProperties> & pProperties);
    void getInstanceLayerProperties(std::vector<VkLayerProperties> & pProperties);
    VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow * handle);
};

#endif /* header guard */
