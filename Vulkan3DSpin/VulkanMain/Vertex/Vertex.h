#pragma once
#include <array> 
#include <vulkan/vulkan.h> 
#include <glm/glm.hpp>    

struct Vertex 
{
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>
        GetAttributeDescriptions();
};