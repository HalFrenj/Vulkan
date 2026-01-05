#include "VulkanMain/Main/Main.h"
#include "VulkanMain/VulkanDebug/VulkanDebug.h"

void VkMain::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void VkMain::InitVulkan() {
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();

    // 1. 레이아웃을 먼저 정의
    CreateDescriptorSetLayout();

    CreateGraphicsPipeline();
    CreateCommandPool(); // 버퍼 생성 시 커맨드가 필요할 수 있으므로 미리 생성

    // 2. 실제 리소스(버퍼) 생성 단계 (중요!)
    // vertex 데이터 초기화는 생성 함수 이전에 하는 것이 안전합니다.
    vertices = {
        {{ 0.0f, -0.5f,  0.0f}, {1.0f, 0.0f, 0.0f}}, // 위쪽 꼭짓점 (빨강)
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // 바닥 1 (초록)
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 바닥 2 (파랑)
        {{ 0.0f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}  // 바닥 3 (흰색)
    };
    CreateVertexBuffer();

    // 이 함수가 정의되어 있는지 확인하고 여기서 호출해야 합니다!
    CreateUniformBuffers();

    // 3. 리소스가 다 만들어진 후 디스크립터 연결
    CreateDescriptorPool();
    CreateDescriptorSets();

    CreateFramebuffers();
    CreateSyncObjects();
    CreateCommandBuffer();
}
/*
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateDescriptorPool();
    CreateDescriptorSetLayout();
    CreateDescriptorSets();
    CreateFramebuffers();
    CreateCommandPool();
    vertices = {
        {{ 0.0f, -0.5f,  0.0f}, {1.0f, 0.0f, 0.0f}}, // 위쪽 꼭짓점 (빨강)
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // 바닥 1 (초록)
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 바닥 2 (파랑)
        {{ 0.0f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}  // 바닥 3 (흰색)
    };
    CreateCommandBuffer();
    CreateVertexBuffer();
    CreateSyncObjects();
*/

void VkMain::MainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        DrawFrame();
    }

    vkDeviceWaitIdle(device);
}

void VkMain::Cleanup() {
    vkDeviceWaitIdle(device);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphore[i], nullptr);
        vkDestroyFence(device, inFlightFence[i], nullptr);
    }
    vkDestroyCommandPool(device, commandPool, nullptr);

    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);

    if (VkDebug::enableValidationLayers) {
        VkDebug::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}