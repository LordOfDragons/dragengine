/*
	<!-- shared/defren/light/ubo_instance_parameters.glsl -->
	<uniformBlock name='InstanceParameters' binding='1'/>
*/

/*
affects:
S: spot
P: point
PE: particle
SL: sky layer
~X: all except X
*/

UBOLAYOUT_BIND(1) uniform InstanceParameters{
	mat4 pMatrixMVP[ 2 ];
	mat4x3 pMatrixMV[ 2 ];
	
	vec4 pSamplesParams; // PE: samplesScaleU, samplesOffsetU, samplesScaleV, samplesOffsetV
	float pBurstFactor; // PE
	
	// light params in camera space
	vec3 pLightPosition[ 2 ]; // ~PE
	vec3 pLightView[ 2 ]; // ~PE
	vec4 pLightParams; // ~PE: x=depthCompare, y=shadow.znear, z=n/a, w=n/a
	
	// 0=disable, 1=greater, -1=less
	#define pDepthCompare pLightParams.x
	
	// near shadow clamp distance
	#define pLightShadowClamp pLightParams.y
	
	mat4 pShadowMatrix1[ 2 ]; // ~PE
	mat4 pShadowMatrix2[ 2 ]; // ~PE
	mat4 pShadowMatrix3[ 2 ]; // SL
	mat4 pShadowMatrix4[ 2 ]; // SL
	vec4 pLayerBorder; // SL
	
	mat4x3 pLightImageOmniMatrix[ 2 ]; // ~PE
	
	vec3 pShadow1Solid; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 pShadow1Transparent; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 pShadow2Solid; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	vec3 pShadow2Transparent; // ~PE: x=pixelSizeU, y=pixelSizeV, z=noiseScale
	
	vec4 pShadowDepthTransform; // ~PE: x=z2depth.scale, y=z2depth.offset, z=depth2z.scale, w=depth2z.offset
	vec4 pShadowDepthTransform2; // SL: x=z2depth.scale, y=z2depth.offset, z=depth2z.scale, w=depth2z.offset
	
	mat4 pGIShadowMatrix; // SL
	vec3 pGIShadowParams; // SL: x=pixelSizeU, y=pixelSizeV, z=noiseScale
};
