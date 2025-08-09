#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d/vertex.glsl"

const vec2 cv1 = vec2( -3.1415927, -1.57079633 ); // -pi, -pi/2
const vec2 cv2 = vec2( 0.0, 1.57079633 ); // 0, pi/2

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( inPosition, 0, 1 );
	
	// this is the basic relationship between the texture coordinates and the polar coordinates.
	// 
	//   s[0..1] = long[-pi..pi] / 2pi + 0.5
	//   t[0..1] = lat[0..pi] / pi
	// 
	// solved for long/lat using a fullscreen quad with upside down gives:
	// 
	//   long[-pi..pi] = posX[-1..1] * pi
	//   lat[0..pi] = pi/2 - posY[1..-1] * pi/2
	// 
	// long is inverted since in the calculation we need -sin(long) and to use a vec2 calculation
	// it is required for long to be positive when it comes into the fragment shader. this can be
	// done since cos(long) = cos(-long) and thus the negation does not change the other
	// calculation. the invertion is done using the s component of the cv1 constant
	vScreenCoord = inPosition * cv1 + cv2;
}
