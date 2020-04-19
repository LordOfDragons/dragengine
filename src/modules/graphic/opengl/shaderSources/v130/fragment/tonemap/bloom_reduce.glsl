precision highp float;
precision highp int;

uniform ivec4 pParam1; // scale.s, scale.t, offset1.s, offset1.t
uniform ivec4 pParam2; // offset2.s, offset2.t, offset3.s, offset3.t

uniform mediump sampler2D texColor;

out vec3 outColor;

const vec3 weights = vec3( 0.25 );

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) * pParam1.xy;
	
	outColor = texelFetch( texColor, tc, 0 ).rgb;
	outColor += texelFetch( texColor, tc + pParam1.zw, 0 ).rgb;
	outColor += texelFetch( texColor, tc + pParam2.xy, 0 ).rgb;
	outColor += texelFetch( texColor, tc + pParam2.zw, 0 ).rgb;
	
	outColor *= weights;
}
