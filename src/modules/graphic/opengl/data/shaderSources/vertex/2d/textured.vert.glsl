#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform; // scaleU, scaleV, offsetU, offsetV

layout(location=0) in vec2 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
}
