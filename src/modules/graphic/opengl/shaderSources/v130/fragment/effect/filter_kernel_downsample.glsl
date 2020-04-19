precision mediump float;
precision mediump int;

// uniform vec4 pOptions; // rows, cols, stepU, stepV
// uniform vec3 pKernel1;
// uniform vec3 pKernel2;
// uniform vec3 pKernel3;
uniform ivec2 pTCClamp;

uniform mediump sampler2D texColor;

in vec2 vTexCoord;

out mediump vec4 outColor;

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );
const vec4 weights = vec4( 0.25 );

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	
	outColor =  texelFetch( texColor, tc.xy, 0 ); // (s*2, t*2)
	outColor += texelFetch( texColor, tc.zy, 0 ); // (s*2+1, t*2)
	outColor += texelFetch( texColor, tc.xw, 0 ); // (s*2, t*2+1)
	outColor += texelFetch( texColor, tc.zw, 0 ); // (s*2+1, t*2+1)
	
	outColor *= weights;
}
