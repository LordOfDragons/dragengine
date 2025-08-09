#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

layout(binding=0) uniform lowp sampler2DArray texValues;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out lowp float outDummy;

void main( void ){
	// the 0.1 more than 255 is only use to ensure rounding
	if( int( texelFetch( texValues, ivec3( 0, 0, vLayer ), 0 ).r * 255.1 ) <= int( gl_FragCoord.x ) ){
		discard;
	}
	outDummy = 1.0;
}
