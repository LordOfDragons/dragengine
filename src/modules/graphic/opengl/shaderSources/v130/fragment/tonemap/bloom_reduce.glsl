precision highp float;
precision highp int;

uniform ivec4 pParam1; // scale.s, scale.t, offset1.s, offset1.t
uniform ivec4 pParam2; // offset2.s, offset2.t, offset3.s, offset3.t

uniform mediump sampler2DArray texColor;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outColor;

const vec3 weights = vec3( 0.25 );

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) * pParam1.xy;
	
	outColor = texelFetch( texColor, ivec3( tc, vLayer ), 0 ).rgb;
	outColor += texelFetch( texColor, ivec3( tc + pParam1.zw, vLayer ), 0 ).rgb;
	outColor += texelFetch( texColor, ivec3( tc + pParam2.xy, vLayer ), 0 ).rgb;
	outColor += texelFetch( texColor, ivec3( tc + pParam2.zw, vLayer ), 0 ).rgb;
	
	outColor *= weights;
}
