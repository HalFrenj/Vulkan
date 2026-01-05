#version 450

// Vertex Attributes
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

// Output to Fragment Shader
layout(location = 0) out vec3 fragColor;

// Dynamic UBO (Binding 0)
// API에서 VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC으로 설정됨
layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 mvp;
} ubo;

void main() {
    // 좌표 변환: MVP 행렬을 정점에 곱함
    gl_Position = ubo.mvp * vec4(inPosition, 1.0);
    
    // 색상 전달
    fragColor = inColor;
}