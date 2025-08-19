#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

UNIFORM_BIND(2) uniform lowp vec4 pColor;

layout(binding=0) uniform lowp sampler2D texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out lowp vec4 outColor;

void main( void ){
	outColor = texture( texColor, vTexCoord ) * pColor;
}
