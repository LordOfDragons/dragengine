precision highp float;
precision highp int;

#include "v130/shared/raycasting.glsl"
#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"


flat in int vOffset;
flat in vec3 vProbePosition;

out vec4 outPosition;

const float STEP_BACK_DISTANCE = 0.01;


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
		outPosition = vec4( vProbePosition, result.distance );
		outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
		// original DDGI code steps back using "hitNormal * 0.01"
		// // outPosition.xyz += direction * result.distance;
		// // outPosition.xyz += hitNormal * STEP_BACK_DISTANCE;
	}else{
		outPosition = vec4( vProbePosition, 10000.0 );
	}
	
	#endif
	
	#if RENDERDOC_DEBUG
	const float PI = 3.14159265;
	const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 64.0, PI / 64.0);
	const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);

	vec2 pc = vec2(tc.x % 64, tc.y) * TC2PCOORD_MUL + TC2PCOORD_ADD;
	float r = cos(pc.y);
	direction = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);

	//vec3 pp = vec3(0,-0.4*(int(tc.x)/64),0);
	//vec3 pp = vec3(2*(int(tc.x)/64),0,0);
	vec3 pp = vec3(0,0,-1*(tc.x/64));

	if( rayCastInstance( pBVHInstanceRootNode, pp, direction, result ) ){
		outPosition = vec4( pp, result.distance );
		outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
	}else{
		outPosition = vec4( pp, 10000.0 );
	}
	
	//if(tc.x/64 != 3) outPosition = vec4(0);
	#endif
}
