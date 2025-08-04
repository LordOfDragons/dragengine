#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

// one pixel per instance
// pixel1: posTransform = (scaleX, scaleY, offsetX, offsetY)
layout(binding=0) uniform mediump samplerBuffer texData1;

// one pixel per instance
// pixel1: color = (r, g, b, a)
layout(binding=1) uniform mediump samplerBuffer texData2;

layout(location=0) in vec2 inPosition;

VARYING_BIND(0) flat out vec4 vColor;

void main( void ){
	vec4 transform = texelFetch( texData1, gl_InstanceID );
	gl_Position = vec4( inPosition * transform.xy + transform.zw, 0.0, 1.0 );
	
	vColor = texelFetch( texData2, gl_InstanceID );
}
