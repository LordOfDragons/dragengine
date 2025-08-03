#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

// one pixel per instance
// pixel1: posTransform = (scaleX, scaleY, offsetX, offsetY)
uniform mediump samplerBuffer texData1;

// one pixel per instance
// pixel1: color = (r, g, b, a)
uniform mediump samplerBuffer texData2;

layout(location=0) in vec2 inPosition;

flat out vec4 vColor;

void main( void ){
	vec4 transform = texelFetch( texData1, gl_InstanceID );
	gl_Position = vec4( inPosition * transform.xy + transform.zw, 0.0, 1.0 );
	
	vColor = texelFetch( texData2, gl_InstanceID );
}
