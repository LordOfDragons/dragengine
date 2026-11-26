// Forward rendering per-light data SSBO.
// Usage:
// #include "shared/defren/skin/ssbo_forward_lights.glsl"
//
// Provides per-light data for low fill rate forward rendering.
// Maximum 9 lights: 1 sky light + 8 spot/point lights.
// Light types: 0 = sky (directional), 1 = spot, 2 = point.

const int frMaxLights = 9;

/*
affects:
S: spot
PR: projector light
P: point
PE: particle
SL: sky layer
GI: gi
~X: all except X
*/

struct sFRLightParameters{
	vec3 color; // ~PE
	float range; // P, S, PR
	vec3 colorAmbient; // SL
	float ambientRatio; // P, S, PR
	vec3 colorAmbientGI; // GI
	
	float attenuationCoefficient;
	float dampingCoefficient;
	float dampingThreshold;
	
	float imageGamma;
	
	float spotFactor; // S, PR
	float spotBase; // S, PR
	float spotExponent; // S, PR
};

struct sFRInstanceParameters{
	mat4 matrixMVP[2];
	mat4x3 matrixMV[2];
	
	vec4 samplesParams; // PE: samplesScaleU, samplesOffsetU, samplesScaleV, samplesOffsetV
	float burstFactor; // PE
	
	// light params in camera space
	vec3 lightPosition[2]; // ~PE
	vec3 lightView[2]; // ~PE
	vec4 lightParams; // ~PE: x=depthCompare, y=shadow.znear, z=n/a, w=n/a
	
	// 0=disable, 1=greater, -1=less
	#define depthCompare lightParams.x
	
	mat4 shadowMatrix1[2]; // ~PE
	mat4 shadowMatrix2[2]; // ~PE
	mat4 shadowMatrix3[2]; // SL
	mat4 shadowMatrix4[2]; // SL
	vec4 layerBorder; // SL
	
	mat4x3 lightImageOmniMatrix[2]; // ~PE
	
	vec3 shadow1Solid; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 shadow1Transparent; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 shadow2Solid; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 shadow2Transparent; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	
	vec4 shadowDepthTransform; // ~PE: x=z2depth.scale, y=z2depth.offset, z=depth2z.scale, w=depth2z.offset
	vec4 shadowDepthTransform2; // SL: x=z2depth.scale, y=z2depth.offset, z=depth2z.scale, w=depth2z.offset
	
	mat4 giShadowMatrix; // SL
	vec3 giShadowParams; // SL: x=pixelSizeU, y=pixelSizeV, z=noiseScale
};

struct sFRLight{
	int type;
	
	// index into shadow array textures. this is the slot index. the real indices are these:
	// solid1: arrayIndex * 2
	// solid2: arrayIndex * 2 + 1
	int arrayIndex;
	
	int pcfMode; // PcfMode*
	
	bvec2 hasTextureShadow; // x=shadow1.solid, y=shadow2.solid
	
	sFRLightParameters light;
	sFRInstanceParameters instance;
};
