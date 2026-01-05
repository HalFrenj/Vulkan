#version 450

// Input from Vertex Shader
layout(location = 0) in vec3 fragColor;

// Final Color Output
layout(location = 0) out vec4 outColor;

void main() {
    // 보간된 색상에 알파 값 1.0(불투명)을 추가하여 출력
    outColor = vec4(fragColor, 1.0);
}