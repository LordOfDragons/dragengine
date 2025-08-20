#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(2) uniform mat4x3 pMatrixReflectionBox;
UNIFORM_BIND(3) uniform mat4x3 pMatrixMaskBox;

VARYING_BIND(0) in vec3 vEnvMapDir;

layout(location=0) out vec4 outMask;



void main( void ){
	// calculate the hit point on the reflection box through a cube map pixel
	vec3 localPosition = pMatrixReflectionBox * vec4( 0.0, 0.0, 0.0, 1.0 ); // (0,0,0) => center of envmap is camera center
	vec3 localReflectDir = mat3( pMatrixReflectionBox ) * vEnvMapDir;
	
	vec3 reflectDirDist = mix( (sign(localReflectDir)-localPosition)/localReflectDir, vec3(1e6), lessThan( abs(localReflectDir), vec3(1e-5) ) );
	reflectDirDist.x = min( reflectDirDist.x, min( reflectDirDist.y, reflectDirDist.z ) );
	
	vec3 hitPoint = vEnvMapDir * vec3( reflectDirDist.x ); // (0,0,0) + vEnvMapDir * hitDistance
	
	// determine if the hitpoint is inside the mask box. if this is the case write out a mask of 0 otherwise 1
	outMask = vec4( mix( 1.0, 0.0, all( lessThan( abs( pMatrixMaskBox * vec4( hitPoint, 1.0 ) ), vec3( 1.0 ) ) ) ) );
}
