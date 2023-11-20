#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_gi.glsl"

uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixP;
uniform int pGICascade;
uniform int pGIDebugProbe;
uniform int pGIRaysPerProbe;
uniform int pGIProbesPerLine;
uniform vec3 pProbePosition;
uniform ivec2 pUpdateDataRayOffset;
uniform int pRayDataValid;

#include "shared/defren/gi/constants.glsl"

UBOLAYOUT uniform GIRayDirections{
	vec3 pGIRayDirection[ GI_MAX_COUNT_RAY_DIRECTIONS ];
};

UBOLAYOUT_BIND(2) restrict buffer RayData {
	vec4 pRayData[ GI_MAX_COUNT_RAY_DIRECTIONS ];
};

#include "shared/defren/gi/raycast/ray_cache.glsl"

uniform mediump sampler2DArray texCacheDistance;
uniform mediump sampler2D texLight;

in vec2 inPosition;

out vec2 vTexCoord;
flat out vec3 vColor;
flat out vec3 vColorRim;
flat out float vRimThickness;

void main( void ){
	ivec3 tcCache = giRayCastCacheTCFromProbeIndex( pGIDebugProbe, gl_InstanceID );
	float dist = texelFetch( texCacheDistance, tcCache, 0 ).r;
	vec3 position = pProbePosition + pGIRayDirection[ gl_InstanceID ] * dist;
	
	vTexCoord = inPosition;
	
	vec3 center = pMatrixMV * vec4( position, 1 );
	
	vRimThickness = mix( 0.98, 0.5, clamp( center.z / 15, 0, 1 ) );
	
	vColor = vec3( 0 );
	vColorRim = vec3( 1, 0, 0 );
	
	if( pUpdateDataRayOffset.x != -1 ){
		pRayData[ gl_InstanceID ] = vec4( texelFetch( texLight, pUpdateDataRayOffset + ivec2( gl_InstanceID, 0 ), 0 ) );
	}
	
	if( pRayDataValid == 1 ){
		vColor = pRayData[ gl_InstanceID ].rgb;
		vColorRim = vec3( 0, 1, 0 );
	}
	
	const float sphereScale = 0.05;
	gl_Position = pMatrixP * vec4( center + vec3( inPosition * sphereScale, 0.0 ), 1.0 );
}
