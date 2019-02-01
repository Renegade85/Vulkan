#include "EngineCore.h"
#include <iostream>
#include <string>

/**
 * @group pub_intf Public interface
 * @{
 */
EngineCore::EngineCore()
{
    m_windowWidth = WINDOW_DEFAULT_WIDTH;
    m_windowHeight = WINDOW_DEFAULT_HEIGHT;
}

void EngineCore::run(void)
{
    init();
    mainLoop();
    cleanup();
}

VkBool32 EngineCore::isRequiredExtensionAvailable(std::vector<const char *> & requiredExtensions)
{
    std::vector<VkExtensionProperties> availableExtensions;
    getInstanceExtensionProperties(nullptr, availableExtensions);
    VkBool32 found = VK_FALSE;

    /* loop through every available extension property and compare it with required extensions */
    for (const char * & extensionName : requiredExtensions)
    {
        for (const VkExtensionProperties & extension : availableExtensions)
        {
            int comp_result = std::string(extensionName).compare(extension.extensionName);
            if (0 == comp_result)
            {
                found = VK_TRUE;
                break;
            }
        }

        if (false == found)
        {
            std::cout << "Extension not supported: " << extensionName << std::endl;
            return VK_FALSE;
        }
        found = VK_FALSE;
    }

    return VK_TRUE;
}
/** @} */ /* end of group pub_intf */

/**
 * @group priv_intf Private interface
 * @{
 */
/**
 * @group win_sup Window creation support functions
 * @{
 */
void EngineCore::setWindowDimensions(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

void EngineCore::setWindowTitle(const char * new_title)
{
    m_windowTitle = new_title;
}
/** @} */ /* end of group win_sup */

/**
 * @group main_logic Engine main logic
 * @{
 */
VkBool32 EngineCore::init(void)
{
    initCore();
    m_window = createWindow(m_windowWidth, m_windowHeight, m_windowTitle);

    std::vector<const char *> requiredLayers(0);
    std::vector<const char *> requiredExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, CORE_PLATFORM_SURFACE_EXTENSION_NAME};
    VkBool32 isExtensionSupported = isRequiredExtensionAvailable(requiredExtensions);

    if (VK_FALSE == isExtensionSupported)
    {
        return VK_FALSE;
    }

    std::vector<VkLayerProperties> layerProperties;
    getInstanceLayerProperties(layerProperties);

    std::vector<VkExtensionProperties> extensionProperties;
    getInstanceExtensionProperties(nullptr, extensionProperties);


    for (uint32_t i = 0u; i < layerProperties.size(); i++)
    {
        std::cout << layerProperties[i].layerName << std::endl;
    }

    for (uint32_t i = 0u; i < extensionProperties.size(); i++)
    {
        std::cout << extensionProperties[i].extensionName << std::endl;
    }

    m_instance = initVulkan(requiredLayers, requiredExtensions);
    m_surface = createSurface(m_instance, m_window);

    return VK_TRUE;
}

void EngineCore::mainLoop(void)
{
    while (false == glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void EngineCore::cleanup(void)
{
    destroyWindow(m_window);
    deinitCore();
}
/** @} */ /* end of group main_logic */
/** @} */ /* end of group priv_inft */