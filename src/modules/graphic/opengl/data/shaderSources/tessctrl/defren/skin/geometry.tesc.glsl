//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

layout(vertices=3) out;

in vec2 vTCSTCColor[];
in vec2 vTCSTCNormal[];
in vec2 vTCSTCReflectivity[];
in vec2 vTCSTCEmissivity[];
in vec2 vTCSTCRefractionDistort[];
in vec2 vTCSTCAO[];
in vec3 vTCSNormal[];
in vec3 vTCSTangent[];
in vec3 vTCSBitangent[];
in float vTCSHTMask[];
// in float vTCSRenderCondition[];

out vec2 vTESTCColor[];
out vec2 vTESTCNormal[];
out vec2 vTESTCReflectivity[];
out vec2 vTESTCEmissivity[];
out vec2 vTESTCRefractionDistort[];
out vec2 vTESTCAO[];
out vec3 vTESNormal[];
out vec3 vTESTangent[];
out vec3 vTESBitangent[];
out float vTESHTMask[];
// out float vTESRenderCondition[];

void main(void){
	if(gl_InvocationID == 0){
		vec3 edgeLen;
		edgeLen.x = length(gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz);
		edgeLen.y = length(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
		edgeLen.z = length(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
		
		vec3 splits = clamp(floor(edgeLen / vec3(0.01)), vec3(1.0), vec3(32.0));
		
		gl_TessLevelOuter[0] = splits.x;
		gl_TessLevelOuter[1] = splits.y;
		gl_TessLevelOuter[2] = splits.z;
		
		gl_TessLevelInner[0] = floor(dot(splits, vec3(1.0 / 3.0)));
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	vTESTCColor[gl_InvocationID] = vTCSTCColor[gl_InvocationID];
	vTESTCNormal[gl_InvocationID] = vTCSTCNormal[gl_InvocationID];
	vTESTCReflectivity[gl_InvocationID] = vTCSTCReflectivity[gl_InvocationID];
	vTESTCEmissivity[gl_InvocationID] = vTCSTCEmissivity[gl_InvocationID];
	vTESTCRefractionDistort[gl_InvocationID] = vTCSTCRefractionDistort[gl_InvocationID];
	vTESTCAO[gl_InvocationID] = vTCSTCAO[gl_InvocationID];
	vTESNormal[gl_InvocationID] = vTCSNormal[gl_InvocationID];
	vTESTangent[gl_InvocationID] = vTCSTangent[gl_InvocationID];
	vTESBitangent[gl_InvocationID] = vTCSBitangent[gl_InvocationID];
	vTESHTMask[gl_InvocationID] = vTCSHTMask[gl_InvocationID];
	// vTESRenderCondition[gl_InvocationID] = vTCSRenderCondition[gl_InvocationID];
}
