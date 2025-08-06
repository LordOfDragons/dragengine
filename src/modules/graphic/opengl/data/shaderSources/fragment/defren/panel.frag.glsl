#include "shared/preamble.glsl"

uniform HIGHP mat4 pColorTransform;
uniform HIGHP vec4 pColorTransform2;

layout(binding=0) uniform mediump sampler2D texTexture;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out lowp vec4 outColor;

void main( void ){
	outColor = pColorTransform * textureLod( texTexture, vTexCoord, 0.0 ) + pColorTransform2;
}
