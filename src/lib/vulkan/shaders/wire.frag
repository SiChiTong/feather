#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inId;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outSelection;

void main() 
{
    outColor = vec4(inColor, 1.0);
    outSelection = vec4(inId,0);
}
