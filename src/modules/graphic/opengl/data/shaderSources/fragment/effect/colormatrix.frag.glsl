#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform mat4 pColorMatrix;
UNIFORM_BIND(4) uniform vec4 pColorOffset;

layout(binding=0) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	outColor = pColorMatrix * texelFetch( texColor, ivec3( gl_FragCoord.xy, vLayer ), 0 ) + pColorOffset;
}
