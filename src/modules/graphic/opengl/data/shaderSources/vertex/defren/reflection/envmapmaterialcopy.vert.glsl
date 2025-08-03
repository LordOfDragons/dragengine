#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pSCTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY

layout(location=0) in vec2 inPosition;

out vec2 vScreenCoord;
out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( inPosition, 0.0, 1.0 );
	vScreenCoord = inPosition * pSCTransform.xy + pSCTransform.zw;
	vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
}
