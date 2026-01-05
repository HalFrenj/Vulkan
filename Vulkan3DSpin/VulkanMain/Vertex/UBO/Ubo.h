#include <glm/glm.hpp>
#include <vector>

struct UBO
{
    glm::mat4 mvp;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};