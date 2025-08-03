#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

UNIFORM_BIND(0) uniform lowp vec4 pColor;

uniform lowp sampler2D texColor;

layout(location=0) in vec2 vTexCoord;

layout(location=0) out lowp vec4 outColor;

void main( void ){
	outColor = texture( texColor, vTexCoord ) * pColor;
}
