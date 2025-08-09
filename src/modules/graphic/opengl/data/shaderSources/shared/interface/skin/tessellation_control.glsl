// usage:
// #include "shared/interface/skin/tessellation_control.glsl"


VARYING_BIND(0) in vec4 vTCSTexCoords1[]; // color, colorTintMask
VARYING_BIND(1) in vec4 vTCSTexCoords2[]; // normal, reflectivity
VARYING_BIND(2) in vec4 vTCSTexCoords3[]; // emissivity, refractionDistort
VARYING_BIND(3) in vec2 vTCSTexCoords4[]; // ao
VARYING_BIND(4) in vec4 vTCSNormalHTMask[]; // normal, HTMask
VARYING_BIND(5) in vec4 vTCSTangentFadeZ[]; // tangent, fadeZ
VARYING_BIND(6) in vec3 vTCSBitangent[];
VARYING_BIND(7) in vec3 vTCSReflectDir[];
VARYING_BIND(8) in vec3 vTCSClipCoord[];
VARYING_BIND(9) in vec3 vTCSSkinClipCoord[];
VARYING_BIND(10) in vec3 vTCSPosition[];
VARYING_BIND(11) flat in ivec4 vTCSIndicesFlags[]; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) in vec3 vTCSParticle0[]; // size, emissivity, rotation
VARYING_BIND(13) in vec4 vTCSParticle1[]; // red, green, blue, transparency
VARYING_BIND(14) flat in int vTCSParticleSheetCount[];


VARYING_BIND(0) out vec4 vTESTexCoords1[]; // color, colorTintMask
VARYING_BIND(1) out vec4 vTESTexCoords2[]; // normal, reflectivity
VARYING_BIND(2) out vec4 vTESTexCoords3[]; // emissivity, refractionDistort
VARYING_BIND(3) out vec2 vTESTexCoords4[]; // ao
VARYING_BIND(4) out vec4 vTESNormalHTMask[]; // normal, HTMask
VARYING_BIND(5) out vec4 vTESTangentFadeZ[]; // tangent, fadeZ
VARYING_BIND(6) out vec3 vTESBitangent[];
VARYING_BIND(7) out vec3 vTESReflectDir[];
VARYING_BIND(8) out vec3 vTESClipCoord[];
VARYING_BIND(9) out vec3 vTESSkinClipCoord[];
VARYING_BIND(10) out vec3 vTESPosition[];
VARYING_BIND(11) flat out ivec4 vTESIndicesFlags[]; // doubleSided, SPBIndex, SPBFlags, layer

VARYING_BIND(12) out vec3 vTESParticle0[]; // size, emissivity, rotation
VARYING_BIND(13) out vec4 vTESParticle1[]; // red, green, blue, transparency
VARYING_BIND(14) flat out int vTESParticleSheetCount[];


// default outputs
void tessellationShaderDefaultPosition(){
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
}

void tessellationShaderDefaultOutputs(){
	vTESTexCoords1[gl_InvocationID] = vTCSTexCoords1[gl_InvocationID];
	vTESTexCoords2[gl_InvocationID] = vTCSTexCoords2[gl_InvocationID];
	vTESTexCoords3[gl_InvocationID] = vTCSTexCoords3[gl_InvocationID];
	vTESTexCoords4[gl_InvocationID] = vTCSTexCoords4[gl_InvocationID];
	vTESNormalHTMask[gl_InvocationID] = vTCSNormalHTMask[gl_InvocationID];
	vTESTangentFadeZ[gl_InvocationID] = vTCSTangentFadeZ[gl_InvocationID];
	vTESBitangent[gl_InvocationID] = vTCSBitangent[gl_InvocationID];
	vTESReflectDir[gl_InvocationID] = vTCSReflectDir[gl_InvocationID];
	vTESClipCoord[gl_InvocationID] = vTCSClipCoord[gl_InvocationID];
	vTESSkinClipCoord[gl_InvocationID] = vTCSSkinClipCoord[gl_InvocationID];
	vTESPosition[gl_InvocationID] = vTCSPosition[gl_InvocationID];
	vTESIndicesFlags[gl_InvocationID] = vTCSIndicesFlags[gl_InvocationID];
	
	vTESParticle0[gl_InvocationID] = vTCSParticle0[gl_InvocationID];
	vTESParticle1[gl_InvocationID] = vTCSParticle1[gl_InvocationID];
	vTESParticleSheetCount[gl_InvocationID] = vTCSParticleSheetCount[gl_InvocationID];
}
