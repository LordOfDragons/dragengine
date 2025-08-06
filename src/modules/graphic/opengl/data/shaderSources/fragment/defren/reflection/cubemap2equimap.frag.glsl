#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform float pLevel;

layout(binding=0) uniform mediump samplerCube texEnvMap;

#include "shared/interface/2d_fragment.glsl"
// vScreenCoord: -long, lat

layout(location=0) out vec4 outColor;

void main( void ){
	// calculate the texture cube map texture coordinates for this pixel. this is:
	// 
	//   x = -sin(long) * sin(lat)
	//   y =  cos(lat)
	//   z =  cos(long) * sin(lat)
	// 
	// to use sin and cos on a vec2 the long value is negated in the vertex shader. this works since
	// cos(long) = cos(-long) and -sin(long) = sin(-long)
	vec4 temp = vec4( sin( vScreenCoord ), cos( vScreenCoord ) ); // -sin(long), sin(lat), cos(long), cos(lat)
	vec3 dir;
	
	dir = temp.ywy;
	dir.xz *= temp.xz;
	
	outColor = textureLod( texEnvMap, dir, pLevel );
}
