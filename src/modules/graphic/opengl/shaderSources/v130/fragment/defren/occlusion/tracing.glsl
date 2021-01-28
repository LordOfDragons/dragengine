precision highp float;
precision highp int;

#include "v130/shared/raycasting.glsl"

uniform int pBVHInstanceRootNode;


out float outOcclusion;
// out vec2 outDistance;


#ifdef DEBUG_TRACING
const float PI = 3.14159265;
const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 256.0, PI / 256.0);
const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);
#endif


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	#ifdef DEBUG_TRACING
	{vec2 pc = vec2(tc) * TC2PCOORD_MUL + TC2PCOORD_ADD;
	float r = cos(pc.y);
	vec3 dir = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);
	
	RayCastResult result;
	if( rayCastInstance( pBVHInstanceRootNode, vec3( 0.0 ), dir, result ) ){
		outOcclusion = result.distance;
	}else{
		outOcclusion = 250.0;
	}
	return;}
	#endif
	
	outOcclusion = 1.0;
	//outDistance = 250.0;
}
