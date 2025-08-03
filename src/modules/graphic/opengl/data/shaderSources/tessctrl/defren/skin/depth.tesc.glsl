//#extension GL_ARB_tessellation_shader : enable

#include "shared/preamble.glsl"

layout( vertices=3 ) out;

in vec2 vTCSTCColor[];
in float vTCSHTMask[];
in vec3 vTCSNormal[];
in vec3 vTCSTangent[];
in vec3 vTCSBitangent[];

out vec2 vTESTCColor[];
out float vTESHTMask[];
out vec3 vTESNormal[];
out vec3 vTESTangent[];
out vec3 vTESBitangent[];

void main(void){
	if(gl_InvocationID == 0){
		vec3 edgeLen;
		edgeLen.x = length(gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz);
		edgeLen.y = length(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
		edgeLen.z = length(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
		
		vec3 splits = clamp(floor(edgeLen / vec3(0.01)), vec3(1), vec3(32));
		
		gl_TessLevelOuter[0] = splits.x;
		gl_TessLevelOuter[1] = splits.y;
		gl_TessLevelOuter[2] = splits.z;
		
		gl_TessLevelInner[0] = floor(dot(splits, vec3(1.0 / 3.0)));
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	vTESTCColor[gl_InvocationID] = vTCSTCColor[gl_InvocationID];
	vTESHTMask[gl_InvocationID] = vTCCSHTMask[gl_InvocationID];
	vTESNormal[gl_InvocationID] = vTCSNormal[gl_InvocationID];
	vTESTangent[gl_InvocationID] = vTCSTangent[gl_InvocationID];
	vTESBitangent[gl_InvocationID] = vTCSBitangent[gl_InvocationID];
}
