#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

VARYING_BIND(0) out vec2 vTexCoord;
VARYING_BIND(1) out vec2 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	vScreenCoord = inPosition;
	vTexCoord = inPosition * pQuadParams.xy + pQuadParams.zw;
}
