#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY

layout(location=0) in vec2 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
}
