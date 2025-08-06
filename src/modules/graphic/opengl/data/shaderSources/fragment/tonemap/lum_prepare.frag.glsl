#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform HIGHP sampler2DArray texColor;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out float outLuminance;

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	outLuminance = dot( texelFetch( texColor, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, lumiFactors );
}
