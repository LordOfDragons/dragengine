#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texReflectivity;
uniform mediump sampler2D texEmissivity;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out vec4 outDiffuse;
layout(location=1) out vec4 outReflectivity;
layout(location=2) out vec4 outEmissivity;

void main( void ){
	outDiffuse = textureLod( texDiffuse, vTexCoord, 0.0 );
	outReflectivity = textureLod( texReflectivity, vTexCoord, 0.0 );
	outEmissivity = textureLod( texEmissivity, vTexCoord, 0.0 );
}
