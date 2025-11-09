#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

// two pixels per instance
// pixel1: posTransform = (scaleX, scaleY, offsetX, offsetY)
// pixel2: tcTransform = (scaleX, scaleY, offsetX, offsetY)
layout(binding=1) uniform mediump samplerBuffer texData1;

// one pixel per instance
// pixel1: color = (r, g, b, a)
layout(binding=2) uniform mediump samplerBuffer texData2;

layout(location=0) in vec2 inPosition;

VARYING_BIND(0) flat out vec4 vColor;
VARYING_BIND(1) out vec2 vTexCoord;

void main( void ){
	vec4 transform = texelFetch( texData1, gl_InstanceID * 2 );
	gl_Position = vec4( inPosition * transform.xy + transform.zw, 0.0, 1.0 );
	
	transform = texelFetch( texData1, gl_InstanceID * 2 + 1 );
	vTexCoord = inPosition * transform.xy + transform.zw;
	
	vColor = texelFetch( texData2, gl_InstanceID );
}
