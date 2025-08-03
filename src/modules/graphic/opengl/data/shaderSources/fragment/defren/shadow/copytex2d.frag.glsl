#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform HIGHP sampler2D texDepth;

void main( void ){
	gl_FragDepth = texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r;
}
