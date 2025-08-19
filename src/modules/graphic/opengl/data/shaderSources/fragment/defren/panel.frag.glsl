#include "shared/preamble.glsl"

UNIFORM_BIND(3) uniform HIGHP mat4 pColorTransform;
UNIFORM_BIND(4) uniform HIGHP vec4 pColorTransform2;

layout(binding=0) uniform mediump sampler2D texTexture;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out lowp vec4 outColor;

void main( void ){
	outColor = pColorTransform * textureLod( texTexture, vTexCoord, 0.0 ) + pColorTransform2;
}
