#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform mediump sampler2D texTexture;

VARYING_BIND(0) flat in vec4 vColor;
VARYING_BIND(1) in vec2 vTexCoord;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = textureLod( texTexture, vTexCoord, 0.0 ) * vec4( vColor );
}
