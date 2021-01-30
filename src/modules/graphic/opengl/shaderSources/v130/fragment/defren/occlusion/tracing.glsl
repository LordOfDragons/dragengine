precision highp float;
precision highp int;

#include "v130/shared/raycasting.glsl"
#include "v130/shared/ubo_defines.glsl"

UBOLAYOUT uniform TracingParameters{
	vec2 pSampleImageScale;
	int pProbeCount;
	int pRaysPerProbe;
	int pProbesPerLine;
	int pBVHInstanceRootNode;
	ivec3 pGridProbeCount;
	vec3 pGridProbeSpacing;
	ivec4 pProbeIndex[ 64 ]; // max probe update count / 4
	vec3 pProbePosition[ 256 ]; // max probe update count
	vec3 pRayDirection[ 64 ]; // ray count
};


flat in int vOffset;
flat in vec3 vProbePosition;

out vec4 outPosition;


#ifdef DEBUG_TRACING
const float PI = 3.14159265;
const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 256.0, PI / 256.0);
const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);
#endif


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	#ifdef DEBUG_TRACING
	vec2 pc = vec2(tc) * TC2PCOORD_MUL + TC2PCOORD_ADD;
	float r = cos(pc.y);
	vec3 dir = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);
	
	RayCastResult result;
	if( rayCastInstance( pBVHInstanceRootNode, vec3( 0.0 ), dir, result ) ){
		outPosition = vec4( dir * result.distance, result.distance );
	}else{
		outPosition = vec4( 0.0, 0.0, 0.0, 250.0 );
	}
	
	#else
	RayCastResult result;
	vec3 direction = pRayDirection[ tc.x - vOffset ];
	if( rayCastInstance( pBVHInstanceRootNode, vProbePosition, direction, result ) ){
		outPosition = vec4( vProbePosition + direction * result.distance, result.distance );
	}else{
		outPosition = vec4( vProbePosition, 10000.0 );
	}
	
	#endif
}
