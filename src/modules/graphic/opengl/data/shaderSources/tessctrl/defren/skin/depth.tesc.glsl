//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

layout(vertices=3) out;

#include "shared/interface/skin/tessellation_control.glsl"

void main(void){
	tessellationShaderDefaultPosition();
	tessellationShaderDefaultOutputs();
}
