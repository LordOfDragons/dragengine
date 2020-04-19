precision highp float;
precision highp int;

uniform float pLevel;

uniform mediump samplerCube texEnvMap;

in vec2 vScreenCoord; // -long, lat

out vec4 outColor;

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
