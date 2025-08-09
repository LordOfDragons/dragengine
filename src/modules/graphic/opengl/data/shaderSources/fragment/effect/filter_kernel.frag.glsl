#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pOptions; // rows, cols, stepU, stepV
UNIFORM_BIND(4) uniform vec3 pKernel1;
UNIFORM_BIND(5) uniform vec3 pKernel2;
UNIFORM_BIND(6) uniform vec3 pKernel3;

layout(binding=0) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	vec3 tc = vec3( vTexCoord, vLayer );
	
	outColor = vec4( texture( texColor, tc + vec3( -pOptions.z, -pOptions.w, 0 ) ).rgb * pKernel1.x, 1 );
	outColor.rgb += texture( texColor, tc + vec3( 0, -pOptions.w, 0 ) ).rgb * pKernel1.y;
	outColor.rgb += texture( texColor, tc + vec3( pOptions.z, -pOptions.w, 0 ) ).rgb * pKernel1.z;
	
	outColor.rgb += texture( texColor, tc + vec3( -pOptions.z, 0, 0 ) ).rgb * pKernel2.x;
	outColor.rgb += texture( texColor, tc ).rgb * pKernel2.y;
	outColor.rgb += texture( texColor, tc + vec3( pOptions.z, 0, 0 ) ).rgb * pKernel2.z;
	
	outColor.rgb += texture( texColor, tc + vec3( -pOptions.z, pOptions.w, 0 ) ).rgb * pKernel3.x;
	outColor.rgb += texture( texColor, tc + vec3( 0, pOptions.w, 0 ) ).rgb * pKernel3.y;
	outColor.rgb += texture( texColor, tc + vec3( pOptions.z, pOptions.w, 0 ) ).rgb * pKernel3.z;
	
	outColor.rgb = max( outColor.rgb, vec3( 0 ) );
}
